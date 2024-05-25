#include "JDI.h"
#include "JDIfont.h"

uint8_t NullChar;
uint8_t i, j;
uint8_t JDI_1Bit_RAM[144][9]; //��ɫ����
//uint8_t JDI_3Bit_RAM[144][27];//��ɫ����

void JDI_LCD_Init(uint8_t nullchar){  //�������, ȡֵ0xff�׵׺���, 0x00�ڵװ���
    NullChar = nullchar;
    for(i = 1 ; i < 144 ; i++){
        for(j = 0 ; j < 9 ; j++){
            JDI_1Bit_RAM[i][j] = nullchar;
        }
    }
}
void JDI_Start(void){                   //��ʼ
    St_SPI_CS(CS,T);
}
void JDILink_6Bit(uint8_t Mode){       //LCDģʽѡ��,ǰ��λ��Ч
    for(i = 0 ; i < 6 ; i++){ //Mode selectperiod(6cloks)
        St_SPI_SCL(F);
        St_SPI_MOSI(Mode&0x20>>i);
        St_SPI_SCL(T);
    }
}
void JDILink_10Bit(uint16_t Data){     //LCD�ŵ�ַ,ǰʮλ��Ч
    for(i = 0 ; i < 10 ; i++){ //Gate line Address selectperiod
        St_SPI_SCL(F);
        St_SPI_MOSI(Data&0x0200>>i);
        St_SPI_SCL(T);
    }
}
void JDILink_Line_Refresh(uint16_t Line){   //����ˢ��
    JDILink_10Bit(Line);
    for(i = 0; i < 9; i++){
        for(j = 0 ; j < 8 ; j++){   //ѭ�����1Byteֵ
            St_SPI_SCL(F);
            if((JDI_1Bit_RAM[Line][i])&(0x80>>j)){St_SPI_MOSI(T);}
            else {St_SPI_MOSI(F);}
            St_SPI_SCL(T);
        }
    }
}
void JDI_Stop(void){   //ֹͣ
    uint8_t i ;
    St_SPI_MOSI(F);
    for(i = 0 ; i < 16 ; i++){
        St_SPI_SCL(F);
        St_SPI_SCL(T);
    }
    St_SPI_CS(CS,F);
    St_SPI_MOSI(F);
    St_SPI_SCL(F);
}

void JDILink_Partial_Refresh(uint8_t Mode, uint16_t SLine, uint16_t ELine){     //LCD�ֲ�ˢ��
    uint16_t c;
    JDI_Start();
    JDILink_6Bit(Mode);
    for(c = SLine; c <= ELine; c++){//��ʼ�кͽ�����
        JDILink_Line_Refresh(c);    //��ˢ��
        St_SPI_MOSI(F);
        for(j = 0; j < 6; j++){ //6λ��������
            St_SPI_SCL(F);
            St_SPI_SCL(T);
        }
    }
    JDI_Stop();
}

//**************************�Դ�д��**********************************
void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data){ //������д�뵥��6*8����
    uint8_t i ;
    //for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][x] = (JDI68[Data-32][i]);
    if(NullChar == 0xff) for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][8-x] = ~(JDI68[Data-32][i]);
    else for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][8-x] = (JDI68[Data-32][i]);
}

void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String){ //д���ַ���
	uint16_t c = 0 ;i = 0;
    while(String[i] != '\0'){               //ѭ����ʾ�ַ����ڵ�����
        if(c > 143){x++; c=0;}              //���y�����143 �л�����һ��
        JDI_WriteRAM_Char(x,y+c,String[i]); //��ʾ�����ַ�
        c+=6; i++;
    }
}

