#include "neopixel.h"
#include "pwm.h"
#include <stdlib.h>

#include "esp_log.h"

// //pwm parameters
// #define PWM_HI (64)
// #define PWM_LO (32)
// #define NUM_BPP (3)

// typedef struct 
// {
//     uint8_t num_of_pixel;
//     uint8_t num_of_byte;
// }neo_pixel_data_t;

// neo_pixel_data_t *neo_pixel;
// static PWM_Data_t pwm_0_0;

// uint_fast8_t wr_buf_p = 0;
// uint8_t *rgb_arr;
// uint8_t *wr_buf;

// static void led_render(uint32_t color)
// {
    
// }

// void NEO_PIXEL_Init(uint8_t num_led)
// {
//     neo_pixel -> num_of_pixel = num_led;
//     neo_pixel -> num_of_byte = NUM_BPP * num_led;
//     rgb_arr = (uint8_t *)malloc(neo_pixel -> num_of_byte);
//     wr_buf = (uint8_t *)malloc(neo_pixel -> num_of_byte);

//     PWM_Init(&pwm_0_0, OUTPUT_PIN, 0, 0, 13, 800000);
// }
// void ALL_Black_Render(void);
// void ONE_Color_Render(uint8_t blue, uint8_t red, uint8_t green);
// void RENDER_Raising_Mode(uint8_t blue, uint8_t red, uint8_t green, uint64_t ms_delay);
// void RENDER_Falling_Mode(uint8_t blue, uint8_t red, uint8_t green, uint64_t ms_delay);
// void RENDER_Rainbow_Mode(uint64_t ms_delay);