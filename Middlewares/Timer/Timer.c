#include "Timer.h"

//72MHz = 72 000 000HZ



void Timer_Init(void){
    //定义结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    //选择时基单元(内部时钟)
    TIM_InternalClockConfig(TIM2);
    //配置结构体
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //指定时钟分频值 1分频 
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 10000-1; //ARR自动重装器值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1; //PSC预分频器值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复计数器值
    //CK_CNT_OV = CK_CNT / (PSC+1) / (ARR+1) 计数器溢出频率 = 系统时钟/(ARR+1)/(PSC+1)
    //初始化时基单元
    //TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    //清除TIM2更新中断标志位
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    //TIM使能中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    //NVIC 配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ; //中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ; //使能通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1 ; //响应优先级
    NVIC_Init(&NVIC_InitStructure);
    //启动定时器
    TIM_Cmd(TIM2, ENABLE);
}
/*
void TIM2_IRQHandler(void){ //TIM2中断函数
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        
    }
}
*/
