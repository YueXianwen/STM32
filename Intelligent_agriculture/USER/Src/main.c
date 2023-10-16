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
u8 pwmpw = 0;//风扇标志位

u8 t = 0;//计数

//初始化
void init(){
    uart_init(115200);
    PrintfInit(USART1);
    led_init();
    delay_init();
    Adc_Init();
    beep_init();
    EXTIX_Init();
    relay_init();
    TIM2_PWM_Init(899, 0);
    TIM3_Int_Init(9999, 7199);
    LED0 = 1;LED1 = 1;LED2 = 1;LED3 = 1;LED4 = 1;
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
}

//OLED初始化
void show(){
    OLED_ShowString(32,0,":  '    :  %");
    OLED_ShowString(32,2,":  %    :  H");
    OLED_ShowCHinese(0,0,0);
    OLED_ShowCHinese(16,0,1);
    OLED_ShowCHinese(64,0,2);
    OLED_ShowCHinese(80,0,3);
    OLED_ShowCHinese(0,2,4);
    OLED_ShowCHinese(16,2,5);
    OLED_ShowCHinese(64,2,6);
    OLED_ShowCHinese(80,2,7);
}

//环境数据获取
void app1(){
    DHT11_Read_Data(&temp,&humi);
    soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
    light = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_9, 10)) / 4095));
    OLED_ShowNum(40,0,temp,2,16);
    OLED_ShowNum(102,0,humi,2,16);
    OLED_ShowNum(40,2,soil,2,16);
    OLED_ShowNum(102,2,light,2,16);
}

int main(void)
{
    init();
    show();
    while(1) {
        //OLED显示
        if (t%50 == 0) LED0 = !LED0;
        if (t%40 == 0) app1();
        if (t%100 == 0)RGB_PWM1();
        t++;
        delay_ms(10);
    }
}
