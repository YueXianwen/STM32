#ifndef __WIFI_H
#define __WIFI_H

#include "usart3.h"	//gwd 这样包含wifi.h的文件也包含有usart2.h文件夹，这样就可以使用usart2.c中定义的函数

#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)x)  //PA1控制WiFi的复位  

#define WiFi_printf       u3_printf           //串口2控制 WiFi
#define WiFi_RX_BUF       Usart3_RxBuff       //串口2控制 WiFi   串口2接收的缓存数组 1024字节大小
#define WiFi_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //串口2控制 WiFi   接收区缓存大小
#define WiFi_RxCounter    Usart3_RxCounter    //串口2控制 WiFi   记录串口2接收多少字节的数据

#define SSID   "Redmi K30 Pro"                   //路由器SSID名称
#define PASS   "00001111"                           //路由器密码

void WiFi_ResetIO_Init(void);
char WiFi_Reset(int timeout);                //timeout允许的超时时间，100ms的倍数
char WiFi_SendCmd(char *cmd, int timeout);   //发送设置指令

char WiFi_JoinAP(int timeout);	             //加入路由器指令
char WiFi_WaitAP(int timeout);               //等待加入路由器
char WiFi_Connect_Server(int timeout);       //连接TCP服务器指令，并进入到透传模式
char WiFi_Connect_IoTServer(void);           //WiFi连接IOT物联网服务器(子功能处理总函数)

#endif

//后期添加:热点可设置操作，热点可设置！


