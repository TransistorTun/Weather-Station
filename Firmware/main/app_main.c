#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "cJSON.h"
#include "sdkconfig.h"

#include "gpio.h"
#include "dht.h"
#include "ds3231.h"
#include "button.h"
#include "exti.h"
#include "pwm.h"
#include "encoder.h"
#include "wifi.h"
#include "tft.h"
#include "macro.h"
#include "font.h"
#include "spiffs.h"
#include "icon.h"
#include "nvs_rw.h"

#define PIN_SDA GPIO_NUM_18
#define PIN_SCL GPIO_NUM_19

#define CS0 GPIO_NUM_15
#define SCLK GPIO_NUM_14
#define MOSI GPIO_NUM_13
#define TFT_DC GPIO_NUM_22
#define TFT_RESET GPIO_NUM_21

#define LED_NEO GPIO_NUM_27

#define BUTTON_SETTING GPIO_NUM_26
#define ROTARY_PHASE_A GPIO_NUM_33
#define ROTARY_PHASE_B GPIO_NUM_25

#define DATA_DHT11 GPIO_NUM_32

#define EVENT_DISCONNECT_WIFI (1 << 0)
#define EVENT_CONNECT_WIFI (1 << 1)
#define EVENT_DHT11 (1 << 2)
#define EVENT_REQUEST (1 << 3)

static TaskHandle_t ILI9341_task;
static TaskHandle_t DS3231_task;
static TaskHandle_t DHT11_task;
static TaskHandle_t ROTARY_task;
static TaskHandle_t REQUEST_task;
static EventGroupHandle_t EVENT_Display;

static rtc_data_t ds3231_0;
static TFT_t tft_0;
static ENCODER_data_t rotary_0;
static BUTTON_Data_t button_0;
dht11_data_t dht_0;

// static uint8_t 			ssid[32] = "Khoa";
// static uint8_t 			pass[32] = "17042021";

static uint8_t ssid[32] = "TP-Link";
static uint8_t pass[32] = "khoaanbk21";

static float temperature = 0, humidity = 0;
static char str[10];
static uint8_t count_pressing_set_time = 0;
char day_rtc[7][4] =
	{
		{'S', 'u', 'n', '\0'},
		{'M', 'o', 'n', '\0'},
		{'T', 'u', 'e', '\0'},
		{'W', 'e', 'd', '\0'},
		{'T', 'h', 'u', '\0'},
		{'F', 'r', 'i', '\0'},
		{'S', 'a', 't', '\0'}};
char month_rtc[12][10] =
	{
		{'J', 'a', 'n', 'u', 'a', 'r', 'y', '\0', '\0', '\0'},
		{'F', 'e', 'b', 'r', 'u', 'a', 'r', 'y', '\0', '\0'},
		{'M', 'a', 'r', 'c', 'h', '\0', '\0', '\0', '\0', '\0'},
		{'A', 'p', 'r', 'i', 'l', '\0', '\0', '\0', '\0', '\0'},
		{'M', 'a', 'y', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
		{'J', 'u', 'n', 'e', '\0', '\0', '\0', '\0', '\0', '\0'},
		{'J', 'u', 'l', 'y', '\0', '\0', '\0', '\0', '\0', '\0'},
		{'A', 'u', 'g', 'u', 's', 't', '\0', '\0', '\0', '\0'},
		{'S', 'e', 'p', 't', 'e', 'm', 'b', 'e', 'r', '\0'},
		{'O', 'c', 't', 'o', 'b', 'e', 'r', '\0', '\0', '\0'},
		{'N', 'o', 'v', 'e', 'm', 'b', 'e', 'r', '\0', '\0'},
		{'D', 'e', 'c', 'e', 'm', 'b', 'e', 'r', '\0', '\0'}};

typedef enum
{
	month,
	day,
} data_change_ds3231_t;

FontxFile fx16G[2];
FontxFile fx24G[2];
FontxFile fx32G[2];
FontxFile fx32L[2];

FontxFile fx16M[2];
FontxFile fx24M[2];
FontxFile fx32M[2];

static void FUNC_callback(uint32_t pin)
{
	if (pin == button_0.pin_button)
	{
		BUTTON_Exit_Handle(&button_0, pin);
	}
}

static void SHORT_pressing(uint32_t pin)
{
	if (pin == button_0.pin_button)
	{
		if (count_pressing_set_time < 7)
		{
			ENCODER_Reset();
			count_pressing_set_time++;
		}
		else
		{
			count_pressing_set_time = 0;
		}
		printf("short\n");
	}
}

static void LONG_pressing(uint32_t pin)
{
	if (pin == button_0.pin_button)
	{
		if (WIFI_init(ssid, pass) == CONNECT_OK)
		{
			vTaskSuspend(DHT11_task);
		}
		else
		{
			vTaskResume(DHT11_task);
			vTaskSuspend(REQUEST_task);
		}
	}
}

static void CHANGE_Value_DS3231(uint8_t value, char *ascii, data_change_ds3231_t type)
{
	uint8_t index = 0;
	if (type == month)
	{
		while (month_rtc[value][index] != '\0')
		{
			ascii[index] = month_rtc[value][index];
			index++;
		}
	}
	else
	{
		while (day_rtc[value][index] != '\0')
		{
			ascii[index] = day_rtc[value][index];
			index++;
		}
	}
}

static void Text_Display(TFT_t *dev, FontxFile *fx, int x, int y,
						 char *ascii, uint16_t color, DIRECTION_Data_t direction)
{
	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);

	lcdSetFontRotation(dev, direction);
	lcdDrawString(dev, fx, x, y, (uint8_t *)ascii, color);
}

