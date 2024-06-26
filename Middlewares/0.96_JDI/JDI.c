#include "JDI.h"
#include "JDIfont.h"

uint8_t NullChar;
uint8_t i, j;
uint8_t JDI_1Bit_RAM[144][9]; //单色数组
//uint8_t JDI_3Bit_RAM[144][27];//三色数组

void JDI_LCD_Init(uint8_t nullchar){  //清空数组, 取值0xff白底黑字, 0x00黑底白字
    NullChar = nullchar;
    for(i = 1 ; i < 144 ; i++){
        for(j = 0 ; j < 9 ; j++){
            JDI_1Bit_RAM[i][j] = nullchar;
        }
    }
}
void JDI_Start(void){                   //开始
    St_SPI_CS(CS,T);
}
void JDILink_6Bit(uint8_t Mode){       //LCD模式选择,前六位有效
    for(i = 0 ; i < 6 ; i++){ //Mode selectperiod(6cloks)
        St_SPI_SCL(F);
        St_SPI_MOSI(Mode&0x20>>i);
        St_SPI_SCL(T);
    }
}
void JDILink_10Bit(uint16_t Data){     //LCD门地址,前十位有效
    for(i = 0 ; i < 10 ; i++){ //Gate line Address selectperiod
        St_SPI_SCL(F);
        St_SPI_MOSI(Data&0x0200>>i);
        St_SPI_SCL(T);
    }
}
void JDILink_Line_Refresh(uint16_t Line){   //单行刷新
    JDILink_10Bit(Line);
    for(i = 0; i < 9; i++){
        for(j = 0 ; j < 8 ; j++){   //循环输出1Byte值
            St_SPI_SCL(F);
            if((JDI_1Bit_RAM[Line][i])&(0x80>>j)){St_SPI_MOSI(T);}
            else {St_SPI_MOSI(F);}
            St_SPI_SCL(T);
        }
    }
}
void JDI_Stop(void){   //停止
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

void JDILink_Partial_Refresh(uint8_t Mode, uint16_t SLine, uint16_t ELine){     //LCD局部刷新
    uint16_t c;
    JDI_Start();
    JDILink_6Bit(Mode);
    for(c = SLine; c <= ELine; c++){//起始行和结束行
        JDILink_Line_Refresh(c);    //行刷新
        St_SPI_MOSI(F);
        for(j = 0; j < 6; j++){ //6位虚拟数据
            St_SPI_SCL(F);
            St_SPI_SCL(T);
        }
    }
    JDI_Stop();
}

//**************************显存写入**********************************
void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data){ //向数组写入单个6*8字体
    uint8_t i ;
    //for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][x] = (JDI68[Data-32][i]);
    if(NullChar == 0xff) for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][8-x] = ~(JDI68[Data-32][i]);
    else for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][8-x] = (JDI68[Data-32][i]);
}

void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String){ //写入字符串
	uint16_t c = 0 ;i = 0;
    while(String[i] != '\0'){               //循环显示字符串内的内容
        if(c > 143){x++; c=0;}              //如果y轴大于143 切换至下一行
        JDI_WriteRAM_Char(x,y+c,String[i]); //显示单个字符
        c+=6; i++;
    }
}

void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){ //写入十进制数
    uint8_t temp[21], i = 0, j, n = 6;     //不知道为什么用temp[20]会改变y0位置
    //是否添加符号
    if (symbol == 1){if(Number > 0) JDI_WriteRAM_Char(y,x,'+'); else JDI_WriteRAM_Char(y,x,'-');x += n;}
    if (Number < 0) Number = -Number;//负数取反
    while (Number){                  //将整数拆分,并装入数组循环直到个位为0
        temp[21-i] = Number%10+0x30; //将整数个位取出,转换为该个位的ASCLL码,并存储在数组temp中
        i++; Number /= 10;}          //去掉整数个位
    j = i ;                          //获取位数
    for(i = 0 ; i<j ; i ++) temp[i] = temp[21-j+i+1];         //倒装数组
    if (!i){temp[0] = '0'; j = 1 ;}                           //若Number为0
    //显示6*8或8*16字符
    for(i = 0 ; (i<j)&&(i<Length) ; i++) {
    JDI_WriteRAM_Char(x,y+i*n,temp[i]);}   //显示数字
    for(i = Length-1 ; i > j ;i--) JDI_WriteRAM_Char(x,y+i*n,' ');               //显示空字符
}



