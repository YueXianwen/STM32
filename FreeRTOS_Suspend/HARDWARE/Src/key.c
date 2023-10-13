#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

/*
KEY0		PC8
KEY1		PC9
KEY2		PD2
KEYUP       PA0
*/

//初始化函数
void key_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE); //使能GPIO_C 端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//按键处理函数
u8 key_scan(void){
	static u8 key_up = 1;
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0)){
        vTaskDelay(10);
		key_up = 0;
		if(KEY0==0)return KEY0_P;
		else if(KEY1==0)return KEY1_P;
		else if(KEY2==0)return KEY2_P;
	}else if(KEY0==1&&KEY1==1&&KEY2==1) key_up=1;
	return 0;
}
