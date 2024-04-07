#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "driver/ledc.h"

typedef struct 
{
    uint8_t channel;
    uint8_t timer_resolution;
}__attribute__((packed))PWM_Data_t;


void PWM_Init(PWM_Data_t *pwm_x, uint32_t io_pin, ledc_timer_t timer, 
                ledc_channel_t channel, ledc_timer_bit_t timer_resolution, uint32_t frequency);

void PWM_SetValue(PWM_Data_t *pwm_x, uint8_t duty);

#endif /* PWM_H */