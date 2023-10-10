/*      ʵ�ִ���2���ܵ�Դ�ļ�       */
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�

USART_DataStruct UART2_Rx_Buffer;

/*------------------------------------------------- */
/*����������ʼ������2���͹���                           */
/*��  ����bound��������                               */
/*����ֵ����                                         */
/*-------------------------------------------------*/
void USART2_Init(unsigned int bound)
{  	 	
	GPIO_InitTypeDef GPIO_InitStructure;      //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;    //����һ�����ô��ڹ��ܵı���
    NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //ʹ�ܴ���2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOAʱ��
	
	USART_DeInit(USART2);                                  //����2�Ĵ�����������ΪĬ��ֵ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                //׼������PA2   TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���2�ķ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //����PA2
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                //׼������PA3   RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //�������룬���ڴ���2�Ľ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //����PA3
	
	USART_InitStructure.USART_BaudRate = bound;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������

	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);                                      //���ô���2	

	USART_ClearFlag(USART2, USART_FLAG_RXNE);	            //������ձ�־λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //���������ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //���ô���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //���ô���2�ж�

	USART_Cmd(USART2, ENABLE);                              //ʹ�ܴ���2
}

/*-------------------------------------------------*/
/*������������2���ͻ������е�����                  */
/*��  ����data������                               */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_TxData(USART_DataStruct DataStruct, u16 size)
{
	while((USART2->SR&0X40)==0);
	for(int	i = 0; i < size; i++)      //gwd ���������ݵĴ�С���ڴ��������ݵ�ǰ2���ֽ��У��Դ����������ݵķ���
	{			
		USART2->DR = DataStruct.Data[i];                   //gwd һ���Է���1���ֽ�
		while((USART2->SR&0X40)==0);	        //gwd �ȴ��������
	} 
}

/*-------------------------------------------------*/
/*������������2�жϴ�����                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)                	//����2�жϷ������
{

#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        UART2_Rx_Buffer.RxBuf =USART_ReceiveData(USART2);	//��ȡ���յ�������
        if(UART2_Rx_Buffer.Flag == 0)       //��λ���տ�ʼ��־
        {
            UART2_Rx_Buffer.Data[UART2_Rx_Buffer.Count] = UART2_Rx_Buffer.RxBuf;
            UART2_Rx_Buffer.Count ++;
            /*���а�ͷУ��*/
            if(1 == UART2_Rx_Buffer.Count)
            {
                if (0x55 != UART2_Rx_Buffer.Data[0])
                    UART2_Rx_Buffer.Count = 0;
            }
            else if(2 == UART2_Rx_Buffer.Count)
            {
                if(0x55 == UART2_Rx_Buffer.Data[1])
                    UART2_Rx_Buffer.Count = 1;
                else if(0xAA != UART2_Rx_Buffer.Data[1])
                    UART2_Rx_Buffer.Count = 0;
                else;
            }
            else
            {
                if(0x66 == UART2_Rx_Buffer.Data[UART2_Rx_Buffer.Count-1])
                {
                    UART2_Rx_Buffer.Count = 0;
                    UART2_Rx_Buffer.Flag = 1;      //��λ������ɱ�־
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}



