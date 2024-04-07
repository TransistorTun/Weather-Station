#include "pwm.h"
#include "esp_err.h"
#include "esp_log.h"

#define LEDC_MODE               LEDC_LOW_SPEED_MODE

static const char *PWM_TAG = "PWM";

void PWM_Init(PWM_Data_t *pwm_x, uint32_t io_pin, ledc_timer_t timer, 
                ledc_channel_t channel, ledc_timer_bit_t timer_resolution, uint32_t frequency)
{
    if (channel >= 8)
    {
        ESP_LOGE(PWM_TAG, "Channel Max: 7");
    }
    else if (timer >= 4)
    {
        ESP_LOGE(PWM_TAG, "Timer Max: 3");
    }
    else if (timer_resolution >= 21)
    {
        ESP_LOGE(PWM_TAG, "Timer resolution max 20 or 14");
    }
    else
    {   
        pwm_x -> channel = channel;
        pwm_x -> timer_resolution = timer_resolution;
        // Prepare and then apply the LEDC PWM timer configuration
        ledc_timer_config_t ledc_timer = {
            .speed_mode       = LEDC_MODE,
            .timer_num        = timer,
            .duty_resolution  = timer_resolution,
            .freq_hz          = frequency,
            .clk_cfg          = LEDC_AUTO_CLK
        };
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

        // Prepare and then apply the LEDC PWM channel configuration
        ledc_channel_config_t ledc_channel = {
            .speed_mode     = LEDC_MODE,
            .channel        = channel,
            .timer_sel      = timer,
            .intr_type      = LEDC_INTR_DISABLE,
            .gpio_num       = io_pin,
            .duty           = 0, // Set duty to 0%
            .hpoint         = 0
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    }
}

void PWM_SetValue(PWM_Data_t *pwm_x, uint8_t duty)
{
    if (duty > 100)
    {
        duty = 100;
    }
    
    uint32_t tempt = 2;
    for (uint8_t i = 1; i < pwm_x -> timer_resolution; i++)
    {
        tempt *= 2;
    }

    // Set duty cycle
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, pwm_x -> channel, (tempt - 1) * duty / 100));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, pwm_x -> channel));
}