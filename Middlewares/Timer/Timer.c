#include "Timer.h"

//72MHz = 72 000 000HZ



void Timer_Init(void){
    //����ṹ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    //ѡ��ʱ����Ԫ(�ڲ�ʱ��)
    TIM_InternalClockConfig(TIM2);
    //���ýṹ��
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ָ��ʱ�ӷ�Ƶֵ 1��Ƶ 
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ ���ϼ���
    TIM_TimeBaseInitStructure.TIM_Period = 10000-1; //ARR�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1; //PSCԤ��Ƶ��ֵ
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //�ظ�������ֵ
    //CK_CNT_OV = CK_CNT / (PSC+1) / (ARR+1) ���������Ƶ�� = ϵͳʱ��/(ARR+1)/(PSC+1)
    //��ʼ��ʱ����Ԫ
    //TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    //���TIM2�����жϱ�־λ
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    //TIMʹ���ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    //NVIC ����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ; //�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ; //ʹ��ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1 ; //��Ӧ���ȼ�
    NVIC_Init(&NVIC_InitStructure);
    //������ʱ��
    TIM_Cmd(TIM2, ENABLE);
}
/*
void TIM2_IRQHandler(void){ //TIM2�жϺ���
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        
    }
}
*/
