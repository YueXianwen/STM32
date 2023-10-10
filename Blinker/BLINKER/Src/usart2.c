/*      实现串口2功能的源文件       */
#include "stm32f10x.h"  //包含需要的头文件
#include "usart2.h"     //包含需要的头文件

USART_DataStruct UART2_Rx_Buffer;

/*------------------------------------------------- */
/*函数名：初始化串口2发送功能                           */
/*参  数：bound：波特率                               */
/*返回值：无                                         */
/*-------------------------------------------------*/
void USART2_Init(unsigned int bound)
{  	 	
	GPIO_InitTypeDef GPIO_InitStructure;      //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;    //定义一个设置串口功能的变量
    NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //使能串口2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
	
	USART_DeInit(USART2);                                  //串口2寄存器重新设置为默认值
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                //准备设置PA2   TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口2的发送
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //设置PA2
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                //准备设置PA3   RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入，用于串口2的接收
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //设置PA3
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制

	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式

    USART_Init(USART2, &USART_InitStructure);                                      //设置串口2	

	USART_ClearFlag(USART2, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启接收中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //设置串口2中断

	USART_Cmd(USART2, ENABLE);                              //使能串口2
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_TxData(USART_DataStruct DataStruct, u16 size)
{
	while((USART2->SR&0X40)==0);
	for(int	i = 0; i < size; i++)      //gwd 待发送数据的大小存在待发送数据的前2个字节中，以此来进行数据的发送
	{			
		USART2->DR = DataStruct.Data[i];                   //gwd 一次性发送1个字节
		while((USART2->SR&0X40)==0);	        //gwd 等待发送完成
	} 
}

/*-------------------------------------------------*/
/*函数名：串口2中断处理函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)                	//串口2中断服务程序
{

#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        UART2_Rx_Buffer.RxBuf =USART_ReceiveData(USART2);	//读取接收到的数据
        if(UART2_Rx_Buffer.Flag == 0)       //置位接收开始标志
        {
            UART2_Rx_Buffer.Data[UART2_Rx_Buffer.Count] = UART2_Rx_Buffer.RxBuf;
            UART2_Rx_Buffer.Count ++;
            /*进行包头校验*/
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
                    UART2_Rx_Buffer.Flag = 1;      //置位接收完成标志
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntExit();
#endif
}



