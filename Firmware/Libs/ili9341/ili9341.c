#include "tft.h"
#include "macro.h"

#include <string.h>

#include <driver/gpio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ili9341";

void lcdWriteRegisterByte(TFT_t *tft_x, uint8_t addr, uint16_t data)
{
	SPI_Master_write_command_byte(tft_x, addr);
	SPI_Master_write_data_word(tft_x, data);
}

void lcdWriteRegisterWord(TFT_t *tft_x, uint16_t addr, uint16_t data)
{
	SPI_Master_write_command_word(tft_x, addr);
	SPI_Master_write_data_word(tft_x, data);
}

esp_err_t lcdInit(TFT_t *tft_x, int width, int height, int offsetx, int offsety)
{
	tft_x->_width = width;
	tft_x->_height = height;
	tft_x->_offsetx = offsetx;
	tft_x->_offsety = offsety;
    tft_x->_font_Rotation = DIRECTION_0;
    tft_x->_font_background = false;

    ESP_LOGI(TAG,"Screen width: %d",width);
    ESP_LOGI(TAG,"Screen height: %d",height);
    SPI_Master_write_command_byte(tft_x, POWER_CONTROL_1);	//Power Control 1
    SPI_Master_write_data_byte(tft_x, PARAMETER_PW1);

    SPI_Master_write_command_byte(tft_x, POWER_CONTROL_2);	//Power Control 2
    SPI_Master_write_data_byte(tft_x, PARAMETER_PW2);

    SPI_Master_write_command_byte(tft_x, VCOM_CONTROL_1);	//VCOM Control 1
    SPI_Master_write_data_byte(tft_x, PARAMETER_VCOM1_1st);
    SPI_Master_write_data_byte(tft_x, PARAMETER_VCOM1_2nd);

    SPI_Master_write_command_byte(tft_x, VCOM_CONTROL_2);	//VCOM Control 2
    SPI_Master_write_data_byte(tft_x, PARAMETER_VCOM2);

    SPI_Master_write_command_byte(tft_x, MEMORY_ACCESS_CONTROL);	//Memory Access Control
    SPI_Master_write_data_byte(tft_x, PARAMETER_MEMORY_ACCESS_CONTROL_1);	//Right top start, BGR color filter panel
    //SPI_Master_write_data_byte(tft_x, PARAMETER_MEMORY_ACCESS_CONTROL_0);//Right top start, RGB color filter panel

    SPI_Master_write_command_byte(tft_x, PIXEL_FORMAT_SET);	//Pixel Format Set
    SPI_Master_write_data_byte(tft_x, PARAMETER_PIXEL_FORMAT_SET);	//65K color: 16-bit/pixel

    SPI_Master_write_command_byte(tft_x, DATA_INVERSION_OFF);	//Display Inversion OFF

    SPI_Master_write_command_byte(tft_x, FRAME_RATE_CONTROL);	//Frame Rate Control
    SPI_Master_write_data_byte(tft_x, PARAMETER_FRAME_RATE_1st);
    SPI_Master_write_data_byte(tft_x, PARAMETER_FRAME_RATE_2nd);

    SPI_Master_write_command_byte(tft_x, DISPLAY_FUNCTION_CONTROL);	//Display Function Control
    SPI_Master_write_data_byte(tft_x, PARAMETER_DISPLAY_FUNCTION_1st);
    SPI_Master_write_data_byte(tft_x, PARAMETER_DISPLAY_FUNCTION_2nd);	// REV:1 GS:0 SS:0 SM:0
    SPI_Master_write_data_byte(tft_x, PARAMETER_DISPLAY_FUNCTION_3rd);
    SPI_Master_write_data_byte(tft_x, PARAMETER_DISPLAY_FUNCTION_4th);

    SPI_Master_write_command_byte(tft_x, GAMMA_SET);	//Gamma Set
    SPI_Master_write_data_byte(tft_x, PARAMETER_GAMMA_SET);

    SPI_Master_write_command_byte(tft_x, POSITIVE_GAMMA_CORRECTION);	//Positive Gamma Correction
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_1st);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_2nd);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_3rd);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_4th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_5th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_6th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_7th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_8th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_9th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_10th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_11th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_12th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_13th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_14th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_POS_GAMMA_CORRECTION_15th);

    SPI_Master_write_command_byte(tft_x, NEGATIVE_GAMMA_CORRECTION);	//Negative Gamma Correction
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_1st);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_2nd);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_3rd);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_4th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_5th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_6th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_7th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_8th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_9th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_10th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_11th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_12th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_13th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_14th);
    SPI_Master_write_data_byte(tft_x, PARAMETER_NEG_GAMMA_CORRECTION_15th);

    SPI_Master_write_command_byte(tft_x, SLEEP_OUT);	//Sleep Out
    vTaskDelay(120 / portTICK_PERIOD_MS);

    SPI_Master_write_command_byte(tft_x, DISPLAY_ON);	//Display ON
	return ESP_OK;
}

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color

