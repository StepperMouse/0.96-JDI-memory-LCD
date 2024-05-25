#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
//#include "OLED_I2C.h"
//#include "EXTI_NVIC.h"
//#include "GPIO.h"
//#include "Menu.h"
//#include "St_I2c.h"
//#include "lcd.h"
//#include "lcd_init.h"
//#include "MPU_6050.h"
//#include "LCD_1617.h"
#include "SPI.h"
#include "JDI.h"
#include "Timer.h"


//变量
int Num = 0;

extern const unsigned char DZAHZ[];

/*函数声明*/
void DigitalClock(uint8_t x0,uint16_t y0);
void JDI_Display(void);


//*************************************************************中断*****************************************************************
void TIM2_IRQHandler(void){ //TIM2
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        DigitalClock(7,2);
        
    }
}
#endif
