#include "stm32f10x.h"
#include "timer.h"
#include "led.h"
#include "usart.h"

void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	TIM_Cmd(TIM3, ENABLE);
}

void TIM2_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_Cap_Init(u16 arr, u16 psc)
{
    TIM_ICInitTypeDef TIM2_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM2_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

//void TIM3_IRQHandler(void){
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//		LED3=!LED3;
//        printf("Temp:%d C\r\n", temperature);
//        printf("Humi:%d %%\r\n",humidity);
//	}
//}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//		LED4=!LED4;
//	}
//}

u8 TIM2_STA=0;
u16 TIM2_VAL;
void TIM2_IRQHandler(void){
//    printf("1212");
    if((TIM2_STA&0x80) == 0){
        if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
            if(TIM2_STA&0x40){
                if ((TIM2_STA&0x3f) == 0x3f){
                    TIM2_STA |= 0x80;
                    TIM2_VAL = 0xFFFF;
                } else TIM2_STA++;
            }
        }
        if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET){
            if (TIM2_STA&0x40){
                TIM2_STA |= 0x80;
                TIM2_VAL = TIM_GetCapture1(TIM2);
                TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
            } else{
                TIM2_STA = 0;
                TIM2_VAL = 0;
                TIM_SetCounter(TIM2, 0);
                TIM2_STA |= 0x40;
                TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling);
            }
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);
}

