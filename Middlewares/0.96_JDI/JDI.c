#include "JDI.h"
#include "JDIfont.h"

uint8_t NullChar;
uint8_t i, j;
uint8_t JDI_1Bit_RAM[144][9]; //µ¥É«Êý×é
//uint8_t JDI_3Bit_RAM[144][27];//ÈýÉ«Êý×é

void JDI_LCD_Init(uint8_t nullchar){  //Çå¿ÕÊý×é, È¡Öµ0xff°×µ×ºÚ×Ö, 0x00ºÚµ×°××Ö
    NullChar = nullchar;
    for(i = 1 ; i < 144 ; i++){
        for(j = 0 ; j < 9 ; j++){
            JDI_1Bit_RAM[i][j] = nullchar;
        }
    }
}
void JDI_Start(void){                   //¿ªÊ¼
    St_SPI_CS(CS,T);
}
void JDILink_6Bit(uint8_t Mode){       //LCDÄ£Ê½Ñ¡Ôñ,Ç°ÁùÎ»ÓÐÐ§
    for(i = 0 ; i < 6 ; i++){ //Mode selectperiod(6cloks)
        St_SPI_SCL(F);
        St_SPI_MOSI(Mode&0x20>>i);
        St_SPI_SCL(T);
    }
}
void JDILink_10Bit(uint16_t Data){     //LCDÃÅµØÖ·,Ç°Ê®Î»ÓÐÐ§
    for(i = 0 ; i < 10 ; i++){ //Gate line Address selectperiod
        St_SPI_SCL(F);
        St_SPI_MOSI(Data&0x0200>>i);
        St_SPI_SCL(T);
    }
}
void JDILink_Line_Refresh(uint16_t Line){   //µ¥ÐÐË¢ÐÂ
    JDILink_10Bit(Line);
    for(i = 0; i < 9; i++){
        for(j = 0 ; j < 8 ; j++){   //Ñ­»·Êä³ö1ByteÖµ
            St_SPI_SCL(F);
            if((JDI_1Bit_RAM[Line][i])&(0x80>>j)){St_SPI_MOSI(T);}
            else {St_SPI_MOSI(F);}
            St_SPI_SCL(T);
        }
    }
}
void JDI_Stop(void){   //Í£Ö¹
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

void JDILink_Partial_Refresh(uint8_t Mode, uint16_t SLine, uint16_t ELine){     //LCD¾Ö²¿Ë¢ÐÂ
    uint16_t c;
    JDI_Start();
    JDILink_6Bit(Mode);
    for(c = SLine; c <= ELine; c++){//ÆðÊ¼ÐÐºÍ½áÊøÐÐ
        JDILink_Line_Refresh(c);    //ÐÐË¢ÐÂ
        St_SPI_MOSI(F);
        for(j = 0; j < 6; j++){ //6Î»ÐéÄâÊý¾Ý
            St_SPI_SCL(F);
            St_SPI_SCL(T);
        }
    }
    JDI_Stop();
}

//**************************ÏÔ´æÐ´Èë**********************************
void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data){ //ÏòÊý×éÐ´Èëµ¥¸ö6*8×ÖÌå
    uint8_t i ;
    //for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][x] = (JDI68[Data-32][i]);
    if(NullChar == 0xff) for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][8-x] = ~(JDI68[Data-32][i]);
    else for(i=0;i<6;i++)JDI_1Bit_RAM[y+i][8-x] = (JDI68[Data-32][i]);
}

void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String){ //Ð´Èë×Ö·û´®
	uint16_t c = 0 ;i = 0;
    while(String[i] != '\0'){               //Ñ­»·ÏÔÊ¾×Ö·û´®ÄÚµÄÄÚÈÝ
        if(c > 143){x++; c=0;}              //Èç¹ûyÖá´óÓÚ143 ÇÐ»»ÖÁÏÂÒ»ÐÐ
        JDI_WriteRAM_Char(x,y+c,String[i]); //ÏÔÊ¾µ¥¸ö×Ö·û
        c+=6; i++;
    }
}

void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){ //Ð´ÈëÊ®½øÖÆÊý
    uint8_t temp[21], i = 0, j, n = 6;     //²»ÖªµÀÎªÊ²Ã´ÓÃtemp[20]»á¸Ä±äy0Î»ÖÃ
    //ÊÇ·ñÌí¼Ó·ûºÅ
    if (symbol == 1){if(Number > 0) JDI_WriteRAM_Char(y,x,'+'); else JDI_WriteRAM_Char(y,x,'-');x += n;}
    if (Number < 0) Number = -Number;//¸ºÊýÈ¡·´
    while (Number){                  //½«ÕûÊý²ð·Ö,²¢×°ÈëÊý×éÑ­»·Ö±µ½¸öÎ»Îª0
        temp[21-i] = Number%10+0x30; //½«ÕûÊý¸öÎ»È¡³ö,×ª»»Îª¸Ã¸öÎ»µÄASCLLÂë,²¢´æ´¢ÔÚÊý×étempÖÐ
        i++; Number /= 10;}          //È¥µôÕûÊý¸öÎ»
    j = i ;                          //»ñÈ¡Î»Êý
    for(i = 0 ; i<j ; i ++) temp[i] = temp[21-j+i+1];         //µ¹×°Êý×é
    if (!i){temp[0] = '0'; j = 1 ;}                           //ÈôNumberÎª0
    //ÏÔÊ¾6*8»ò8*16×Ö·û
    for(i = 0 ; (i<j)&&(i<Length) ; i++) {
    JDI_WriteRAM_Char(x,y+i*n,temp[i]);}   //ÏÔÊ¾Êý×Ö
    for(i = Length-1 ; i > j ;i--) JDI_WriteRAM_Char(x,y+i*n,' ');               //ÏÔÊ¾¿Õ×Ö·û
}



