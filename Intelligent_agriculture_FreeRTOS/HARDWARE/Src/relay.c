//
// Created by 36348 on 2023/9/19.
//
#include "stm32f10x.h"
#include "relay.h"

void relay_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIO_C �˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //LED0-->PC.0 LED1-->PC.1 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO ���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}