void lcdDrawPixel(TFT_t *tft_x, uint16_t x, uint16_t y, uint16_t color)
{
	if (x >= tft_x->_width) return;
	if (y >= tft_x->_height) return;

	uint16_t _x = x + tft_x->_offsetx;
	uint16_t _y = y + tft_x->_offsety;

    SPI_Master_write_command_byte(tft_x, COLUMN_ADDR_SET);	// set column(x) address
    SPI_Master_write_addr(tft_x, _x, _x);
    SPI_Master_write_command_byte(tft_x, PAGE_ADDR_SET);	// set Page(y) address
    SPI_Master_write_addr(tft_x, _y, _y);
    SPI_Master_write_command_byte(tft_x, MEMORY_WRITE);	// Memory Write
    SPI_Master_write_data_word(tft_x, color);
}

void lcdDrawMultiPixels(TFT_t *tft_x, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors)
{
	if (x+size > tft_x->_width) return;
	if (y >= tft_x->_height) return;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",tft_x->_offsetx,tft_x->_offsety);
	uint16_t _x1 = x + tft_x->_offsetx;
	uint16_t _x2 = _x1 + (size-1);
	uint16_t _y1 = y + tft_x->_offsety;
	uint16_t _y2 = _y1;
	ESP_LOGD(TAG,"_x1=%d _x2=%d _y1=%d _y2=%d",_x1, _x2, _y1, _y2);

    SPI_Master_write_command_byte(tft_x, COLUMN_ADDR_SET);	// set column(x) address
    SPI_Master_write_addr(tft_x, _x1, _x2);
    SPI_Master_write_command_byte(tft_x, PAGE_ADDR_SET);	// set Page(y) address
    SPI_Master_write_addr(tft_x, _y1, _y2);
    SPI_Master_write_command_byte(tft_x, MEMORY_WRITE);	// Memory Write
    SPI_Master_write_colors(tft_x, colors, size);
}

// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(TFT_t *tft_x, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	if (x1 >= tft_x->_width) return;
	if (x2 >= tft_x->_width) x2=tft_x->_width - 1;
	if (y1 >= tft_x->_height) return;
	if (y2 >= tft_x->_height) y2=tft_x->_height - 1;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",tft_x->_offsetx,tft_x->_offsety);
	uint16_t _x1 = x1 + tft_x->_offsetx;
	uint16_t _x2 = x2 + tft_x->_offsetx;
	uint16_t _y1 = y1 + tft_x->_offsety;
	uint16_t _y2 = y2 + tft_x->_offsety;

    SPI_Master_write_command_byte(tft_x, 0x2A);	// set column(x) address
    SPI_Master_write_addr(tft_x, _x1, _x2);
    SPI_Master_write_command_byte(tft_x, 0x2B);	// set Page(y) address
    SPI_Master_write_addr(tft_x, _y1, _y2);
    SPI_Master_write_command_byte(tft_x, 0x2C);	// Memory Write
    for(int i = _x1; i <= _x2; i++) 
    {
        uint16_t size = _y2 -_y1 + 1;
        SPI_Master_write_color(tft_x, color, size);
    }
}

// x0:Center X coordinate
// y0:Center Y coordinate
// size:Square size
// color:color
void lcdDrawFillRect2(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t size, uint16_t color)
{
	uint16_t x1 = x0-size;
	uint16_t y1 = y0-size;
	uint16_t x2 = x0+size;
	uint16_t y2 = y0+size;
	lcdDrawFillRect(tft_x, x1, y1, x2, y2, color);
}

