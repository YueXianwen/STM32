#include "delay.h"
#include "key.h"
#include "exti.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t LED0_Task_Handle;

void EXTIX_Init(void){
	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	key_init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
//	EXTI_Init(&EXTI_InitStructure);
//
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
}


void EXTI0_IRQHandler(void){
	delay_xms(10);
    BaseType_t xYieldRequired;
    if (KEYUP == 1){
        xYieldRequired = xTaskResumeFromISR(LED0_Task_Handle);
        printf("(÷–∂œ÷–)Task1 Resume!!\r\n");
    }
    if (xYieldRequired == pdTRUE){
        portYIELD_FROM_ISR(xYieldRequired);
    }
	EXTI_ClearITPendingBit(EXTI_Line0);
}

//void EXTI2_IRQHandler(void){
//
//	delay_ms(10);
//	if(KEY2==0){
//		LED2 = 0;
//	}
//	EXTI_ClearITPendingBit(EXTI_Line2);
//}
//
//void EXTI9_5_IRQHandler(void){
//
//	delay_ms(10);
//	if(KEY0==0){
//		LED0 = 0;
//		EXTI_ClearITPendingBit(EXTI_Line8);
//	}
//	if(KEY1==0){
//		LED1 = 0;
//		EXTI_ClearITPendingBit(EXTI_Line9);
//	}
//}
