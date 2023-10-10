#ifndef _PRINT_UART_H__
#define _PRINT_UART_H__

#include "stm32f10x.h"
#include <stdio.h>

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;
void uart_init(u32 bound);

void PrintfInit(USART_TypeDef *uarTx);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

#endif //#ifndef _PRINT_UART_H__