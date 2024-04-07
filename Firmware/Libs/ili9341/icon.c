#include "icon.h"
#include "macro.h"
#include "math.h"

void ICON_Sun(TFT_t *tft_x, uint8_t x, uint8_t y, uint8_t size)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		lcdDrawCircle(tft_x, x, y, size + i, YELLOW);
	}

    lcdDrawLine(tft_x, x + size + 5, y, x + size + 13, y, YELLOW);
    lcdDrawLine(tft_x, x + size + 5, y - 1, x + size + 13, y - 1, YELLOW);
    lcdDrawLine(tft_x, x + size + 5, y + 1, x + size + 13, y + 1, YELLOW);

    lcdDrawLine(tft_x, x - size - 5, y, x - size - 13, y, YELLOW);
    lcdDrawLine(tft_x, x - size - 5, y - 1, x - size - 13, y - 1, YELLOW);
    lcdDrawLine(tft_x, x - size - 5, y + 1, x - size - 13, y + 1, YELLOW);

    lcdDrawLine(tft_x, x, y + size + 5, x, y + size + 13, YELLOW);
    lcdDrawLine(tft_x, x + 1, y + size + 5, x + 1, y + size + 13, YELLOW);
    lcdDrawLine(tft_x, x - 1, y + size + 5, x - 1, y + size + 13, YELLOW);

    lcdDrawLine(tft_x, x, y - size - 5, x, y - size - 13, YELLOW);
    lcdDrawLine(tft_x, x + 1, y - size - 5, x + 1, y - size - 13, YELLOW);
    lcdDrawLine(tft_x, x - 1, y - size - 5, x - 1, y - size - 13, YELLOW);

    lcdDrawLine(tft_x, x + (size + 5) / sqrt(2), y + (size + 5) / sqrt(2), 
                x + (size + 13) / sqrt(2), y + (size + 13) / sqrt(2), YELLOW);
    lcdDrawLine(tft_x, x + (size + 5) / sqrt(2), y + (size + 5) / sqrt(2) + 1, 
                x + (size + 13) / sqrt(2), y + (size + 13) / sqrt(2) + 1, YELLOW);
    lcdDrawLine(tft_x, x + (size + 5) / sqrt(2), y + (size + 5) / sqrt(2) - 1, 
                x + (size + 13) / sqrt(2), y + (size + 13) / sqrt(2) - 1, YELLOW);

    lcdDrawLine(tft_x, x - (size + 5) / sqrt(2), y - (size + 5) / sqrt(2), 
                x - (size + 13) / sqrt(2), y - (size + 13) / sqrt(2), YELLOW);
    lcdDrawLine(tft_x, x - (size + 5) / sqrt(2), y - (size + 5) / sqrt(2) + 1, 
                x - (size + 13) / sqrt(2), y - (size + 13) / sqrt(2) + 1, YELLOW);
    lcdDrawLine(tft_x, x - (size + 5) / sqrt(2), y - (size + 5) / sqrt(2) - 1, 
                x - (size + 13) / sqrt(2), y - (size + 13) / sqrt(2) - 1, YELLOW);

    lcdDrawLine(tft_x, x + (size + 5) / sqrt(2), y - (size + 5) / sqrt(2), 
                x + (size + 13) / sqrt(2), y - (size + 13) / sqrt(2), YELLOW);
    lcdDrawLine(tft_x, x + (size + 5) / sqrt(2), y - (size + 5) / sqrt(2) + 1, 
                x + (size + 13) / sqrt(2), y - (size + 13) / sqrt(2) + 1, YELLOW);
    lcdDrawLine(tft_x, x + (size + 5) / sqrt(2), y - (size + 5) / sqrt(2) - 1, 
                x + (size + 13) / sqrt(2), y - (size + 13) / sqrt(2) - 1, YELLOW);

    lcdDrawLine(tft_x, x - (size + 5) / sqrt(2), y + (size + 5) / sqrt(2), 
                x - (size + 13) / sqrt(2), y + (size + 13) / sqrt(2), YELLOW);
    lcdDrawLine(tft_x, x - (size + 5) / sqrt(2), y + (size + 5) / sqrt(2) + 1, 
                x - (size + 13) / sqrt(2), y + (size + 13) / sqrt(2) + 1, YELLOW);
    lcdDrawLine(tft_x, x - (size + 5) / sqrt(2), y + (size + 5) / sqrt(2) - 1, 
                x - (size + 13) / sqrt(2), y + (size + 13) / sqrt(2) - 1, YELLOW);
}

void ICON_Rain(TFT_t *tft_x, uint8_t x, uint8_t y)
{
    ICON_Cloud(tft_x, x, y);
    for (int8_t i = -2; i <= 2; i++)
    {
        lcdDrawLine(tft_x, x - 35, y - 1 + 10 * i, x - 50, y - 1 + 10 * i - 3, CYAN);
        lcdDrawLine(tft_x, x - 35, y + 10 * i, x - 50, y + 10 * i - 3, CYAN);
        lcdDrawLine(tft_x, x - 35, y + 1 + 10 * i, x - 50, y + 1 + 10 * i - 3, CYAN);
    }
    
}

void ICON_Cloud(TFT_t *tft_x, uint8_t x, uint8_t y)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		lcdDrawFillCircle(tft_x, x, y + 2, 24 + i, WHITE);
	}
    
	for (uint8_t i = 0; i < 3; i++)
	{
		lcdDrawFillCircle(tft_x, x - 5, y + 25, 12 + i, WHITE);
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		lcdDrawFillCircle(tft_x, x - 7, y - 20, 16 + i, WHITE);
	}

}