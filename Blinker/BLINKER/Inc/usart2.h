/*-------------------------------------------------*/
/*                                                 */
/*             实现串口2功能的头文件                  */
/*                                                 */
/*-------------------------------------------------*/
#ifndef __USART2_H
#define __USART2_H

/*串口收发数据结构体*/
typedef struct {
    uint8_t Data[50]; //接收数据存储区
    uint8_t RxBuf;    //接收缓冲区
    uint8_t Count;
    uint8_t Flag;
} USART_DataStruct;

extern USART_DataStruct UART2_Rx_Buffer;

void USART2_Init(unsigned int);
void USART2_TxData(USART_DataStruct DataStruct, u16 size);

#endif


