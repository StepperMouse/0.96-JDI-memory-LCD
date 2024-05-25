#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
#include "OLED_I2C.h"
#include "EXTI_NVIC.h"
#include "GPIO.h"
//#include "Menu.h"
#include "St_I2c.h"
//#include "lcd.h"
//#include "lcd_init.h"
#include "MPU_6050.h"
#include "LCD_1617.h"

//volatile static uint32_t LED_State;

int Num = 0;
int Encoder_Count;					//编码器数据


extern const unsigned char DZAHZ[];

/*函数声明*/
void DigitalClock(unsigned char x0,unsigned char y0,unsigned char N);
void LCD_SPI(void);
void MCP_3421_Data(void);
void OLED_I2C(void);



#endif

