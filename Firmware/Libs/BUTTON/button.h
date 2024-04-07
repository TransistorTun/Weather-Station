#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"

typedef void (* BUTTON_CallBack_Function_t) (uint32_t pin);

typedef struct 
{
    gpio_num_t pin_button;
    uint8_t is_pressing;
    uint64_t pressing_timer;
    uint64_t debouncing_timer;
}BUTTON_Data_t;


esp_err_t BUTTON_Init(BUTTON_Data_t *button_x, gpio_num_t io_pin);
void BUTTON_Exit_Handle(BUTTON_Data_t *button_x, gpio_num_t io_pin);
void BUTTON_SetCallBackFunction(BUTTON_CallBack_Function_t short_Pressing_Function,
                                    BUTTON_CallBack_Function_t long_Pressing_Function,
                                    BUTTON_CallBack_Function_t Releasing_Function);

#endif /* BUTTON_H */