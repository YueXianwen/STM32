#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tx_api.h"

uint8_t task1_buff[1024];
uint8_t task2_buff[1024];

TX_THREAD task1_thread,task2_thread;

void task1_thread_entry(ULONG thread_input)
{
    while(1)
    {
        printf ("Task1 is running!\r\n");
        LED0 = !LED0;
        tx_thread_sleep(300);
    }
}

void task2_thread_entry(ULONG thread_input)
{
    while(1)
    {
        printf ("Task2 is running!\r\n");
        LED1 = !LED1;
        tx_thread_sleep(3000);
    }
}

void tx_application_define(void *first_unused_memory)
{
    tx_thread_create(&task1_thread,"task1 Thread",
                     task1_thread_entry,0x1234,task1_buff,1024,3,3,TX_NO_TIME_SLICE,TX_AUTO_START);
    tx_thread_create(&task2_thread,"task2 Thread",
                     task2_thread_entry,0x1234,task2_buff,1024,4,3,TX_NO_TIME_SLICE,TX_AUTO_START);
}

int main(void)
{
    delay_init();
    led_init();
    uart_init(115200);
    PrintfInit(USART1);

    tx_kernel_enter();

	while(1) {

    }
}