void ILI9341(void *arg)
{
	while (1)
	{
		EventBits_t status_wifi = xEventGroupWaitBits(EVENT_Display,
													  EVENT_DISCONNECT_WIFI | EVENT_CONNECT_WIFI, pdFALSE, pdFALSE, portMAX_DELAY);
		if (status_wifi & EVENT_CONNECT_WIFI)
		{
			EventBits_t request_data = xEventGroupWaitBits(EVENT_Display,
														   EVENT_REQUEST, pdTRUE, pdFALSE, portMAX_DELAY);
			if (request_data & EVENT_REQUEST)
			{
				char string_temp[20];
				puts(str);
				if (!strcmp(str, "Rain"))
				{
					ICON_Rain(&tft_0, 150, 70);
				}
				else if (!strcmp(str, "Clouds"))
				{
					ICON_Cloud(&tft_0, 150, 70);
				}
				else
				{
					ICON_Sun(&tft_0, 150, 70, 15);
				}
			}
		}
		if (status_wifi & EVENT_DISCONNECT_WIFI)
		{
			EventBits_t DHT11_data = xEventGroupWaitBits(EVENT_Display,
														 EVENT_DHT11, pdTRUE, pdFALSE, portMAX_DELAY);
			if (DHT11_data & EVENT_DHT11)
			{
				printf("dht_tft\n");
			}
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void ROTARY(void *arg)
{
	while (1)
	{
		uint8_t data_temp = 0;
		switch (count_pressing_set_time)
		{
		case 0:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 23)
			{
				data_temp = 23;
			}
			ds3231_0.hour = data_temp;
			break;

		case 1:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 59)
			{
				data_temp = 59;
			}
			ds3231_0.minute = data_temp;
			break;

		case 2:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 23)
			{
				data_temp = 23;
			}
			ds3231_0.second = data_temp;
			break;

		case 3:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 7)
			{
				data_temp = 7;
			}
			ds3231_0.day = data_temp;
			break;

		case 4:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 31)
			{
				data_temp = 31;
			}
			ds3231_0.date = data_temp;
			break;

		case 5:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 12)
			{
				data_temp = 12;
			}
			ds3231_0.month = data_temp;
			break;

		case 6:
			data_temp = (uint8_t)ENCODER_Read(&rotary_0);
			if (data_temp > 99)
			{
				data_temp = 99;
			}
			ds3231_0.year = data_temp;
			break;
		default:
			DS3231_Register_Write(&ds3231_0);
			vTaskSuspend(DS3231_task);
			break;
		}
	}
}

