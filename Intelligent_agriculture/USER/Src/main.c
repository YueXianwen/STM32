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

u8 t = 0;//����

int main(void)
{
    uart_init(115200);
    PrintfInit(USART1);
    delay_init();                   //��ʱ������ʼ��
    led_init();                     //LED��ʼ��
    beep_init();                    //��������ʼ��
    Adc_Init();                     //ADC��ʼ��
    EXTIX_Init();                   //�жϳ�ʼ��
    relay_init();                   //�̵�����ʼ��
    Remote_Init();                  //�����ʼ��
    TIM2_PWM_Init(899, 0);  //PWM��ʼ��
    OLED_Init();                    //OLED��ʼ��
    OLED_Clear();                   //OLED���
    delay_ms(1000);            //��ʱ1��
    DHT11_Init();                   //DHT11��ʼ��

    OLED_show();

    while(1) {
        if (t%50 == 0) LED0 = !LED0;
        if (t%100 == 0) Read_Data();
        if (t%10 == 0) OLED_Refresh();
        t++;
        delay_ms(10);
    }
}
