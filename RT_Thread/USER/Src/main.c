#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "rthw.h"
#include "rtthread.h"

static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t led2_thread = RT_NULL;

static void led1_thread_entry(void *parameter);
static void led2_thread_entry(void *parameter);

int main(void)
{
    led_init();
    uart_init(115200);
    PrintfInit(USART1);
    printf("OK!!!");
    led1_thread = rt_thread_create("led1",
                                   led1_thread_entry,
                                   RT_NULL,
                                   512,
                                   3,
                                   20);

    led2_thread = rt_thread_create("led2",
                                   led2_thread_entry,
                                   RT_NULL,
                                   512,
                                   3,
                                   20);
    if (led1_thread != RT_NULL && led2_thread != RT_NULL){
        rt_thread_startup(led1_thread);
        rt_thread_startup(led2_thread);
    }
    else
        return -1;
}

//线程定义
static void led1_thread_entry(void* parameter){
    while (1){
        LED0 = 0;
        rt_thread_delay(500);
        LED0 = 1;
        rt_thread_delay(500);
    }
}

static void led2_thread_entry(void* parameter){
    while (1){
        LED1 = 0;
        rt_thread_delay(1000);
        LED1 = 1;
        rt_thread_delay(1000);
    }
}