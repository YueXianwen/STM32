#ifndef __BEEP_H
#define __BEEP_H

#include "sys.h"

#define BEEP PBout(8)

#define BEEP_IN_STA GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)

void beep_init(void);

#endif
