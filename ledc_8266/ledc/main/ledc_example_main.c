#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "driver/pwm.h"
#include "driver/ledc.h"

#define PWM_0_OUT_IO_NUM 12
#define PWM_1_OUT_IO_NUM 15
#define PWM_2_OUT_IO_NUM 14
//#define PWM_3_OUT_IO_NUM 15
//#define LEDC_CH3_GPIO       13

#define LEDC_PERIOD         (1000)
#define LEDC_TEST_DUTY      (500)
#define LEDC_TEST_FADE_TIME (3000)
#define LEDC_TEST_CH_NUM    (3)  

static const char *TAG = "ledc_example";



void app_main()
{
    int ch = 0;

    ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
        {
            .channel_num    = 0,
            .period         = LEDC_PERIOD,
            .duty           = 10,
            .gpio_num       = PWM_0_OUT_IO_NUM,
        },
        {
            .channel_num    = 1,
            .period         = LEDC_PERIOD,
            .duty           = 10,
            .gpio_num       = PWM_1_OUT_IO_NUM,
        },
        {
            .channel_num    = 2,
            .period         = LEDC_PERIOD,
            .duty           = 10,
            .gpio_num       = PWM_2_OUT_IO_NUM,
        },
    };

    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
    {
        printf("gpio: %d, duty:%d;\n", ledc_channel[ch].gpio_num, ledc_channel[ch].duty);
        ledc_channel_config(&ledc_channel[ch]);
    }

   
    while(1)
    {
        printf("1. LEDC fade up to duty = %d\n", LEDC_TEST_DUTY);
        for(ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
        {
            ledc_set_fade_with_time(ledc_channel[ch].channel_num, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
            ledc_fade_start(ledc_channel[ch].channel_num, LEDC_FADE_NO_WAIT);   
        }
        vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

        printf("2. LEDC fade down to duty = %d\n", LEDC_TEST_DUTY);
        for(ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
        {
            ledc_set_fade_with_time(ledc_channel[ch].channel_num, 0, LEDC_TEST_FADE_TIME);
            ledc_fade_start(ledc_channel[ch].channel_num, LEDC_FADE_NO_WAIT);   
        }
        vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

        printf("3. LEDC set duty = %d without fade\n", LEDC_TEST_DUTY);
        for(ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
        {
            ledc_set_duty(ledc_channel[ch].channel_num, LEDC_TEST_DUTY);
            ledc_update_duty(ledc_channel[ch].channel_num);   
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        printf("4. LEDC set duty = 0 without fade\n");
        for(ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
        {
            ledc_set_duty(ledc_channel[ch].channel_num, 0);
            ledc_update_duty(ledc_channel[ch].channel_num);   
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}