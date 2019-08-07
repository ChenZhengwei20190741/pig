#ifndef __LEDC_H__
#define __LEDC_H__

#define LEDC_FADE_NO_WAIT (0)
#define DUTY_GO (0)
#define DUTY_BACK (1)
#define LEDC_STEP_TIME (10)
#define FLAG_ON (1)
#define FLAG_OFF (0)

typedef struct {
    uint8_t channel_num; //通道编号
    uint32_t period;
    uint32_t duty; //一开始的占空比设置
    uint32_t gpio_num; //gpio设置
    int16_t phase;
    uint8_t vel; //高低电平设置
} ledc_channel_config_t;

esp_err_t ledc_set_duty(uint8_t ledc_channel, uint32_t ledc_duty);
esp_err_t ledc_update_duty(uint8_t ledc_channel);
esp_err_t ledc_set_fade_with_time(uint8_t ledc_channel, uint32_t ledc_duty, uint32_t ledc_fade_time);
esp_err_t ledc_fade_start(uint8_t ledc_channel, uint32_t wait_time);
esp_err_t ledc_channel_config(const ledc_channel_config_t* ledc_conf);
uint8_t ledc_fade_up(uint8_t channel);
uint8_t ledc_fade_down(uint8_t channel);
void ledc_task(void* pvParameters);
esp_err_t ledc_fade_func_install(void);

#endif