#ifndef ICON_H_
#define ICON_H_

#include <stdio.h>
#include "tft.h"

void ICON_Sun(TFT_t *tft_x, uint8_t x, uint8_t y, uint8_t size);
void ICON_Rain(TFT_t *tft_x, uint8_t x, uint8_t y);
void ICON_Cloud(TFT_t *tft_x, uint8_t x, uint8_t y);

#endif /* ICON_H_*/