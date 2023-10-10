#include "timer1.h"
#include "led.h"
#include "connect.h"

void TIM1_Init(unsigned short int arr, unsigned short int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            
	NVIC_InitTypeDef NVIC_InitStructure;                          
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE); //TIME1是挂载在APB2时钟总线上的        
	TIM_DeInit(TIM1);  
    
	TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1                         
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn ;             //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断	
}

void TIM1_UP_IRQHandler(void)	
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){ 
				
		//....
		LED1=!LED1;
		connect_deal();
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);                //清除TIM1溢出中断标志 		
	}


}
