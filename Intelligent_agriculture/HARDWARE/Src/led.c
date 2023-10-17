#include "stm32f10x.h"
#include "led.h"

void led_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��GPIO_C �˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //LED0-->PC.0 LED1-->PC.1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO ���ٶ�Ϊ50MHz
	LED0 = 1;LED1 = 1;LED2 = 1;LED3 = 1;LED4 = 1;LED5 = 1;LED6 = 1;LED7 = 1;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��GPIOC.0 ��GPIOC.1
}
