#ifndef NEO_PIXEL_H_
#define NEO_PIXEL_H_

#include <stdint.h>

void NEO_PIXEL_Init(uint8_t num_led);
void ALL_Black_Render(void);
void ONE_Color_Render(uint8_t blue, uint8_t red, uint8_t green);
void RENDER_Raising_Mode(uint8_t blue, uint8_t red, uint8_t green, uint64_t ms_delay);
void RENDER_Falling_Mode(uint8_t blue, uint8_t red, uint8_t green, uint64_t ms_delay);
void RENDER_Rainbow_Mode(uint64_t ms_delay);

#endif /* NEO_PIXEL_H_ */