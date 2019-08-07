#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"

#include "esp8266/eagle_soc.h"
#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "esp_heap_caps.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/pwm.h"
// Temporary use the FreeRTOS critical function
#include "FreeRTOS.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


#define MAX_PWM_CHANNEL (8)
#define MAX_DELAY (10)

static const char* LEDC_TAG = "ledc";

//static portMUX_TYPE ledc_spinlock = portMUX_INITIALIZER_UNLOCKED;
#define LEDC_CHECK(s, str, ret_val)                                   \
    if (!(a)) {                                                       \
        ESP_LOGE(LED_TAG, "%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val);                                             \
    }

#define LED_ARG_CHECK(a, param) LEDC_CHECK(a, param " argument is invalid", ESP_ERR_INVALID_ARG)

typedef struct {
    uint8_t channel_num; //通道编号
    uint32_t duty_p; //当前占空比
    uint32_t duty; // 目标占空比
    uint32_t step_duty; //步进值
    //    uint32_t fade_time; //fade所需时间
} ledc_t;

QueueHandle_t channel_Queue;
static ledc_t ledc[MAX_PWM_CHANNEL] = { 0 };
static uint8_t Ledc_Max = 0;

static uint8_t ledc_channel_num;
static uint32_t ledc_gpio_num[MAX_PWM_CHANNEL];
static uint32_t ledc_duty[MAX_PWM_CHANNEL];
static int16_t ledc_phase[MAX_PWM_CHANNEL];
static uint8_t ledc_wait_time;

esp_err_t ledc_set_duty(uint8_t ledc_channel, uint32_t ledc_duty)
{
    ledc[ledc_channel].channel_num = ledc_channel;
    ledc[ledc_channel].duty = ledc_duty;
    pwm_get_duty(ledc_channel, &ledc[ledc_channel].duty_p);
    /*目前占空比和目标占空比之差 */
    ledc[ledc_channel].step_duty = (ledc[ledc_channel].duty_p > ledc[ledc_channel].duty ? ledc[ledc_channel].duty_p - ledc[ledc_channel].duty : ledc[ledc_channel].duty - ledc[ledc_channel].duty_p);
    ESP_LOGI(LEDC_TAG, "channel_num = %d; duty = %d; duty_p = %d; step_duty = %d;\n",
        ledc[ledc_channel].channel_num, ledc[ledc_channel].duty,
        ledc[ledc_channel].duty_p, ledc[ledc_channel].step_duty);

    return ESP_OK;
}

esp_err_t ledc_update_duty(uint8_t ledc_channel)
{

    //发送队列
    uint8_t ret = xQueueSend(channel_Queue, &ledc_channel, LEDC_STEP_TIME);
    ESP_LOGI(LEDC_TAG, "send............channel:%d.....\n", ledc_channel);
    if (ret != pdPASS) {
        ESP_LOGI(LEDC_TAG, "xQueueSend err\r\n");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t ledc_set_fade_with_time(uint8_t ledc_channel, uint32_t ledc_duty, uint32_t ledc_fade_time)
{
    ledc[ledc_channel].channel_num = ledc_channel;
    ledc[ledc_channel].duty = ledc_duty;


    pwm_get_duty(ledc_channel, &ledc[ledc_channel].duty_p);

    ledc[ledc_channel].step_duty = (ledc[ledc_channel].duty_p > ledc[ledc_channel].duty ? 
    (ledc[ledc_channel].duty_p - ledc[ledc_channel].duty) / (ledc_fade_time / LEDC_STEP_TIME) :
     (ledc[ledc_channel].duty - ledc[ledc_channel].duty_p) / (ledc_fade_time / LEDC_STEP_TIME));

    ESP_LOGI(LEDC_TAG, "channel_num = %d; duty = %d; duty_p = %d; step_duty = %d;\n",
        ledc[ledc_channel].channel_num, ledc[ledc_channel].duty,
        ledc[ledc_channel].duty_p, ledc[ledc_channel].step_duty);

    return ESP_OK;
}

esp_err_t ledc_fade_start(uint8_t ledc_channel, uint32_t wait_time)
{

    //wait
    vTaskDelay(wait_time / portTICK_PERIOD_MS);

    return ledc_update_duty(ledc_channel);
}

esp_err_t ledc_channel_config(const ledc_channel_config_t* ledc_conf)
{
    ESP_LOGI(LEDC_TAG, "ledc_init---start!!\n");


    ledc_gpio_num[Ledc_Max] = ledc_conf->gpio_num;
    ledc_duty[Ledc_Max] = ledc_conf->duty;
    ledc_phase[Ledc_Max] = ledc_conf->phase;
    printf("gpio: %d, duty:%d, phase:%d;\n", ledc_gpio_num[Ledc_Max], ledc_duty[Ledc_Max], ledc_phase[Ledc_Max]);
    Ledc_Max++;
    /*     
    if(ledc_conf->vel != 0)
    {
        pwm_set_channel_invert(0x1 << ledc_conf->channel_num);//占位，以后做修改，目前只拿改一个
    }
  */
    ESP_LOGI(LEDC_TAG, "ledc_init---end!!\n");
    return ESP_OK;
}

uint8_t ledc_fade_up(uint8_t channel)
{

    if (ledc[channel].duty_p < ledc[channel].duty) {
        ledc[channel].duty_p += ledc[channel].step_duty;
        pwm_set_duty(channel, ledc[channel].duty_p);
        pwm_start();
    }

    if (ledc[channel].duty_p < ledc[channel].duty) {
        return FLAG_ON;
    } else {
        return FLAG_OFF;
    }
}

uint8_t ledc_fade_down(uint8_t channel)
{

    if (ledc[channel].duty_p > ledc[channel].duty) {
        ledc[channel].duty_p -= ledc[channel].step_duty;
        pwm_set_duty(channel, ledc[channel].duty_p);
        pwm_start();
    }

    if (ledc[channel].duty_p > ledc[channel].duty) {
        return FLAG_ON;
    } else {
        return FLAG_OFF;
    }
}

/*完成消息队列接收同时改变占空比 */
void ledc_task(void* pvParameters)
{
    uint8_t channel;
    uint8_t i;
    uint8_t flag[MAX_PWM_CHANNEL] = { 0 };
    uint8_t ret;
    while (1) {
        if (xQueueReceive(channel_Queue, &channel, LEDC_STEP_TIME)) {
            flag[channel] = FLAG_ON;
        }

        for (i = 0; i < MAX_PWM_CHANNEL; i++) {
            if (flag[i] == FLAG_ON) {
                if (ledc[i].duty_p < ledc[i].duty) {
                    ret = ledc_fade_up(i);
                } else {
                    ret = ledc_fade_down(i);
                }
                flag[i] = ret;
                
            }
        }
    }
    vTaskDelete(NULL);
}

esp_err_t ledc_fade_func_install(void)
{
    //need to change
    pwm_init(1000, ledc_duty, Ledc_Max , ledc_gpio_num);
    printf("ledc_max:%d\n", Ledc_Max);
    for(int i = 0; i < Ledc_Max + 1; i++)
    {
        printf("gpio:%d\n", ledc_gpio_num[i]);
    }

    pwm_set_phases(ledc_phase);
    channel_Queue = xQueueCreate(4, sizeof(uint8_t));
    if (channel_Queue == 0) {
        ESP_LOGI(LEDC_TAG, "xQueueCreate err\r\n");
    }
    return (pwm_start() || xTaskCreate(ledc_task, "ledc_task", 1024, NULL, 2, NULL));
}
