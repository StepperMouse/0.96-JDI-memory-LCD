/*
    ����ʱ��        2024.5.15
    ������          DZAHZ
    ����            �Զ���GPIO����ģ��SPIͨѶ   (Software SPI)
*/
#include "SPI.h"
uint8_t CS[]={CS0,CS1,CS2,CS3};

void St_SPI_Init(void){                        //���ų�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(SPI_Port, ENABLE);	 //ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //�ٶ�50MHz
 	GPIO_Init(GPIO_Port, &GPIO_InitStructure);	             //��ʼ��GPIO
 	GPIO_ResetBits(GPIO_Port,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
}

void St_SPI_SCL(uint8_t BitVal){               //ʱ����д
    GPIO_WriteBit(GPIO_Port, SCLK ,(BitAction)BitVal);
    Delay_us(sleep);
}

void St_SPI_CS(uint8_t Select,uint8_t BitVal){ //Ƭѡ
    GPIO_WriteBit(GPIO_Port, CS[Select] ,(BitAction)BitVal);
    Delay_us(sleep);
}

void St_SPI_MOSI(uint8_t BitVal){              //������д
    GPIO_WriteBit(GPIO_Port, MOSI ,(BitAction)BitVal);
    Delay_us(sleep);
}

uint8_t St_SPI_MISO(void){                     //�����߶�
    uint8_t Bitval;
    Bitval = GPIO_ReadInputDataBit(GPIO_Port, MISO);
    Delay_us(sleep);
    return Bitval;
}




//ÿ�κ���������SCLK����Ϊ�͵�ƽ

/**         2024.5.16        By DZAHZ
  * @Prototype   St_SPI_Writ_Byte
  * @Description SPI�ֽ�д��
  * @param       CS        Ƭѡ�� ��Χ1~12
  * @param       CSValue   Ƭѡ��Чֵ,һ��͵�ƽ��Ч
  * @param       Data      Ҫд�������
  * @retval      ��
  */
void St_SPI_Writ_Byte(uint8_t CS,uint8_t CSValue, uint8_t Data){
    uint8_t i;
    St_SPI_CS(CS,CSValue);
    for(i = 0 ; i < 8 ; i++){
        St_SPI_SCL(F);
        St_SPI_MOSI(Data&(0x80>>i));
        St_SPI_SCL(T);
    }
    St_SPI_CS(CS,!CSValue);
    St_SPI_MOSI(F);
    St_SPI_SCL(F);
}

/**         2024.5.16        By DZAHZ
  * @Prototype   St_SPI_Writ_DoubleByte
  * @Description SPI˫�ֽ�д��
  * @param       CS        Ƭѡ�� ��Χ1~12
  * @param       CSValue   Ƭѡ��Чֵ,һ��͵�ƽ��Ч
  * @param       Data      Ҫд�������
  * @retval      ��
  */
void St_SPI_Writ_DoubleByte(uint8_t CS,uint8_t CSValue, uint16_t Data){
    /*St_SPI_Writ_Byte(CS,CSValue,Data>>8);
    St_SPI_Writ_Byte(CS,CSValue,Data);
    */
    uint8_t i , j;
    St_SPI_CS(CS,CSValue);
    for(j = 0 ; j < 2 ; j++){
        for(i = 0 ; i < 8 ; i++){
            St_SPI_SCL(F);
            St_SPI_MOSI((Data>>(1-j)*8)&(0x80>>i));
            St_SPI_SCL(T);
        }
    }
    St_SPI_CS(CS,!CSValue);
    St_SPI_MOSI(F);
    St_SPI_SCL(F);
}

/**         2024.5.16        By DZAHZ
  * @Prototype   St_SPI_Writ_DoubleByte
  * @Description SPI˫�ֽ�д��
  * @param       CS        Ƭѡ�� ��Χ3~12
  * @param       CSValue   Ƭѡ��Чֵ,һ��͵�ƽ��Ч
  * @retval      Data
  */
uint8_t St_SPI_Read_Byte(uint8_t CS,uint8_t CSValue){
    uint8_t i , Data;
    St_SPI_CS(CS,CSValue);
    for(i = 0 ; i < 8 ; i++){
        St_SPI_SCL(F);
        if(St_SPI_MISO() == 0) Data |= (0x80>>i);
        St_SPI_SCL(T);
    }
    St_SPI_CS(CS,!CSValue);
    return Data;
}

































