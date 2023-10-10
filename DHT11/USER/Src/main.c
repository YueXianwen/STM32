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
#include "dht11.h"

u16 adc_x;
float temp;

u8 t = 0;
u8 temperature;
u8 humidity;

int main(void)
{
    delay_init();
    beep_init();
    led_init();
    key_init();
    uart_init(115200);
    PrintfInit(USART1);
    TIM3_Int_Init(9999, 7199);
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
    while (DHT11_Init()){
        OLED_ShowString(24,2,"DHT11 ERROR!!");
        delay_ms(200);
        OLED_Clear();
        printf("DHT11 ERROR!!\r\n");
    }
    printf("DHT11 OK!!\r\n");
    OLED_Clear();
    OLED_ShowString(12,0,"Hello World!!");
    OLED_ShowString(30,2,"DHT11 OK!!");
    OLED_ShowString(30,4,"Temp:  C");
    OLED_ShowString(30,6,"Humi:  %");
    while(1)
    {
        if (t%10 == 0) {
            DHT11_Read_Data(&temperature,&humidity);
            OLED_ShowNum(70,4,temperature,2,16);
            OLED_ShowNum(70,6,humidity,2,16);
        }
        delay_ms(10);
        t++;
        if (t==20){
            t=0;
            LED0 = !LED0;
        }
    }
}
