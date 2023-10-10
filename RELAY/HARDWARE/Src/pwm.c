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

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
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
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PolarityConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PolarityConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PolarityConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PolarityConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);
}

u16 ledR = 900;
u16 ledG = 0;
u16 ledB = 0;
u8 dirR = 1;
u8 dirG = 0;
u8 dirB = 0;
u8 degree = 1;

void RGB_PWM1(void){
    if(dirR)ledR+=degree;
    else ledR-=degree;
    if (ledR>900)dirR = 0;
    if (ledR==0)dirR = 1;
    TIM_SetCompare2(TIM2, ledR);

    if(dirG)ledG+=degree;
    else ledG-=degree;
    if (ledG>900)dirG = 0;
    if (ledG==0)dirG = 1;
    TIM_SetCompare3(TIM2, ledG);

    if(dirB)ledB+=degree;
    else ledB-=degree;
    if (ledB>900)dirB = 0;
    if (ledB==0)dirB = 1;
    TIM_SetCompare4(TIM2, ledB);
}

void RGB_PWM2(void){
    if(dirR) {
        ledG+=degree;
        ledR-=degree;
        if (ledR == 0) {
            dirR = 0;
            dirG = 1;
        }
    }
    if(dirG) {
        ledB+=degree;
        ledG-=degree;
        if (ledG == 0) {
            dirG = 0;
            dirB = 1;
        }
    }
    if(dirB) {
        ledR+=degree;
        ledB-=degree;
        if (ledB == 0) {
            dirB = 0;
            dirR = 1;
        }
    }
    TIM_SetCompare2(TIM2, ledR);
    TIM_SetCompare3(TIM2, ledG);
    TIM_SetCompare4(TIM2, ledB);
}

void RGB_PWM3(u16 x){
    TIM_SetCompare1(TIM2, x);
}