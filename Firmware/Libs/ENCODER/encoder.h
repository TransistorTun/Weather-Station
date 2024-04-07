#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>
#include "exti.h"
#include "esp_err.h"

typedef struct 
{
    uint32_t phase_a;
    uint32_t phase_b;
    uint32_t sw;
}ENCODER_data_t;

esp_err_t ENCODER_Init(ENCODER_data_t *encoder_x, uint32_t phase_a, 
            uint32_t phase_b, uint32_t button, EXTI_EDGE_t edge);
int64_t ENCODER_Read(ENCODER_data_t *encoder_x);
void ENCODER_Reset();


#endif /* ENCODER_H_*/