/*
void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){
    uint8_t temp[20], i = 0, j, n = 6;
    //是否添加符号
    if (symbol == 1){if(Number > 0) JDI_WriteRAM_Char(y,x,'+'); else JDI_WriteRAM_Char(y,x,'-');x += n;}
    //|********************************************************************************************
    if (Number < 0) Number = -Number;//负数取反                                                  *
    while (Number){                  //将整数拆分,并装入数组循环直到个位为0                      *
        temp[20-i] = Number%10+0x30; //将整数个位取出,转换为该个位的ASCLL码,并存储在数组temp中   *
        i++; Number /= 10;}          //去掉整数个位                                              *
    j = i ;                          //获取位数                                                  *
    for(i = 0 ; i<j ; i ++) temp[i] = temp[20-j+i+1];         //倒装数组                         *
    if (!i){temp[0] = '0'; j = 1 ;}                           //若Number为0                      *
    //-------------------------------------------------------------------------------------------*
    //显示6*8或8*16字符
    for(i = 0 ; (i<j)&&(i<Length) ; i++) JDI_WriteRAM_Char(x,y+i*n,temp[i]);   //显示数字
    for(i = Length ; i > j ;i--) JDI_WriteRAM_Char(x,y+i*n,' ');               //显示空字符
}
*/

/*

void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data){ //6*8字体
    unsigned char c=0 ;
    c = Data-32;
    for(i=0;i<6;i++){JDI_1Bit_RAM[y+i][x] = (JDI68[j][i]);}
}

void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String){
	uint16_t c = 0 ; i = 0;
    while(String[i] != '\0'){           //循环显示字符串内的内容
        if(c > 143){ x--; c=0; }        //如果y轴大于 143 切换至下一行
        JDI_WriteRAM_Char(x,y+c,String[i]);  //显示单个字符
        c+=6;i++;
    }
}

void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){
    char temp[20];
    unsigned char i = 0,j,n = 6;
    if (symbol == 1){//symbol为1时,判断正负并添加符号
        if(Number > 0) JDI_WriteRAM_Char(y,x,'+');
        if(Number < 0) JDI_WriteRAM_Char(y,x,'-');
        x += n;}
    //|********************************************************************************************
    if (Number < 0) Number = -Number;//负数取反                                                  *
    uint32_t Num = Number;           //将Number赋值到Num(无符号)                                 *
    //将整数拆分,并装入数组                                                                      *
    while (Num){                     //循环直到个位为0                                           *                               
        temp[20-i] = Num%10+0x30;    //将整数个位取出,转换为该个位的ASCLL码,并存储在数组temp中   *
        i++;                         //                                                          *
        Num /= 10;}                  //去掉整数个位                                              *
    j = i ;                          //                                                          *
    for(i = 0 ; i<j ; i ++) temp[i] = temp[20-j+i+1];         //倒装数组                         *
    if (!i){temp[0] = '0'; j = 1 ;}                           //若Number为0                      *
    //-------------------------------------------------------------------------------------------*
    显示6*8或8*16字符
    for(i = 0 ; (i<j)&&(i<Length) ; i++)    JDI_WriteRAM_Char(y,x+i*n,temp[i]);    //显示数字
    for(i = Length ; i >= j ;i--)   JDI_WriteRAM_Char(y,x+i*n,' ');                //显示空字�
}

*/































