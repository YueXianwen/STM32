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
#include "pwm.h"
#include "exti.h"
#include "relay.h"
#include "remote.h"

u16 adc_x;
float temp;

u8 t = 0;
u8 temperature;
u8 humidity;
extern u8 TIM2_STA;
extern u16 TIM2_VAL;
u32 tempIC;
u8 *hw = 0;
u8 num = 0;
u8 hwtemp = 0;

int main(void)
{
    delay_init();
    beep_init();
    led_init();
    key_init();
    EXTIX_Init();
    uart_init(115200);
    PrintfInit(USART1);
//    TIM2_Cap_Init(65535,71);
//    TIM3_Int_Init(9999, 7199);
    W25QXX_Init();
    OLED_Init();
    Adc_Init();
    RTC_Init();
    TIM2_PWM_Init(899,0);
    relay_init();
    Fengji = 0;
    Shuibeng = 0;
    Remote_Init();
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
    OLED_ShowString(8,0,"Hello World!!");
    OLED_ShowString(0,2,"DHT11");
    OLED_ShowString(0,4,"Tep:  C");
    OLED_ShowString(0,6,"Hui:  %");

    OLED_ShowString(60,2,"Light");
    OLED_ShowString(60,4,"LC:");
    OLED_ShowString(60,6,"LA:");
    while(1)
    {
//        if (TIM2_STA&0x80){
//            tempIC=TIM2_STA&0x3F;
//            tempIC *= 65536;
//            tempIC += TIM2_VAL;
//            printf("HIGH: %d us\r\n", tempIC);
//            TIM2_STA = 0;
//        }
        RGB_PWM2();
        if (!remote_key()){
            hwtemp = 0;
        }
        if (remote_key() && !hwtemp) {
            hw = remote_key();
            num = Remote_Scan();
            hwtemp = 1;
            printf("%s",hw);
            switch (num) {
                case 162:
                    LED1 = 0;
                    Fengji = 1;
                    BEEP = 0;
                    delay_ms(50);
                    BEEP = 1;
                    break;
                case 226:
                    LED1 = 1;
                    Fengji = 0;
                    BEEP = 0;
                    delay_ms(50);
                    BEEP = 1;
                    break;
                default:
                    BEEP = 0;
                    delay_ms(50);
                    BEEP = 1;
                    break;
            }
        }
        if (t%10 == 0) {
            DHT11_Read_Data(&temperature,&humidity);
            OLED_ShowNum(32,4,temperature,2,16);
            OLED_ShowNum(32,6,humidity,2,16);
        }
        if (t%20 == 0) {
            adc_x = Get_Adc_Average(ADC_Channel_9, 10);
            OLED_ShowNum(64+20, 4, adc_x, 4, 16);
            temp = 3.3 * ((double )adc_x / 4095);
            adc_x = (float)adc_x;
            adc_x = temp;
            OLED_ShowNum(64+20, 6, adc_x, 1, 16);
            OLED_ShowString(64+20+8, 6, ".");
            temp -= adc_x;
            temp *=1000;
            OLED_ShowNum(64+20+8+8, 6, temp, 2, 16);
            OLED_ShowString(64+20+8+8+18, 6, "V");
        }
        if (t%50==0){
            adc_x = Get_Adc_Average(ADC_Channel_4, 10);
            temp = 3.3 * ((double )adc_x / 4095);
//            printf("The humdity of soil is %d, %f V \r\n", adc_x,temp);
        }
        delay_ms(10);
        t++;
        if (t==20){
            t=0;
            LED0 = !LED0;
        }
    }
}