/*
void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){
    uint8_t temp[20], i = 0, j, n = 6;
    //ÊÇ·ñÌí¼Ó·ûºÅ
    if (symbol == 1){if(Number > 0) JDI_WriteRAM_Char(y,x,'+'); else JDI_WriteRAM_Char(y,x,'-');x += n;}
    //|********************************************************************************************
    if (Number < 0) Number = -Number;//¸ºÊýÈ¡·´                                                  *
    while (Number){                  //½«ÕûÊý²ð·Ö,²¢×°ÈëÊý×éÑ­»·Ö±µ½¸öÎ»Îª0                      *
        temp[20-i] = Number%10+0x30; //½«ÕûÊý¸öÎ»È¡³ö,×ª»»Îª¸Ã¸öÎ»µÄASCLLÂë,²¢´æ´¢ÔÚÊý×étempÖÐ   *
        i++; Number /= 10;}          //È¥µôÕûÊý¸öÎ»                                              *
    j = i ;                          //»ñÈ¡Î»Êý                                                  *
    for(i = 0 ; i<j ; i ++) temp[i] = temp[20-j+i+1];         //µ¹×°Êý×é                         *
    if (!i){temp[0] = '0'; j = 1 ;}                           //ÈôNumberÎª0                      *
    //-------------------------------------------------------------------------------------------*
    //ÏÔÊ¾6*8»ò8*16×Ö·û
    for(i = 0 ; (i<j)&&(i<Length) ; i++) JDI_WriteRAM_Char(x,y+i*n,temp[i]);   //ÏÔÊ¾Êý×Ö
    for(i = Length ; i > j ;i--) JDI_WriteRAM_Char(x,y+i*n,' ');               //ÏÔÊ¾¿Õ×Ö·û
}
*/

/*

void JDI_WriteRAM_Char(uint8_t x, uint16_t y, uint8_t Data){ //6*8×ÖÌå
    unsigned char c=0 ;
    c = Data-32;
    for(i=0;i<6;i++){JDI_1Bit_RAM[y+i][x] = (JDI68[j][i]);}
}

void JDI_WriteRAM_Str(uint8_t x, uint16_t y, char *String){
	uint16_t c = 0 ; i = 0;
    while(String[i] != '\0'){           //Ñ­»·ÏÔÊ¾×Ö·û´®ÄÚµÄÄÚÈÝ
        if(c > 143){ x--; c=0; }        //Èç¹ûyÖá´óÓÚ 143 ÇÐ»»ÖÁÏÂÒ»ÐÐ
        JDI_WriteRAM_Char(x,y+c,String[i]);  //ÏÔÊ¾µ¥¸ö×Ö·û
        c+=6;i++;
    }
}

void JDI_WriteRAM_SignedNum(uint8_t x,uint16_t y, int Number,unsigned char Length,uint8_t symbol){
    char temp[20];
    unsigned char i = 0,j,n = 6;
    if (symbol == 1){//symbolÎª1Ê±,ÅÐ¶ÏÕý¸º²¢Ìí¼Ó·ûºÅ
        if(Number > 0) JDI_WriteRAM_Char(y,x,'+');
        if(Number < 0) JDI_WriteRAM_Char(y,x,'-');
        x += n;}
    //|********************************************************************************************
    if (Number < 0) Number = -Number;//¸ºÊýÈ¡·´                                                  *
    uint32_t Num = Number;           //½«Number¸³Öµµ½Num(ÎÞ·ûºÅ)                                 *
    //½«ÕûÊý²ð·Ö,²¢×°ÈëÊý×é                                                                      *
    while (Num){                     //Ñ­»·Ö±µ½¸öÎ»Îª0                                           *                               
        temp[20-i] = Num%10+0x30;    //½«ÕûÊý¸öÎ»È¡³ö,×ª»»Îª¸Ã¸öÎ»µÄASCLLÂë,²¢´æ´¢ÔÚÊý×étempÖÐ   *
        i++;                         //                                                          *
        Num /= 10;}                  //È¥µôÕûÊý¸öÎ»                                              *
    j = i ;                          //                                                          *
    for(i = 0 ; i<j ; i ++) temp[i] = temp[20-j+i+1];         //µ¹×°Êý×é                         *
    if (!i){temp[0] = '0'; j = 1 ;}                           //ÈôNumberÎª0                      *
    //-------------------------------------------------------------------------------------------*
    ÏÔÊ¾6*8»ò8*16×Ö·û
    for(i = 0 ; (i<j)&&(i<Length) ; i++)    JDI_WriteRAM_Char(y,x+i*n,temp[i]);    //ÏÔÊ¾Êý×Ö
    for(i = Length ; i >= j ;i--)   JDI_WriteRAM_Char(y,x+i*n,' ');                //ÏÔÊ¾¿Õ×Ö·
}

*/































