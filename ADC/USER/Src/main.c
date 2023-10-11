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

u16 adc_x;
float temp;

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
    OLED_ShowString(0,0,"Hello World!!");
    OLED_ShowNum(0,2,2023091214,10,16);
    while(1)
    {
        adc_x = Get_Adc_Average(ADC_Channel_9, 10);
        OLED_ShowNum(0, 4, adc_x, 4, 16);
        temp = 3.3 * ((double )adc_x / 4095);
        adc_x = (float)adc_x;
        adc_x = temp;
        OLED_ShowNum(0, 6, adc_x, 1, 16);
        OLED_ShowString(10, 6, ".");
        temp -= adc_x;
        temp *=1000;
        OLED_ShowNum(16, 6, temp, 3, 16);
        OLED_ShowString(40, 6, "V");
        LED0 = !LED0;
        delay_ms(100);
    }
}
