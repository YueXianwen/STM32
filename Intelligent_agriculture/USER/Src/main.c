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

u8 temp = 0;//ʵʱ�¶�
u8 humi = 0;//ʵʱ����ʪ��
u8 soil = 0;//ʵʱ����ʪ��
u8 light = 0;//ʵʱ����ǿ��
u8 pwm = 0;//���ȷ���

u8 beeptemp = 0;//������־λ  ��3������ʹ�ܶ�ʱ��2
u8 pwmpw = 0;//���ȱ�־λ

u8 t = 0;//����

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
    while (DHT11_Init()){//DHT11��ʼ��
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
        //OLED��ʾ
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
        //��������

        //���⴦��

    }
}
