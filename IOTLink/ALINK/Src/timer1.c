#include "timer1.h"
#include "led.h"
#include "connect.h"

void TIM1_Init(unsigned short int arr, unsigned short int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            
	NVIC_InitTypeDef NVIC_InitStructure;                          
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE); //TIME1�ǹ�����APB2ʱ�������ϵ�        
	TIM_DeInit(TIM1);  
    
	TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //����TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //ʹ��TIM1����ж�    
	TIM_Cmd(TIM1,ENABLE);                                         //��TIM1                         
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn ;             //����TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�	
}

void TIM1_UP_IRQHandler(void)	
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){ 
				
		//....
		LED1=!LED1;
		connect_deal();
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);                //���TIM1����жϱ�־ 		
	}


}
