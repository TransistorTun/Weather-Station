#include "button.h"
#include "exti.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include <stdio.h>

#include "esp_log.h"
#include "esp_timer.h"

#define GET_TICK (xTaskGetTickCount() * portTICK_PERIOD_MS)
#define PRESSING_TIMEOUT 2000 // ms

#define BUTTON_Short_Pressing_BIT (1 << 0)
#define BUTTON_Long_Pressing_BIT (1 << 1)
#define BUTTON_Releasing_BIT (1 << 2)

static BUTTON_CallBack_Function_t BUTTON_Short_Pressing_CallBack_Function = NULL;
static BUTTON_CallBack_Function_t BUTTON_Long_Pressing_CallBack_Function = NULL;
static BUTTON_CallBack_Function_t BUTTON_Releasing_CallBack_Function = NULL;

static uint8_t is_initialize = 0;
static BUTTON_Data_t *current_button = NULL;

static EventGroupHandle_t button_event = NULL;
static TaskHandle_t button_debouncing_task = NULL;
static TaskHandle_t button_executing_task = NULL;
static TimerHandle_t button_time_pressing = NULL;
static QueueHandle_t button_queue_data = NULL;

static void BUTTON_Debouncing_Task(void *arg)
{
    BUTTON_Data_t *button_temp = NULL;
    while (1)
    {
        if (xQueueReceive(button_queue_data, &button_temp, portMAX_DELAY))
        {
            current_button = button_temp;

            if ((GET_TICK - button_temp->debouncing_timer) < 20)
            {
                continue;
            }
            button_temp->debouncing_timer = GET_TICK;
            // vTaskDelay(50 / portTICK_PERIOD_MS);

            if (gpio_get_level(button_temp->pin_button) == 0)
            {
                button_temp->pressing_timer = GET_TICK;
                xTimerReset(button_time_pressing, 0);
            }
            else
            {
                xTimerStop(button_time_pressing, 0);
                xEventGroupSetBits(button_event, BUTTON_Releasing_BIT);
                if ((GET_TICK - button_temp->pressing_timer) < PRESSING_TIMEOUT)
                {
                    xEventGroupSetBits(button_event, BUTTON_Short_Pressing_BIT);
                }
            }
        }
    }
}

static void BUTTON_Executing_Task(void *arg)
{
    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(button_event,
                                                 BUTTON_Short_Pressing_BIT | BUTTON_Releasing_BIT,
                                                 pdTRUE, pdFALSE, portMAX_DELAY);

        if (uxBits & BUTTON_Short_Pressing_BIT)
        {
            if (BUTTON_Short_Pressing_CallBack_Function != NULL && current_button != NULL)
            {
                BUTTON_Short_Pressing_CallBack_Function(current_button->pin_button);
            }
        }

        if (uxBits & BUTTON_Releasing_BIT)
        {
            if (BUTTON_Releasing_CallBack_Function != NULL && current_button != NULL)
            {
                BUTTON_Releasing_CallBack_Function(current_button->pin_button);
            }
        }
    }
}

static void BUTTON_Timer_CallBack_Function()
{
    xTimerStop(button_time_pressing, 0);
    BUTTON_Long_Pressing_CallBack_Function(current_button->pin_button);
}

esp_err_t BUTTON_Init(BUTTON_Data_t *button_x, gpio_num_t io_pin)
{
    if (is_initialize == 0)
    {
        button_event = xEventGroupCreate();
        button_queue_data = xQueueCreate(1, sizeof(BUTTON_Data_t *));

        button_time_pressing = xTimerCreate("button_time_pressing",
                                            PRESSING_TIMEOUT / portTICK_PERIOD_MS,
                                            pdFALSE, (void *)0, BUTTON_Timer_CallBack_Function);

        xTaskCreate(BUTTON_Debouncing_Task, "BUTTON_Debouncing_Task",
                    2048, NULL, 14, &button_debouncing_task);

        xTaskCreate(BUTTON_Executing_Task, "BUTTON_Executing_Task",
                    2048, NULL, 13, &button_executing_task);

        is_initialize = 1;
    }
    button_x->debouncing_timer = 0;
    button_x->is_pressing = 0;
    button_x->pressing_timer = 0;
    button_x->pin_button = io_pin;
    EXIT_Init(button_x->pin_button, EXTI_EDGE_ALL);
    return ESP_OK;
}

void BUTTON_Exit_Handle(BUTTON_Data_t *button_x, gpio_num_t io_pin)
{
    if (button_x->pin_button == io_pin)
    {
        // button_x->debouncing_timer = GET_TICK;
        xQueueSend(button_queue_data, (void *)&button_x, 0);
    }
}
void BUTTON_SetCallBackFunction(BUTTON_CallBack_Function_t Short_Pressing_Function,
                                BUTTON_CallBack_Function_t Long_Pressing_Function,
                                BUTTON_CallBack_Function_t Releasing_Function)
{
    BUTTON_Long_Pressing_CallBack_Function = Long_Pressing_Function;
    BUTTON_Short_Pressing_CallBack_Function = Short_Pressing_Function;
    BUTTON_Releasing_CallBack_Function = Releasing_Function;
}