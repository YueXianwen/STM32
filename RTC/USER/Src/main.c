#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "w25qxx.h"
#include "oled.h"
#include "adc.h"
#include "rtc.h"

u16 adc_x;
float temp;

u8 t = 0;

int main(void)
{
    delay_init();
    beep_init();
    led_init();
    key_init();
    uart_init(115200);
    PrintfInit(USART1);
    TIM3_Int_Init(4999, 7199);
    W25QXX_Init();
    OLED_Init();
    Adc_Init();
    RTC_Init();
    OLED_Clear();
    while(W25QXX_ReadID()!=W25Q16){
        printf("W25Q128 Check Failed!!\r\n");
        delay_ms(500);
        printf("Please Check!!\r\n");
        delay_ms(500);
        LED0 = !LED0;
    }
    if(W25QXX_ReadID()==W25Q16){
        printf("W25Q128 OK!!\r\n");
    }
    delay_ms(1000);
    OLED_ShowString(12,0,"Hello World!!");
    while(1)
    {
        if (t != calendar.sec)
        {
            t = calendar.sec;
            OLED_ShowNum(24, 2, calendar.w_year, 4, 16);
            OLED_ShowString(56,2,"-");
            OLED_ShowNum(64, 2, calendar.w_month, 2, 16);
            OLED_ShowString(80,2,"-");
            OLED_ShowNum(88, 2, calendar.w_date, 2, 16);
            switch (calendar.week) {
                case 0:
                    OLED_ShowString(24,4,"Sunday  ");
                    break;
                case 1:
                    OLED_ShowString(24,4,"Monday  ");
                    break;
                case 2:
                    OLED_ShowString(24,4,"Tuesday  ");
                    break;
                case 3:
                    OLED_ShowString(24,4,"Wednesday  ");
                    break;
                case 4:
                    OLED_ShowString(24,4,"Thursday  ");
                    break;
                case 5:
                    OLED_ShowString(24,4,"Friday  ");
                    break;
                case 6:
                    OLED_ShowString(24,4,"Saturday  ");
                    break;
            }
            OLED_ShowNum(24, 6, calendar.hour, 2, 16);
            OLED_ShowString(40,6,":");
            OLED_ShowNum(48, 6, calendar.min, 2, 16);
            OLED_ShowString(64,6,":");
            OLED_ShowNum(72, 6, calendar.sec, 2, 16);
            LED0 = !LED0;
        }
        delay_ms(10);
    }
}