inline void lcdDisplayOff(TFT_t *tft_x)
{
    SPI_Master_write_command_byte(tft_x, DISPLAY_OFF);
}

inline void lcdDisplayOn(TFT_t *tft_x)
{
    SPI_Master_write_command_byte(tft_x, DISPLAY_ON);
}

inline void lcdInversionOff(TFT_t *tft_x)
{
    SPI_Master_write_command_byte(tft_x, INVERSION_OFF);
}

inline void lcdInversionOn(TFT_t *tft_x)
{
    SPI_Master_write_command_byte(tft_x, INVERSION_ON);
}

inline void lcdRGBFilter(TFT_t *tft_x)
{
    SPI_Master_write_command_byte(tft_x, MEMORY_ACCESS_CONTROL);	//Memory Access Control
    SPI_Master_write_data_byte(tft_x, PARAMETER_MEMORY_ACCESS_CONTROL_0);	//Right top start, RGB color filter panel
}

inline void lcdFillScreen(TFT_t *tft_x, uint16_t color)
{
    lcdDrawFillRect(tft_x, 0, 0, tft_x->_width-1, tft_x->_height-1, color);
}

void lcdDrawLine(TFT_t *tft_x, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t distance_x, distance_y, step_x, step_y, error; 

    distance_x = (x1 >= x2) ? x1 - x2 : x2 - x1;
    distance_y = (y1 >= y2) ? y1 - y2 : y2 - y1;

    step_x = (x1 > x2) ? -1 : 1;
    step_y = (y1 > y2) ? -1 : 1;
    
    if (distance_x > distance_y)
    {
        error = -distance_x;
        for (uint16_t i = 0; i <= distance_x; i++)
        {
            lcdDrawPixel(tft_x, x1, y1, color);
            x1 += step_x;
            error += 2 * distance_y;
            if (error >= 0)
            {
                y1 += step_y;
                error -= 2 * distance_x;
            }
            
        }
        
    }
    else
    {
        error = -distance_y;
        for (uint16_t i = 0; i <= distance_y; i++)
        {
            lcdDrawPixel(tft_x, x1, y1, color);
            y1 += step_y;
            error += 2 * distance_x;
            if (error >= 0)
            {
                x1 += step_x;
                error -= 2 * distance_y;
            }
            
        }
    }
    tft_x->_font_background_color = color;
}

void lcdDrawSquare(TFT_t *tft_x, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	lcdDrawLine(tft_x, x1, y1, x2, y1, color);
	lcdDrawLine(tft_x, x2, y1, x2, y2, color);
	lcdDrawLine(tft_x, x2, y2, x1, y2, color);
	lcdDrawLine(tft_x, x1, y2, x1, y1, color);
}

void lcdDrawSquare2(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t size, uint16_t color)
{
	lcdDrawLine(tft_x, x0, y0, x0 + size, y0, color);
	lcdDrawLine(tft_x, x0 + size, y0, x0 + size, y0 + size, color);
	lcdDrawLine(tft_x, x0 + size, y0 + size, x0, y0 + size, color);
	lcdDrawLine(tft_x, x0, y0 + size, x0, y0, color);
}

static void lcdDrawCircle_raw(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t color)
{
    lcdDrawPixel(tft_x, x + x0, y + y0, color); 
    lcdDrawPixel(tft_x, x + x0, -y + y0, color); 
    lcdDrawPixel(tft_x, -x + x0, -y + y0, color); 
    lcdDrawPixel(tft_x, -x + x0, y + y0, color);

    lcdDrawPixel(tft_x, y + x0, x + y0, color);
    lcdDrawPixel(tft_x, y + x0, -x + y0, color);
    lcdDrawPixel(tft_x, -y + x0, -x + y0, color);
    lcdDrawPixel(tft_x, -y + x0, x + y0, color);
}

