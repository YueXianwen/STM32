/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���2���ܵ�ͷ�ļ�                  */
/*                                                 */
/*-------------------------------------------------*/
#ifndef __USART2_H
#define __USART2_H

/*�����շ����ݽṹ��*/
typedef struct {
    uint8_t Data[50]; //�������ݴ洢��
    uint8_t RxBuf;    //���ջ�����
    uint8_t Count;
    uint8_t Flag;
} USART_DataStruct;

extern USART_DataStruct UART2_Rx_Buffer;

void USART2_Init(unsigned int);
void USART2_TxData(USART_DataStruct DataStruct, u16 size);

#endif


