#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

int main(void)
{
    uart_init(115200);
    PrintfInit(USART1);
	led_init();
	while(1) {

    }
}
