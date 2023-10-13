#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY0    PCin(8)
#define KEY1    PCin(9)
#define KEY2    PDin(2)
#define KEY_UP  PAin(0)

#define KEY0_P  1
#define KEY1_P  2
#define KEY2_P  3
#define KEYUP_P 3

void key_init(void);
u8 key_scan(void);

#endif
