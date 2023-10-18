#include "stm32f10x.h"  
#include "string.h"
#include "../Inc/mqtt.h"
#include "../Inc/usart3.h"

char timer4_flag = 0;   //1:定时器4中断，有数据到

/*-------------------------------------------------*/
/*函数名：定时器4初始化                             */
/*参  数：arr：自动重装值   0~65535                 */
/*参  数：psc：时钟预分频数 0~65535                 */
/*返回值：无                                       */
/*说  明：定时时间：arr*psc*1000/72000000  单位ms   */
/*-------------------------------------------------*/
void TIM4_Init(unsigned short int arr, unsigned short int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           //使能TIM4时钟	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	                //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                  //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);            //设置TIM4
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                      //使能TIM4溢出中断    
	TIM_Cmd(TIM4,DISABLE);                                        //先关闭TIM4                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;                 //设置TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
	
}

/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)  //30ms进入一次  处理串口接收到的数据，每30ms处理一次
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){              //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if				
		memcpy(&MQTT_RxDataInPtr[2],Usart3_RxBuff,Usart3_RxCounter);  //拷贝数据到接收缓冲区(将串口接收到的数据COPY到缓存区进行相关的处理)
		MQTT_RxDataInPtr[0] = Usart3_RxCounter/256;                   //记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart3_RxCounter%256;                   //记录数据长度低字节
		MQTT_RxDataInPtr+=BUFF_UNIT;                                  //指针下移，此接收数据存储块已用掉！
		
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                     //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //指针归位到缓冲区开头		
		
		Usart3_RxCounter = 0;                                   //串口2接收数据量变量清零
		//TIM_SetCounter(TIM3, 0);                                //清零定时器3计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				  //关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			  //清零定时器4计数器
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
	}
}




