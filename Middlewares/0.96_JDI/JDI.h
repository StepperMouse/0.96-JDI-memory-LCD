#ifndef __JDI_H
#define __JDI_H

#include "SPI.h"
//SPI pin
#define CS  0
#define CSValue 1


#define DISPLAY_WIDTH   72         // Display width in pixel
#define DISPLAY_HEIGHT  144        // Display height in pixel

#define DIFF_LINE_UPDATE
#define HALF_WIDTH (DISPLAY_WIDTH / 2)

#define COLOR_BLACK             0x00 //黑
#define COLOR_BLUE              0x02 //蓝
#define COLOR_GREEN             0x04 //绿
#define COLOR_CYAN              0x06 //青
#define COLOR_RED               0x08 //红
#define COLOR_MAGENTA           0x0a //品红
#define COLOR_YELLOW            0x0c //黄
#define COLOR_WHITE             0x0e //白

#define CMD_NO_UPDATE           0x00 //
#define CMD_BLINKING_BLACK      0x10 //
#define CMD_BLINKING_INVERSION  0x14 // 
#define CMD_BLINKING_WHITE      0x18 //
#define CMD_ALL_CLEAR           0x20 //
#define CMD_VCOM                0x40 //
#define CMD_UPDATE              0x90 //


void JDI_LCD_Init(uint8_t nullchar);
void JDI_LCD_WriteData(uint8_t Mode, uint16_t Data);

//模块化函数
void JDI_Start(void);
void JDILink_6Bit(uint8_t Mode);
void JDILink_10Bit(uint16_t Data);
void JDILink_Line_Refresh(uint16_t Line); //行刷新
void JDILink_Partial_Refresh(uint8_t Mode, uint16_t SLine, uint16_t ELine); //LCD局部刷新
void JDI_Stop(void);
//void JDI_Write_RAM(uint8_t x, uint8_t y, uint8_t Data);

void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data);
void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String);
void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol);

#endif
