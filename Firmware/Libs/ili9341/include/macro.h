#ifndef MACRO_H_
#define MACRO_H_

// COLORS

#define RED                         0xf800
#define GREEN                       0x07e0
#define BLUE                        0x001f
#define BLACK                       0x0000
#define WHITE                       0xffff
#define GRAY                        0x8c51
#define YELLOW                      0xFFE0
#define CYAN                        0x07FF
#define PURPLE                      0xF81F

// COMMAND

#define POWER_CONTROL_1             0xC0
#define POWER_CONTROL_2             0xC1
#define VCOM_CONTROL_1              0xC5
#define VCOM_CONTROL_2              0xC7
#define MEMORY_ACCESS_CONTROL       0x36
#define PIXEL_FORMAT_SET            0x3A
#define DATA_INVERSION_OFF          0x20
#define FRAME_RATE_CONTROL          0xB1            // In Normal mode
#define DISPLAY_FUNCTION_CONTROL    0xB6
#define GAMMA_SET                   0x26
#define POSITIVE_GAMMA_CORRECTION   0xE0
#define NEGATIVE_GAMMA_CORRECTION   0xE1
#define SLEEP_OUT                   0x11
#define DISPLAY_ON                  0x29
#define DISPLAY_OFF                 0x28
#define INVERSION_ON                0x21
#define INVERSION_OFF               0x20

// DATA

#define PARAMETER_PW1                       0x23            // Power control 1 -> 4.6V
#define PARAMETER_PW2                       0x10            

#define PARAMETER_VCOM1_1st                 0x3E            // VCOMH -> 4.25
#define PARAMETER_VCOM1_2nd                 0x28            // VCOML -> -1.5

#define PARAMETER_VCOM2                     0x86            // VCOML = VML + 6; VCOMH = VMH + 6; 

#define PARAMETER_MEMORY_ACCESS_CONTROL_0   0x00            // RGB color filter panel
#define PARAMETER_MEMORY_ACCESS_CONTROL_1   0x08            // BGR color filter panel

#define PARAMETER_PIXEL_FORMAT_SET          0X55            // 65K color: 16-bit/pixel

#define PARAMETER_FRAME_RATE_1st            0x00            // Division Ratio NONE
#define PARAMETER_FRAME_RATE_2nd            0x18            // Frame Rate = 79Hz

#define PARAMETER_DISPLAY_FUNCTION_1st      0x08            // 
#define PARAMETER_DISPLAY_FUNCTION_2nd      0xA2            // REV:1 GS:0 SS:0 SM:0
#define PARAMETER_DISPLAY_FUNCTION_3rd      0x27            // 320 Lines
#define PARAMETER_DISPLAY_FUNCTION_4th      0x00            // NONE

#define PARAMETER_GAMMA_SET                 0X01            // DEFAULT

#define PARAMETER_POS_GAMMA_CORRECTION_1st  0x0F
#define PARAMETER_POS_GAMMA_CORRECTION_2nd  0x31
#define PARAMETER_POS_GAMMA_CORRECTION_3rd  0x2B
#define PARAMETER_POS_GAMMA_CORRECTION_4th  0x0C
#define PARAMETER_POS_GAMMA_CORRECTION_5th  0x0E
#define PARAMETER_POS_GAMMA_CORRECTION_6th  0x08
#define PARAMETER_POS_GAMMA_CORRECTION_7th  0x4E
#define PARAMETER_POS_GAMMA_CORRECTION_8th  0xF1
#define PARAMETER_POS_GAMMA_CORRECTION_9th  0x37
#define PARAMETER_POS_GAMMA_CORRECTION_10th 0x07
#define PARAMETER_POS_GAMMA_CORRECTION_11th 0x10
#define PARAMETER_POS_GAMMA_CORRECTION_12th 0x03
#define PARAMETER_POS_GAMMA_CORRECTION_13th 0x0E
#define PARAMETER_POS_GAMMA_CORRECTION_14th 0x09
#define PARAMETER_POS_GAMMA_CORRECTION_15th 0x00

#define PARAMETER_NEG_GAMMA_CORRECTION_1st  0x00
#define PARAMETER_NEG_GAMMA_CORRECTION_2nd  0x0E
#define PARAMETER_NEG_GAMMA_CORRECTION_3rd  0x14
#define PARAMETER_NEG_GAMMA_CORRECTION_4th  0x03
#define PARAMETER_NEG_GAMMA_CORRECTION_5th  0x11
#define PARAMETER_NEG_GAMMA_CORRECTION_6th  0x07
#define PARAMETER_NEG_GAMMA_CORRECTION_7th  0x31
#define PARAMETER_NEG_GAMMA_CORRECTION_8th  0xC1
#define PARAMETER_NEG_GAMMA_CORRECTION_9th  0x48
#define PARAMETER_NEG_GAMMA_CORRECTION_10th 0x08
#define PARAMETER_NEG_GAMMA_CORRECTION_11th 0x0F
#define PARAMETER_NEG_GAMMA_CORRECTION_12th 0x0C
#define PARAMETER_NEG_GAMMA_CORRECTION_13th 0x31
#define PARAMETER_NEG_GAMMA_CORRECTION_14th 0x36
#define PARAMETER_NEG_GAMMA_CORRECTION_15th 0x0F

#define COLUMN_ADDR_SET                     0x2A        // Coordinate x
#define PAGE_ADDR_SET                       0x2B        // Coordinate y
#define MEMORY_WRITE                        0x2C

#endif /* MACRO_H_ */