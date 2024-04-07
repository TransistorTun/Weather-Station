#ifndef TFT_H_
#define TFT_H_

#include <stdint.h>
#include <stdbool.h>
#include "driver/spi_master.h"
#include "font.h"
#include "esp_err.h"

typedef enum
{
	DIRECTION_0 = 0,
	DIRECTION_1,
	DIRECTION_2,
	DIRECTION_3,
}DIRECTION_Data_t;

typedef struct {
	uint8_t _font_Rotation;
	uint8_t _font_background;
	uint16_t _font;
	uint16_t _font_color;
	uint16_t _font_background_color;
	uint16_t _width;
	uint16_t _height;
	uint16_t _offsetx;
	uint16_t _offsety;
	int16_t _dc;
	spi_device_handle_t _TFT_Handle;
} TFT_t;

esp_err_t SPI_Master_Init(TFT_t *tft_x, int16_t TFT_MOSI, int16_t TFT_SCLK, 
        int16_t TFT_CS, int16_t GPIO_DC, int16_t GPIO_RESET);
bool SPI_Master_write_byte(spi_device_handle_t SPIHandle, const uint8_t* Data, size_t DataLength);
bool SPI_Master_write_command_byte(TFT_t *tft_x, uint8_t cmd);
bool SPI_Master_write_command_word(TFT_t *tft_x, uint16_t cmd);
bool SPI_Master_write_data_byte(TFT_t *tft_x, uint8_t data);
bool SPI_Master_write_data_word(TFT_t *tft_x, uint16_t data);
bool SPI_Master_write_addr(TFT_t *tft_x, uint16_t addr1, uint16_t addr2);
bool SPI_Master_write_color(TFT_t *tft_x, uint16_t color, uint16_t size);
bool SPI_Master_write_colors(TFT_t *tft_x, uint16_t * colors, uint16_t size);
void lcdWriteRegisterByte(TFT_t *tft_x, uint8_t addr, uint16_t data);
void lcdWriteRegisterWord(TFT_t *tft_x, uint16_t addr, uint16_t data);
esp_err_t lcdInit(TFT_t *tft_x, int width, int height, int offsetx, int offsety);
void lcdDrawPixel(TFT_t *tft_x, uint16_t x, uint16_t y, uint16_t color);
void lcdDrawMultiPixels(TFT_t *tft_x, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
void lcdDrawFillRect(TFT_t *tft_x, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawFillRect2(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t size, uint16_t color);
void lcdDisplayOff(TFT_t *tft_x);
void lcdDisplayOn(TFT_t *tft_x);
void lcdInversionOff(TFT_t *tft_x);
void lcdInversionOn(TFT_t *tft_x);
void lcdRGBFilter(TFT_t *tft_x);
void lcdFillScreen(TFT_t *tft_x, uint16_t color);
void lcdDrawLine(TFT_t *tft_x, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawSquare(TFT_t *tft_x, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawSquare2(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t size, uint16_t color);
void lcdDrawCircle(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawFillCircle(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdBacklightOff(TFT_t *tft_x);
void lcdBacklightOn(TFT_t *tft_x);
void lcdSetFontRotation(TFT_t * tft_x, uint16_t direction);
void lcdSetFontBackground(TFT_t * tft_x, uint16_t color);
void lcdUnsetFontBackground(TFT_t * tft_x);
uint8_t lcdDrawChar(TFT_t * tft_x, FontxFile *fx, uint16_t x, uint16_t y, uint8_t ascii, uint16_t color);
uint8_t lcdDrawString(TFT_t * tft_x, FontxFile *fx, uint16_t x, uint16_t y, uint8_t * ascii, uint16_t color);

#endif /* TFT_H_ */