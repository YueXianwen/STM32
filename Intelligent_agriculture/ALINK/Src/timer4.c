#include "stm32f10x.h"  
#include "string.h"
#include "../Inc/mqtt.h"
#include "../Inc/usart3.h"

char timer4_flag = 0;   //1:��ʱ��4�жϣ������ݵ�

/*-------------------------------------------------*/
/*����������ʱ��4��ʼ��                             */
/*��  ����arr���Զ���װֵ   0~65535                 */
/*��  ����psc��ʱ��Ԥ��Ƶ�� 0~65535                 */
/*����ֵ����                                       */
/*˵  ������ʱʱ�䣺arr*psc*1000/72000000  ��λms   */
/*-------------------------------------------------*/
void TIM4_Init(unsigned short int arr, unsigned short int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           //ʹ��TIM4ʱ��	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	                //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                  //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);            //����TIM4
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                      //ʹ��TIM4����ж�    
	TIM_Cmd(TIM4,DISABLE);                                        //�ȹر�TIM4                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;                 //����TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
	
}

/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����                      */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)  //30ms����һ��  �����ڽ��յ������ݣ�ÿ30ms����һ��
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){              //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if				
		memcpy(&MQTT_RxDataInPtr[2],Usart3_RxBuff,Usart3_RxCounter);  //�������ݵ����ջ�����(�����ڽ��յ�������COPY��������������صĴ���)
		MQTT_RxDataInPtr[0] = Usart3_RxCounter/256;                   //��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart3_RxCounter%256;                   //��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr+=BUFF_UNIT;                                  //ָ�����ƣ��˽������ݴ洢�����õ���
		
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                     //���ָ�뵽������β����
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //ָ���λ����������ͷ		
		
		Usart3_RxCounter = 0;                                   //����2������������������
		//TIM_SetCounter(TIM3, 0);                                //���㶨ʱ��3�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);                        				  //�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			  //���㶨ʱ��4������
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //���TIM4����жϱ�־ 	
	}
}




