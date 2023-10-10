//
// Created by 36348 on 2023/10/8.
//

#ifndef RTOS_BOARD_H
#define RTOS_BOARD_H

#include "stm32f10x.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif //RTOS_BOARD_H
