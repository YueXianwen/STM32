#ifndef  __TIMER1_H
#define  __TIMER1_H

#include "sys.h"

void TIM1_Init(unsigned short int arr, unsigned short int psc);
void TIM1_ENABLE_2S(void);
void TIM1_ENABLE_5S(void);
void TIM1_ENABLE_10S(void);
void TIM1_ENABLE_30S(void);

#endif


