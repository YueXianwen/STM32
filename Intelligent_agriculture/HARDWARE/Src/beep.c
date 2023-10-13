#include "stm32f10x.h"
#include "beep.h"

void beep_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIO_C �˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //LED0-->PC.0 LED1-->PC.1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO ���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIOC.0 ��GPIOC.1
}