// Function for circle-generation
// using Bresenham's algorithm
void lcdDrawCircle(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t x, y, error;
    
    x = 0;
    y = r;
    error = 3 - (2 * r);
    lcdDrawCircle_raw(tft_x, x0, y0, x, y, color);
    while (y > x)
    {       
        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (error > 0)
        {
            y--;
            error = error + 4 * (x - y) + 10;
        }
        else
        {
            error = error + 4 * x + 6;
        }
        x++;
        lcdDrawCircle_raw(tft_x, x0, y0, x, y, color);

    }
    
	// int x;
	// int y;
	// int err;
	// int old_err;

	// x=0;
	// y=-r;
	// err=2-2*r;
	// do{
	// 	lcdDrawPixel(tft_x, x0-x, y0+y, color); 
	// 	lcdDrawPixel(tft_x, x0-y, y0-x, color); 
	// 	lcdDrawPixel(tft_x, x0+x, y0-y, color); 
	// 	lcdDrawPixel(tft_x, x0+y, y0+x, color); 
	// 	if ((old_err=err)<=x) err+=++x*2+1;
	// 	if (old_err>y || err>x) err+=++y*2+1;
	// } while(y<0);
}

// static void lcdDrawCircleFill_raw(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t x, uint16_t y, uint16_t color)
// {
//     lcdDrawLine(tft_x, x + x0, y + y0, x0, y0, color); 
//     lcdDrawLine(tft_x, x + x0, -y + y0, x0, y0, color); 
//     lcdDrawLine(tft_x, -x + x0, -y + y0, x0, y0, color); 
//     lcdDrawLine(tft_x, -x + x0, y + y0, x0, y0, color);

//     lcdDrawLine(tft_x, y + x0, x + y0, x0, y0, color);
//     lcdDrawLine(tft_x, y + x0, -x + y0, x0, y0, color);
//     lcdDrawLine(tft_x, -y + x0, -x + y0, x0, y0, color);
//     lcdDrawLine(tft_x, -y + x0, x + y0, x0, y0, color);
// }

void lcdDrawFillCircle(TFT_t *tft_x, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    // int16_t x, y, error;
    
    // x = 0;
    // y = r;
    // error = 3 - (2 * r);
    // lcdDrawCircleFill_raw(tft_x, x0, y0, x, y, color);
    // while (y > x)
    // {       
    //     // check for decision parameter
    //     // and correspondingly
    //     // update d, x, y
    //     if (error > 0)
    //     {
    //         y--;
    //         error = error + 4 * (x - y) + 10;
    //     }
    //     else
    //     {
    //         error = error + 4 * x + 6;
    //     }
    //     x++;
    //     lcdDrawCircleFill_raw(tft_x, x0, y0, x, y, color);

    // }

	int16_t x, y, err, old_err, ChangeX;

	x = 0;
	y = -r;
	err = 2 - 2 * r;
	ChangeX = 1;
	do{
		if(ChangeX) 
        {
			lcdDrawLine(tft_x, x0-x, y0-y, x0-x, y0+y, color);
			lcdDrawLine(tft_x, x0+x, y0-y, x0+x, y0+y, color);
		} // endif
		ChangeX = (old_err=err) <= x;
		if (ChangeX) err += ++x * 2 + 1;
		if (old_err > y || err > x) err += ++y * 2 +  1;
	} while(y <= 0);
}

void lcdBacklightOff(TFT_t *tft_x);
void lcdBacklightOn(TFT_t *tft_x);

inline void lcdSetFontRotation(TFT_t * tft_x, uint16_t direction)
{
    tft_x->_font_Rotation = direction;
}

inline void lcdSetFontBackground(TFT_t * tft_x, uint16_t color)
{
    tft_x->_font_background = true;
    tft_x->_font_background_color = color;
}

inline void lcdUnsetFontBackground(TFT_t * tft_x)
{
    tft_x->_font_background = false;
}

