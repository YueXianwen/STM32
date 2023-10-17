//
// Created by 36348 on 2023/9/18.
//
#include "stm32f10x.h"
#include "pwm.h"

void TIM2_PWM_Init(u16 arr, u16 psc) {

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PolarityConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PolarityConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);

    TIM_SetCompare2(TIM2, 0);
    TIM_SetCompare3(TIM2, 0);
}

void Feng_PWM(u8 speed){
    switch (speed) {
        case 0:
            TIM_SetCompare2(TIM2, 0);
            break;
        case 1:
            TIM_SetCompare2(TIM2, 500);
            break;
        case 2:
            TIM_SetCompare2(TIM2, 700);
            break;
        case 3:
            TIM_SetCompare2(TIM2, 900);
            break;
        default:
            break;
    }
}

void Light_PWM(u8 speed){
    switch (speed) {
        case 0:
            TIM_SetCompare3(TIM2, 0);
            break;
        case 1:
            TIM_SetCompare3(TIM2, 300);
            break;
        case 2:
            TIM_SetCompare3(TIM2, 600);
            break;
        case 3:
            TIM_SetCompare3(TIM2, 900);
            break;
        default:
            break;
    }
}