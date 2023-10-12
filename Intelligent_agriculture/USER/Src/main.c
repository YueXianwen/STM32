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

u8 temp = 0;//实时温度
u8 humi = 0;//实时空气湿度
u8 soil = 0;//实时土壤湿度
u8 light = 0;//实时光照强度
u8 pwm = 0;//风扇风速

u8 beeptemp = 0;//报警标志位  置3避免多次使能定时器2
u8 pwmpw = 0;//风扇标志位

u8 t = 0;//计数

int main(void)
{
    led_init();
    delay_init();
    uart_init(115200);
    PrintfInit(USART1);
    Adc_Init();
    OLED_Init();
    OLED_Clear();
    delay_ms(1000);
    while (DHT11_Init()){//DHT11初始化
        OLED_ShowString(24,2,"DHT11 ERROR!!");
        delay_ms(200);
        OLED_Clear();
        printf("DHT11 ERROR!!\r\n");
    }
    printf("DHT11 OK!!\r\n");
    OLED_ShowString(0,0,"Hello STM32!!");
    OLED_ShowString(0,2,"DHT11 OK!!");
    OLED_ShowString(0,4,"Te:   C Sl:   %");
    OLED_ShowString(0,6,"Hm:   % Lt:   H");
    while(1) {
        //OLED显示
        if (t%40 == 0) {
            DHT11_Read_Data(&temp,&humi);
            soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
            light = 100-(Get_Adc_Average(ADC_Channel_9,10)>>6);
            OLED_ShowNum(32,4,temp,2,16);
            OLED_ShowNum(96,4,soil,2,16);
            OLED_ShowNum(32,6,humi,2,16);
            OLED_ShowNum(96,6,light,2,16);
        }
        delay_ms(10);
        t++;
        //按键处理

        //红外处理

    }
}
