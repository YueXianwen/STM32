#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "adc.h"
#include "oled.h"
#include "dht11.h"
#include "pwm.h"
#include "relay.h"
#include "exti.h"
#include "remote.h"
#include "data.h"
#include "connect.h"
#include "w25qxx.h"

u8 t = 0;//����
extern myData nowData;
extern myStatus nowStatus;

int main(void)
{
    uart_init(115200);       //���ڳ�ʼ��
    PrintfInit(USART1);       //���������ʼ��
    delay_init();                   //��ʱ������ʼ��
    led_init();                     //LED��ʼ��
    beep_init();                    //��������ʼ��
    Adc_Init();                     //ADC��ʼ��
    EXTIX_Init();                   //�жϳ�ʼ��(����)
    relay_init();                   //�̵�����ʼ��
    Remote_Init();                  //�����ʼ��
    TIM2_PWM_Init(899, 0);  //PWM��ʼ��
    OLED_Init();                    //OLED��ʼ��
    OLED_Clear();                   //OLED���
    delay_ms(1000);            //��ʱ1��
    W25QXX_Init();                  //W25Q16��ʼ��
    DHT11_Init();                   //DHT11��ʼ��
    Alink_Init();                   //Alink��ʼ��
    OLED_Start();                   //��������
    Beep_40ms();

    while(1) {
        if (t%50 == 0) LED0 = !LED0;                                //��������ָʾ��
        if (t%2 == 0) connect_deal();                               //���Ӵ���
        if (t%10 == 0) OLED_Refresh();                              //OLEDˢ��
        if (t%100 == 0) Read_Data();                                //���ݶ�ȡ
        if (t%50 == 0 && nowStatus.Control_PW == 1) Control();      //�Զ�����
        if (t%10 == 0) Beep_Run();                                  //��������
        if (Remote_Scan()) Auto_Remote();                           //���⴦��
        if (t%100 == 0 && ConnectPack_flag == 1) Publish_Trans();   //������Ϣ����
        t++;
        delay_ms(10);
    }
}
