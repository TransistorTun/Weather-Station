#include "tft.h"

#include <driver/gpio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#define TFT_ID SPI2_HOST

static const int TFT_Frequency = SPI_MASTER_FREQ_40M;
static const int SPI_Command_Mode = 0;
static const int SPI_Data_Mode = 1;

static const char *TAG = "spi_tft";

esp_err_t SPI_Master_Init(TFT_t *tft_x, int16_t TFT_MOSI, int16_t TFT_SCLK, 
        int16_t TFT_CS, int16_t GPIO_DC, int16_t GPIO_RESET)
{
    esp_err_t ret;
	ESP_LOGI(TAG, "TFT_MOSI = %d",TFT_MOSI);
	ESP_LOGI(TAG, "TFT_SCLK = %d",TFT_SCLK);
	ESP_LOGI(TAG, "TFT_CS = %d",TFT_CS);
	gpio_reset_pin( TFT_CS );
	gpio_set_direction( TFT_CS, GPIO_MODE_OUTPUT );
	gpio_set_level( TFT_CS, 1 );

	ESP_LOGI(TAG, "GPIO_DC = %d",GPIO_DC);
	gpio_reset_pin( GPIO_DC );
	gpio_set_direction( GPIO_DC, GPIO_MODE_OUTPUT );
	gpio_set_level( GPIO_DC, 0 );

	ESP_LOGI(TAG, "GPIO_RESET = %d",GPIO_RESET);
	if ( GPIO_RESET >= 0 ) {
		gpio_reset_pin( GPIO_RESET );
		gpio_set_direction( GPIO_RESET, GPIO_MODE_OUTPUT );
		gpio_set_level( GPIO_RESET, 0 );
		vTaskDelay(100 / portTICK_PERIOD_MS);
		gpio_set_level( GPIO_RESET, 1 );
	}

	spi_bus_config_t tft_buscfg = {
		.sclk_io_num = TFT_SCLK,
		.mosi_io_num = TFT_MOSI,
		.miso_io_num = -1,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1
	};

	ret = spi_bus_initialize( TFT_ID, &tft_buscfg, SPI_DMA_CH_AUTO );
	ESP_LOGI(TAG, "spi_bus_initialize(TFT) ret=%d TFT_ID=%d",ret, TFT_ID);
	assert(ret==ESP_OK);

	spi_device_interface_config_t tft_devcfg = {
		.clock_speed_hz = TFT_Frequency,
		.spics_io_num = TFT_CS,
		.queue_size = 8,
		.flags = SPI_DEVICE_NO_DUMMY,
	};

	spi_device_handle_t tft_handle;
	ret = spi_bus_add_device( TFT_ID, &tft_devcfg, &tft_handle);
	ESP_LOGD(TAG, "spi_bus_add_device=%d",ret);
	assert(ret==ESP_OK);
	tft_x->_dc = GPIO_DC;
	tft_x->_TFT_Handle = tft_handle;
	return ESP_OK;
}

bool SPI_Master_write_byte(spi_device_handle_t SPIHandle, const uint8_t* Data, size_t DataLength)
{
	spi_transaction_t SPITransaction;
	esp_err_t ret;

    if (DataLength > 0)
    {
        memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
		SPITransaction.length = DataLength * 8;
		SPITransaction.tx_buffer = Data;
        ret = spi_device_transmit( SPIHandle, &SPITransaction );
        assert(ret==ESP_OK); 
    }
    return true;
}

bool SPI_Master_write_command_byte(TFT_t *tft_x, uint8_t cmd)
{
	static uint8_t Byte = 0;
	Byte = cmd;
	gpio_set_level( tft_x->_dc, SPI_Command_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, &Byte, 1 );
}

bool SPI_Master_write_command_word(TFT_t *tft_x, uint16_t cmd)
{
	static uint8_t Byte[2];
	Byte[0] = (cmd >> 8) & 0xFF;
	Byte[1] = cmd & 0xFF;
	gpio_set_level( tft_x->_dc, SPI_Command_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, Byte, 2 );
}

bool SPI_Master_write_data_byte(TFT_t *tft_x, uint8_t data)
{
	static uint8_t Byte = 0;
	Byte = data;
	gpio_set_level( tft_x->_dc, SPI_Data_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, &Byte, 1 );
}

bool SPI_Master_write_data_word(TFT_t *tft_x, uint16_t data)
{
	static uint8_t Byte[2];
	Byte[0] = (data >> 8) & 0xFF;
	Byte[1] = data & 0xFF;
	gpio_set_level( tft_x->_dc, SPI_Data_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, Byte, 2 );
}

bool SPI_Master_write_addr(TFT_t *tft_x, uint16_t addr1, uint16_t addr2)
{
	static uint8_t Byte[4];
	Byte[0] = (addr1 >> 8) & 0xFF;
	Byte[1] = addr1 & 0xFF;
	Byte[2] = (addr2 >> 8) & 0xFF;
	Byte[3] = addr2 & 0xFF;
	gpio_set_level( tft_x->_dc, SPI_Data_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, Byte, 4);
}

bool SPI_Master_write_color(TFT_t *tft_x, uint16_t color, uint16_t size)
{
	static uint8_t Byte[1024];
	uint16_t index = 0;
	for(uint16_t i = 0; i < size; i++) 
    {
		Byte[index++] = (color >> 8) & 0xFF;
		Byte[index++] = color & 0xFF;
	}
	gpio_set_level( tft_x->_dc, SPI_Data_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, Byte, size*2);
}

bool SPI_Master_write_colors(TFT_t *tft_x, uint16_t * colors, uint16_t size)
{
	static uint8_t Byte[1024];
	uint16_t index = 0;
	for(uint16_t i = 0; i < size; i++) 
    {
		Byte[index++] = (colors[i] >> 8) & 0xFF;
		Byte[index++] = colors[i] & 0xFF;
	}
	gpio_set_level( tft_x->_dc, SPI_Data_Mode );
	return SPI_Master_write_byte( tft_x->_TFT_Handle, Byte, size*2);
}