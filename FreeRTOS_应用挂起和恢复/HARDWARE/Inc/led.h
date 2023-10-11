#ifndef __LED_H
#define __LED_H

#include "sys.h"

void led_init(void);

#define LED0 PCout(0)
#define LED1 PCout(1)
#define LED2 PCout(2)
#define LED3 PCout(3)

#endif
