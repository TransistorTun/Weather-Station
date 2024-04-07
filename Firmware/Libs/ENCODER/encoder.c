#include "encoder.h"

static int64_t scroll = 0;
static uint8_t flag = 0;
static ENCODER_data_t *encoder_tempt = NULL;

static void function_callback_pulse(uint32_t io_pin)
{
    if (io_pin == encoder_tempt -> phase_a)
    {
        if (flag == 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
            scroll--;
        }
        
    }
    
    if (io_pin == encoder_tempt -> phase_b)
    {
        if (flag == 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
            scroll++;
        }
        
    }
}

esp_err_t ENCODER_Init(ENCODER_data_t *encoder_x, uint32_t phase_a, 
            uint32_t phase_b, uint32_t button, EXTI_EDGE_t edge)
{
    encoder_x -> phase_a = phase_a;
    encoder_x -> phase_b = phase_b;
    encoder_x -> sw = button;
    EXIT_Init(phase_a, edge);
    EXIT_Init(phase_b, edge);
    return ESP_OK;
}

int64_t ENCODER_Read(ENCODER_data_t *encoder_x)
{
    encoder_tempt = encoder_x;
    EXIT_SetCallBackFunction(function_callback_pulse);
    return scroll;
}

inline void ENCODER_Reset()
{
    scroll = 0;
}
