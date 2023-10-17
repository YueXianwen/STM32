#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "timer.h"
#include "adc.h"
#include "oled.h"
#include "dht11.h"
#include "pwm.h"
#include "relay.h"
#include "exti.h"
#include "remote.h"
#include "data.h"

u8 t = 0;//计数

int main(void)
{
    uart_init(115200);
    PrintfInit(USART1);
    delay_init();                   //延时函数初始化
    led_init();                     //LED初始化
    beep_init();                    //蜂鸣器初始化
    Adc_Init();                     //ADC初始化
    EXTIX_Init();                   //中断初始化
    relay_init();                   //继电器初始化
    Remote_Init();                  //红外初始化
    TIM2_PWM_Init(899, 0);  //PWM初始化
    OLED_Init();                    //OLED初始化
    OLED_Clear();                   //OLED清除
    delay_ms(1000);            //延时1秒
    DHT11_Init();                   //DHT11初始化

    OLED_show();

    while(1) {
        if (t%50 == 0) LED0 = !LED0;
        if (t%100 == 0) Read_Data();
        if (t%10 == 0) OLED_Refresh();
        t++;
        delay_ms(10);
    }
}