void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){ //д��ʮ������
    uint8_t temp[21], i = 0, j, n = 6;     //��֪��Ϊʲô��temp[20]��ı�y0λ��
    //�Ƿ���ӷ���
    if (symbol == 1){if(Number > 0) JDI_WriteRAM_Char(y,x,'+'); else JDI_WriteRAM_Char(y,x,'-');x += n;}
    if (Number < 0) Number = -Number;//����ȡ��
    while (Number){                  //���������,��װ������ѭ��ֱ����λΪ0
        temp[21-i] = Number%10+0x30; //��������λȡ��,ת��Ϊ�ø�λ��ASCLL��,���洢������temp��
        i++; Number /= 10;}          //ȥ��������λ
    j = i ;                          //��ȡλ��
    for(i = 0 ; i<j ; i ++) temp[i] = temp[21-j+i+1];         //��װ����
    if (!i){temp[0] = '0'; j = 1 ;}                           //��NumberΪ0
    //��ʾ6*8��8*16�ַ�
    for(i = 0 ; (i<j)&&(i<Length) ; i++) {
    JDI_WriteRAM_Char(x,y+i*n,temp[i]);}   //��ʾ����
    for(i = Length-1 ; i > j ;i--) JDI_WriteRAM_Char(x,y+i*n,' ');               //��ʾ���ַ�
}



/*
void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){
    uint8_t temp[20], i = 0, j, n = 6;
    //�Ƿ���ӷ���
    if (symbol == 1){if(Number > 0) JDI_WriteRAM_Char(y,x,'+'); else JDI_WriteRAM_Char(y,x,'-');x += n;}
    //|********************************************************************************************
    if (Number < 0) Number = -Number;//����ȡ��                                                  *
    while (Number){                  //���������,��װ������ѭ��ֱ����λΪ0                      *
        temp[20-i] = Number%10+0x30; //��������λȡ��,ת��Ϊ�ø�λ��ASCLL��,���洢������temp��   *
        i++; Number /= 10;}          //ȥ��������λ                                              *
    j = i ;                          //��ȡλ��                                                  *
    for(i = 0 ; i<j ; i ++) temp[i] = temp[20-j+i+1];         //��װ����                         *
    if (!i){temp[0] = '0'; j = 1 ;}                           //��NumberΪ0                      *
    //-------------------------------------------------------------------------------------------*
    //��ʾ6*8��8*16�ַ�
    for(i = 0 ; (i<j)&&(i<Length) ; i++) JDI_WriteRAM_Char(x,y+i*n,temp[i]);   //��ʾ����
    for(i = Length ; i > j ;i--) JDI_WriteRAM_Char(x,y+i*n,' ');               //��ʾ���ַ�
}
*/

/*

void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data){ //6*8����
    unsigned char c=0 ;
    c = Data-32;
    for(i=0;i<6;i++){JDI_1Bit_RAM[y+i][x] = (JDI68[j][i]);}
}

void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String){
	uint16_t c = 0 ; i = 0;
    while(String[i] != '\0'){           //ѭ����ʾ�ַ����ڵ�����
        if(c > 143){ x--; c=0; }        //���y����� 143 �л�����һ��
        JDI_WriteRAM_Char(x,y+c,String[i]);  //��ʾ�����ַ�
        c+=6;i++;
    }
}

void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){
    char temp[20];
    unsigned char i = 0,j,n = 6;
    if (symbol == 1){//symbolΪ1ʱ,�ж���������ӷ���
        if(Number > 0) JDI_WriteRAM_Char(y,x,'+');
        if(Number < 0) JDI_WriteRAM_Char(y,x,'-');
        x += n;}
    //|********************************************************************************************
    if (Number < 0) Number = -Number;//����ȡ��                                                  *
    uint32_t Num = Number;           //��Number��ֵ��Num(�޷���)                                 *
    //���������,��װ������                                                                      *
    while (Num){                     //ѭ��ֱ����λΪ0                                           *                               
        temp[20-i] = Num%10+0x30;    //��������λȡ��,ת��Ϊ�ø�λ��ASCLL��,���洢������temp��   *
        i++;                         //                                                          *
        Num /= 10;}                  //ȥ��������λ                                              *
    j = i ;                          //                                                          *
    for(i = 0 ; i<j ; i ++) temp[i] = temp[20-j+i+1];         //��װ����                         *
    if (!i){temp[0] = '0'; j = 1 ;}                           //��NumberΪ0                      *
    //-------------------------------------------------------------------------------------------*
    ��ʾ6*8��8*16�ַ�
    for(i = 0 ; (i<j)&&(i<Length) ; i++)    JDI_WriteRAM_Char(y,x+i*n,temp[i]);    //��ʾ����
    for(i = Length ; i >= j ;i--)   JDI_WriteRAM_Char(y,x+i*n,' ');                //��ʾ���ַ
}

*/