void DHT11(void *arg)
{
	while (1)
	{
		printf("dht\n");
		DHT11_Get_Data(&dht_0);
		temperature = dht_0.temperature;
		humidity = dht_0.humidity;
		xEventGroupSetBits(EVENT_Display, EVENT_DHT11);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

static void HANDLE_DATA_HTTP_REQUEST(char *json_string)
{
	const cJSON *weather = NULL;
	const cJSON *weathers = NULL;
	cJSON *root = cJSON_Parse(json_string);
	cJSON *obj = cJSON_GetObjectItemCaseSensitive(root, "main");

	temperature = cJSON_GetObjectItemCaseSensitive(obj, "temp")->valuedouble;
	humidity = cJSON_GetObjectItemCaseSensitive(obj, "humidity")->valuedouble;
	temperature = temperature - 273.15;

	weathers = cJSON_GetObjectItemCaseSensitive(root, "weather");
	cJSON_ArrayForEach(weather, weathers)
	{
		cJSON *main = cJSON_GetObjectItemCaseSensitive(weather, "main");
		char *status_weather;
		status_weather = main->valuestring;
		sprintf(str, "%s", status_weather);
	}
	cJSON_Delete(root);
}

void REQUEST_DATA(void *arg)
{
	while (1)
	{
		char *data = NULL;
		data = WIFI_HTTP_Request();
		if (data == NULL)
		{
			printf("disconnect\n");
			xEventGroupClearBits(EVENT_Display, EVENT_CONNECT_WIFI);
			xEventGroupSetBits(EVENT_Display, EVENT_DISCONNECT_WIFI);
			vTaskResume(DHT11_task);
			vTaskSuspend(REQUEST_task);
		}
		else
		{
			vTaskSuspend(DHT11_task);
			xEventGroupClearBits(EVENT_Display, EVENT_DISCONNECT_WIFI);
			xEventGroupSetBits(EVENT_Display, EVENT_CONNECT_WIFI);
			HANDLE_DATA_HTTP_REQUEST(data);
			xEventGroupSetBits(EVENT_Display, EVENT_REQUEST);
			free(data);
		}
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
	// Initialize NVS
	NVS_Init();
	// set font file
	esp_err_t ret;
	ret = mountSPIFFS("/spiffs", "storage0", 10);
	if (ret != ESP_OK)
		return;
	InitFontx(fx16G, "/spiffs/ILGH16XB.FNT", ""); // 8x16Dot Gothic
	InitFontx(fx24G, "/spiffs/ILGH24XB.FNT", ""); // 12x24Dot Gothic
	InitFontx(fx32G, "/spiffs/ILGH32XB.FNT", ""); // 16x32Dot Gothic
	InitFontx(fx32L, "/spiffs/LATIN32B.FNT", ""); // 16x32Dot Latinc

	InitFontx(fx16M, "/spiffs/ILMH16XB.FNT", ""); // 8x16Dot Mincyo
	InitFontx(fx24M, "/spiffs/ILMH24XB.FNT", ""); // 12x24Dot Mincyo
	InitFontx(fx32M, "/spiffs/ILMH32XB.FNT", ""); // 16x32Dot Mincyo

	ENCODER_Init(&rotary_0, ROTARY_PHASE_A, ROTARY_PHASE_B, BUTTON_SETTING, EXTI_EDGE_RISING);
	BUTTON_Init(&button_0, BUTTON_SETTING);
	DHT11_Init(&dht_0, DATA_DHT11);
	SPI_Master_Init(&tft_0, MOSI, SCLK, CS0, TFT_DC, TFT_RESET);
	lcdInit(&tft_0, 240, 320, 0, 0);
	WIFI_Status_t temp_state = WIFI_init(ssid, pass);

	printf("Initialized functions\n");
	EXIT_SetCallBackFunction(FUNC_callback);
	BUTTON_SetCallBackFunction(SHORT_pressing, LONG_pressing, NULL);

	lcdFillScreen(&tft_0, BLACK);
	for (uint8_t i = 0; i <= 5; i++)
	{
		lcdDrawCircle(&tft_0, 180, 230, 53 + i, CYAN);
	}

	for (uint8_t i = 0; i <= 5; i++)
	{
		lcdDrawCircle(&tft_0, 60, 230, 53 + i, CYAN);
	}

	EVENT_Display = xEventGroupCreate();
	xTaskCreate(ROTARY, "ROTARY", 1024 * 2, NULL, 4, &ROTARY_task);
	xTaskCreate(DHT11, "DHT11", 1024 * 2, NULL, 2, &DHT11_task);
	xTaskCreate(REQUEST_DATA, "http_request", 1024 * 3, NULL, 4, &REQUEST_task);
	xTaskCreate(ILI9341, "ILI9341", 1024 * 6, NULL, 1, &ILI9341_task);
	if (temp_state == CONNECT_OK)
	{
		xEventGroupSetBits(EVENT_Display, EVENT_CONNECT_WIFI);
		vTaskSuspend(DHT11_task);
	}
	else
	{
		xEventGroupSetBits(EVENT_Display, EVENT_DISCONNECT_WIFI);
		vTaskSuspend(REQUEST_task);
	}
}