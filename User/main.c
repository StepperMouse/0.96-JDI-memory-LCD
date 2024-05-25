#include "main.h"



void JDI_Display(void){                //OLED屏幕显示
    //unsigned char Xin2[]={0x49,0x2A,0x1C,0x7F,0x1C,0x2A,0x49,};
    //MPU6050输出
    //int16_t Ax, Ay, Az , Gx, Gy, Gz, Tp;
    //JDI_WriteRAM_Str(8,0,"-------Test Window------");
    JDI_WriteRAM_Str(2,0,"Today is KFC Crazy Thursday");
    JDI_WriteRAM_Str(8,84,"@StepMouse");
    JDILink_Partial_Refresh(0x23,0,144);
    
    //DigitalClock(0,0);
    //-------------------------------------------------------------
    /*
        do{
            MPU_6050_ReadData(&Ax, &Ay, &Az, &Tp , &Gx, &Gy, &Gz);
            JDI_WriteRAM_SignedNum(8, 6 , Ax, 4, 1);//(y , x ,Number,Length,Symbol)
            JDI_WriteRAM_SignedNum(8, 5 , Ay, 4, 1);
            JDI_WriteRAM_SignedNum(8, 4 , Az, 4, 1);
            JDI_WriteRAM_SignedNum(50, 6 , Gx, 4, 1);
            JDI_WriteRAM_SignedNum(50, 5 , Gy, 4, 1);
            JDI_WriteRAM_SignedNum(50, 4 , Gz, 4, 1);
            JDI_WriteRAM_SignedNum(8, 2 , Tp, 4, 1);
            JDI_Start();
            JDILink_Partial_Refresh(0x23,8,90);
            //JDILink_Partial_Refresh(0x23,100,120);
            JDI_Stop();
        }while (0);*/
    }

/**         2023.5.25       By DZAHZ OR StepMouse
  * @Prototype   void DigitalClock
  * @Description 简单低24h时钟
  * @param       x0 行位置, 范围0-8
  * @param       y0 列位置, 范围0-144
  * @retval      无
  */

int8_t Second = 0 , Minute = 0 , Hour = 0 , TimeFlag = 0 ;
void DigitalClock(uint8_t x0,uint16_t y0){
    //初始化
    if(TimeFlag == 0){
        if(Hour==0)     {JDI_WriteRAM_Str(x0,y0,"00"); JDI_WriteRAM_Char(x0,y0+13,':');}
        if(Minute==0)   {JDI_WriteRAM_Str(x0,19+y0,"00"); JDI_WriteRAM_Char(x0,y0+32,':');}
        if(Second==0)   JDI_WriteRAM_Str(x0,38+y0,"00");
        JDILink_Partial_Refresh(0x23,0,y0+50);//屏幕刷新
        TimeFlag = 1;
    }
    //秒显示
    if(Second>=10)JDI_WriteRAM_SignedNum(x0,38+y0,Second,2,0);
    else JDI_WriteRAM_SignedNum(x0,44+y0,Second,1,0);
    JDILink_Partial_Refresh(0x23,y0+38,y0+50);//屏幕刷新
    Second++;
    if(Second < 60) return;
    Second = 0; Minute++; TimeFlag = 0;
    //分显示
    if(Minute>=10)JDI_WriteRAM_SignedNum(x0,19+y0,Minute,2,0);
    else JDI_WriteRAM_SignedNum(x0,25+y0,Minute,1,0);
    if(Minute < 60) return;
    Minute = 0; Hour++; TimeFlag = 0;
    //时显示
    if(Hour>=10)JDI_WriteRAM_SignedNum(x0,y0,Hour,2,0);
    else JDI_WriteRAM_SignedNum(x0,6+y0,Hour,1,0);
    if(Hour < 24) return;
    Hour = 0; TimeFlag = 0;   
}



int main(void){                       //主函数
   DelayInit();            //计时器配置
    Timer_Init();
    //软件SPI初始化
    St_SPI_Init();
    //LCD初始化
    JDI_LCD_Init(0xff);
    JDI_Display();

    while (1){  }
}
