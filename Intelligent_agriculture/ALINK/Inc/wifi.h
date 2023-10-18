#ifndef __WIFI_H
#define __WIFI_H

#include "usart3.h"	//gwd ��������wifi.h���ļ�Ҳ������usart2.h�ļ��У������Ϳ���ʹ��usart2.c�ж���ĺ���

#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)x)  //PA1����WiFi�ĸ�λ  

#define WiFi_printf       u3_printf           //����2���� WiFi
#define WiFi_RX_BUF       Usart3_RxBuff       //����2���� WiFi   ����2���յĻ������� 1024�ֽڴ�С
#define WiFi_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //����2���� WiFi   �����������С
#define WiFi_RxCounter    Usart3_RxCounter    //����2���� WiFi   ��¼����2���ն����ֽڵ�����

#define SSID   "Redmi K30 Pro"                   //·����SSID����
#define PASS   "00001111"                           //·��������

void WiFi_ResetIO_Init(void);
char WiFi_Reset(int timeout);                //timeout����ĳ�ʱʱ�䣬100ms�ı���
char WiFi_SendCmd(char *cmd, int timeout);   //��������ָ��

char WiFi_JoinAP(int timeout);	             //����·����ָ��
char WiFi_WaitAP(int timeout);               //�ȴ�����·����
char WiFi_Connect_Server(int timeout);       //����TCP������ָ������뵽͸��ģʽ
char WiFi_Connect_IoTServer(void);           //WiFi����IOT������������(�ӹ��ܴ����ܺ���)

#endif

//�������:�ȵ�����ò������ȵ�����ã�


