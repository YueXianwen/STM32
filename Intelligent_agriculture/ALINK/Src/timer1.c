#include "../Inc/timer1.h"
#include "../Inc/connect.h"

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

//void TIM1_UP_IRQHandler(void)	
//{
//	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){ 
//				
//		//....
//		LED1=!LED1;
//		connect_deal();
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);                //清除TIM1溢出中断标志 		
//	}

//}

/*-------------------------------------------------*/
/*函数名：定时器1使能30s定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_30S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //使能TIM1时钟	
	TIM_DeInit(TIM1);                                             //定时器1寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 30000-1; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}

/*-------------------------------------------------*/
/*函数名：定时器1使能10s定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_10S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //使能TIM1时钟	
	TIM_DeInit(TIM1);                                             //定时器1寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}


/*-------------------------------------------------*/
/*函数名：定时器1使能5s定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_5S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //使能TIM1时钟	
	TIM_DeInit(TIM1);                                             //定时器1寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 5000-1; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}

/*-------------------------------------------------*/
/*函数名：定时器1使能5s定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_2S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //使能TIM1时钟	
	TIM_DeInit(TIM1);                                             //定时器1寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 2000-1; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}


/*-------------------------------------------------*/
/*函数名：定时器1中断服务函数                      */
/*-------------------------------------------------*/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //如果TIM_IT_Update置位，表示TIM3溢出中断，进入if
		{           			
			switch(Ping_flag)
			{                          //判断Ping_flag的状态
				case 0:								   //如果Ping_flag等于0，表示正常状态，发送Ping报文  
						MQTT_PingREQ(); 		 //添加Ping报文到发送缓冲区  
						printf("Ping_flag==%d\r\n",Ping_flag);
						break;
				case 1:								   //如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
						TIM1_ENABLE_2S(); 	 //我们将定时器6设置为2s定时,快速发送Ping报文
						MQTT_PingREQ();  		 //添加Ping报文到发送缓冲区 
						printf("Ping_flag==%d\r\n",Ping_flag);
						break;
				case 2:								   //如果Ping_flag等于2，说明还没有收到服务器回复
				case 3:				           //如果Ping_flag等于3，说明还没有收到服务器回复
				case 4:				           //如果Ping_flag等于4，说明还没有收到服务器回复	
						MQTT_PingREQ();  		 //添加Ping报文到发送缓冲区 
						printf("Ping_flag==%d\r\n",Ping_flag);
						break;
				case 5:								   //如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
						printf("Ping_flag==%d\r\n",Ping_flag);
						Connect_flag = 0;        //连接状态置0，表示断开，没连上服务器
						TIM_Cmd(TIM1,DISABLE);   //关TIM3 				
						break;			
			}
			Ping_flag++;           		                  //Ping_flag自增1，表示又发送了一次ping，期待服务器的回复
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update); //清除TIM3溢出中断标志 	
	  }
}



