#include "exti.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_attr.h"
#include "math.h"

static uint8_t          is_initialize = 0;
static QueueHandle_t    exti_event_queue = NULL;
static TaskHandle_t     exti_handle_task = NULL;
static EXIT_CALLBACK_FUNCTION_t EXIT_Callback_Function = NULL;

static void IRAM_ATTR GPIO_ISR_Handler(void *argument)
{
    uint32_t io_pin = (uint32_t)argument;
    xQueueSendFromISR(exti_event_queue, &io_pin, NULL);
}

static void EXIT_Task_Handle()
{
    uint32_t io_pin = 0;
    while (1)
    {
        if (xQueueReceive(exti_event_queue, &io_pin, portMAX_DELAY))
        {
            if (EXIT_Callback_Function != NULL)
            {
                EXIT_Callback_Function(io_pin);
            }
            
        }
        
    }
    
}

esp_err_t EXIT_Init(gpio_num_t gpio_pin, EXTI_EDGE_t edge)
{
    if (is_initialize == 0)
    {
        exti_event_queue = xQueueCreate(10, sizeof(uint32_t));
        xTaskCreate(EXIT_Task_Handle, "exti_handle_task", 2048, 0, 15, &exti_handle_task);
        gpio_install_isr_service(0);
        is_initialize = 1;
    }
    gpio_config_t io_config = 
    {
        .pin_bit_mask   = pow(2, gpio_pin),
        .mode           = GPIO_MODE_INPUT,
        .pull_up_en     = GPIO_PULLUP_ENABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = edge,
    };
    gpio_config(&io_config);
    gpio_isr_handler_add(gpio_pin, GPIO_ISR_Handler, (void *)gpio_pin);
    return ESP_OK;

}
void EXIT_SetCallBackFunction(EXIT_CALLBACK_FUNCTION_t callbackFunction)
{
    EXIT_Callback_Function = callbackFunction;
}