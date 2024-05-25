#ifndef __SPI_H
#define	__SPI_H
#include "stm32f10x.h"
#include "delay.h"
//#############################�����궨��############################
#define sleep   1           //�ӳ�(΢��)
//д��ֵ
#define T       1
#define F       0
//GPIO����ѡ��
#define GPIO_Port       GPIOD
#define SPI_Port        RCC_APB2Periph_GPIOD
//GPIO���Ŷ���
#define SCLK        GPIO_Pin_0  //SPIʱ����
#define MOSI        GPIO_Pin_1 //�������������
#define MISO        GPIO_Pin_2 //��������������
//CSƬѡ��,ÿһ���豸һ��Ƭѡ��
#define CS0         GPIO_Pin_3
#define CS1         GPIO_Pin_4
#define CS2         GPIO_Pin_5
#define CS3         GPIO_Pin_6
//##############################��������#############################
void St_SPI_Init(void);
void St_SPI_SCL(uint8_t BitVal);
void St_SPI_CS(uint8_t Select,uint8_t BitVal);
void St_SPI_MOSI(uint8_t BitVal);
uint8_t SPI_MISO(void);
//*******************************************************************
//���ô˺���ʱ����ذ� CS CSValueֵ����ͷ�ļ���,���㿪��
void St_SPI_Writ_Byte(uint8_t CS,uint8_t CSValue, uint8_t Data);
void St_SPI_Writ_DoubleByte(uint8_t CS,uint8_t CSValue, uint16_t Data);
uint8_t St_SPI_Read_Byte(uint8_t CS,uint8_t CSValue);



#endif
