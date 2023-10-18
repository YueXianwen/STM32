#include "../Inc/timer1.h"
#include "../Inc/connect.h"

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

//void TIM1_UP_IRQHandler(void)	
//{
//	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){ 
//				
//		//....
//		LED1=!LED1;
//		connect_deal();
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);                //���TIM1����жϱ�־ 		
//	}

//}

/*-------------------------------------------------*/
/*����������ʱ��1ʹ��30s��ʱ                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_30S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //ʹ��TIM1ʱ��	
	TIM_DeInit(TIM1);                                             //��ʱ��1�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 30000-1; 	              //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //����TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //ʹ��TIM1����ж�    
	TIM_Cmd(TIM1,ENABLE);                                         //��TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //����TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}

/*-------------------------------------------------*/
/*����������ʱ��1ʹ��10s��ʱ                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_10S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //ʹ��TIM1ʱ��	
	TIM_DeInit(TIM1);                                             //��ʱ��1�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1; 	              //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //����TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //ʹ��TIM1����ж�    
	TIM_Cmd(TIM1,ENABLE);                                         //��TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //����TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}


/*-------------------------------------------------*/
/*����������ʱ��1ʹ��5s��ʱ                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_5S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //ʹ��TIM1ʱ��	
	TIM_DeInit(TIM1);                                             //��ʱ��1�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 5000-1; 	              //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //����TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //ʹ��TIM1����ж�    
	TIM_Cmd(TIM1,ENABLE);                                         //��TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //����TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}

/*-------------------------------------------------*/
/*����������ʱ��1ʹ��5s��ʱ                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_2S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //ʹ��TIM1ʱ��	
	TIM_DeInit(TIM1);                                             //��ʱ��1�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 2000-1; 	              //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=14400-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //����TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //ʹ��TIM1����ж�    
	TIM_Cmd(TIM1,ENABLE);                                         //��TIM1                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //����TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}


/*-------------------------------------------------*/
/*����������ʱ��1�жϷ�����                      */
/*-------------------------------------------------*/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM_IT_Update��λ����ʾTIM3����жϣ�����if
		{           			
			switch(Ping_flag)
			{                          //�ж�Ping_flag��״̬
				case 0:								   //���Ping_flag����0����ʾ����״̬������Ping����  
						MQTT_PingREQ(); 		 //���Ping���ĵ����ͻ�����  
						printf("Ping_flag==%d\r\n",Ping_flag);
						break;
				case 1:								   //���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
						TIM1_ENABLE_2S(); 	 //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
						MQTT_PingREQ();  		 //���Ping���ĵ����ͻ����� 
						printf("Ping_flag==%d\r\n",Ping_flag);
						break;
				case 2:								   //���Ping_flag����2��˵����û���յ��������ظ�
				case 3:				           //���Ping_flag����3��˵����û���յ��������ظ�
				case 4:				           //���Ping_flag����4��˵����û���յ��������ظ�	
						MQTT_PingREQ();  		 //���Ping���ĵ����ͻ����� 
						printf("Ping_flag==%d\r\n",Ping_flag);
						break;
				case 5:								   //���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
						printf("Ping_flag==%d\r\n",Ping_flag);
						Connect_flag = 0;        //����״̬��0����ʾ�Ͽ���û���Ϸ�����
						TIM_Cmd(TIM1,DISABLE);   //��TIM3 				
						break;			
			}
			Ping_flag++;           		                  //Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update); //���TIM3����жϱ�־ 	
	  }
}