uint8_t lcdDrawChar(TFT_t * tft_x, FontxFile *fxs, uint16_t x, uint16_t y, uint8_t ascii, uint16_t color) {
	uint16_t xx,yy,bit,ofs;
	unsigned char fonts[128]; // font pattern
	unsigned char pw, ph;
	int h,w;
	uint16_t mask;
	bool rc;
	rc = GetFontx(fxs, ascii, fonts, &pw, &ph);
	if (!rc) return 0;

	int16_t xd1 = 0;
	int16_t yd1 = 0;
	int16_t xd2 = 0;
	int16_t yd2 = 0;
	int16_t xss = 0;
	int16_t yss = 0;
	int16_t xsd = 0;
	int16_t ysd = 0;
	int16_t next = 0;
	int16_t x0	= 0;
	int16_t x1	= 0;
	int16_t y0	= 0;
	int16_t y1	= 0;
	if (tft_x->_font_Rotation == 0) 
    {
		xd1 = +1;
		yd1 = +1; //-1;
		xd2 =  0;
		yd2 =  0;
		xss =  x;
		yss =  y - (ph - 1);
		xsd =  1;
		ysd =  0;
		next = x + pw;

		x0	= x;
		y0	= y - (ph-1);
		x1	= x + (pw-1);
		y1	= y;
	} 
    else if (tft_x->_font_Rotation == 2) 
    {
		xd1 = -1;
		yd1 = -1; //+1;
		xd2 =  0;
		yd2 =  0;
		xss =  x;
		yss =  y + ph + 1;
		xsd =  1;
		ysd =  0;
		next = x - pw;

		x0	= x - (pw-1);
		y0	= y;
		x1	= x;
		y1	= y + (ph-1);
	} 
    else if (tft_x->_font_Rotation == 1) 
    {
		xd1 =  0;
		yd1 =  0;
		xd2 = -1;
		yd2 = +1; //-1;
		xss =  x + ph;
		yss =  y;
		xsd =  0;
		ysd =  1;
		next = y + pw; //y - pw;

		x0	= x;
		y0	= y;
		x1	= x + (ph-1);
		y1	= y + (pw-1);
	} 
    else if (tft_x->_font_Rotation == 3) 
    {
		xd1 =  0;
		yd1 =  0;
		xd2 = +1;
		yd2 = -1; //+1;
		xss =  x - (ph - 1);
		yss =  y;
		xsd =  0;
		ysd =  1;
		next = y - pw; //y + pw;

		x0	= x - (ph-1);
		y0	= y - (pw-1);
		x1	= x;
		y1	= y;
	}

	if (tft_x->_font_background) 
    {
        lcdDrawFillRect(tft_x, x0, y0, x1, y1, tft_x->_font_background_color);
    }

	int bits;
	ofs = 0;
	yy = yss;
	xx = xss;
	for(h = 0; h < ph; h++)
	{
		if(xsd) xx = xss;
		if(ysd) yy = yss;
		//for(w=0;w<(pw/8);w++) {
		bits = pw;
		for(w = 0; w < ((pw + 4) / 8); w++) 
		{
			mask = 0x80;
			for(bit = 0; bit < 8; bit++)
			{
				bits--;
				if (bits < 0) continue;
				//if(_DEBUG_)printf("xx=%d yy=%d mask=%02x fonts[%d]=%02x\n",xx,yy,mask,ofs,fonts[ofs]);
				if (fonts[ofs] & mask) 
				{
					lcdDrawPixel(tft_x, xx, yy, color);
				} 
				else 
				{
					//if (tft_x->_font_fill) lcdDrawPixel(tft_x, xx, yy, tft_x->_font_fill_color);
				}
				xx = xx + xd1;
				yy = yy + yd2;
				mask = mask >> 1;
			}
			ofs++;
		}
		yy = yy + yd1;
		xx = xx + xd2;
	}

	if (next < 0) next = 0;
	return next;
}

uint8_t lcdDrawString(TFT_t * tft_x, FontxFile *fx, uint16_t x, uint16_t y, uint8_t * ascii, uint16_t color) {
	int length = strlen((char *)ascii);
	for(int i=0;i<length;i++) {
		if (tft_x->_font_Rotation == 0)
			x = lcdDrawChar(tft_x, fx, x, y, ascii[i], color);
		if (tft_x->_font_Rotation == 1)
			y = lcdDrawChar(tft_x, fx, x, y, ascii[i], color);
		if (tft_x->_font_Rotation == 2)
			x = lcdDrawChar(tft_x, fx, x, y, ascii[i], color);
		if (tft_x->_font_Rotation == 3)
			y = lcdDrawChar(tft_x, fx, x, y, ascii[i], color);
	}
	if (tft_x->_font_Rotation == 0) return x;
	if (tft_x->_font_Rotation == 2) return x;
	if (tft_x->_font_Rotation == 1) return y;
	if (tft_x->_font_Rotation == 3) return y;
	return 0;
}