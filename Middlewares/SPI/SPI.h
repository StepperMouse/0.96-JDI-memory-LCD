#ifndef __SPI_H
#define	__SPI_H
#include "stm32f10x.h"
#include "delay.h"
//#############################参数宏定义############################
#define sleep   1           //延迟(微秒)
//写入值
#define T       1
#define F       0
//GPIO引脚选择
#define GPIO_Port       GPIOD
#define SPI_Port        RCC_APB2Periph_GPIOD
//GPIO引脚定义
#define SCLK        GPIO_Pin_0  //SPI时钟线
#define MOSI        GPIO_Pin_1 //主器件数据输出
#define MISO        GPIO_Pin_2 //主器件数据输入
//CS片选线,每一个设备一条片选线
#define CS0         GPIO_Pin_3
#define CS1         GPIO_Pin_4
#define CS2         GPIO_Pin_5
#define CS3         GPIO_Pin_6
//##############################基本函数#############################
void St_SPI_Init(void);
void St_SPI_SCL(uint8_t BitVal);
void St_SPI_CS(uint8_t Select,uint8_t BitVal);
void St_SPI_MOSI(uint8_t BitVal);
uint8_t SPI_MISO(void);
//*******************************************************************
//调用此函数时请务必把 CS CSValue值加入头文件中,方便开发
void St_SPI_Writ_Byte(uint8_t CS,uint8_t CSValue, uint8_t Data);
void St_SPI_Writ_DoubleByte(uint8_t CS,uint8_t CSValue, uint16_t Data);
uint8_t St_SPI_Read_Byte(uint8_t CS,uint8_t CSValue);



#endif
