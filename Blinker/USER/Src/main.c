#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
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
/*
 * Data[2]:�¶�
 * Data[3]:����ʪ��
 * Data[4]:����ʪ��
 * Data[5]:����ǿ��
 * Data[6]:����
 * Data[7]:���ȿ���
 * Data[8]:��������
 * Data[9]:����ƿ���
 * Data[10]:ˮ�ÿ���
 * Data[11]:����
 */
USART_DataStruct UART2_Tx_Buffer = {0xAA, 0x55, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66}; //���ڱ��洮��2���յ�������

int main(void)
{
    led_init();
    beep_init();
    delay_init();
    EXTIX_Init();
    uart_init(115200);
    PrintfInit(USART1);
    //��ʼ������2
    USART2_Init(115200);
    TIM2_PWM_Init(899,0);
    TIM3_Int_Init(999,7199);
    relay_init();
    BEEP = 1;
    LIGHT = 0;
    Shuibeng = 0;
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
        if (t%40 == 0) {
            DHT11_Read_Data(&temp,&humi);
            soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
            light = 100-(Get_Adc_Average(ADC_Channel_9,10)>>6);
            OLED_ShowNum(32,4,temp,2,16);
            OLED_ShowNum(96,4,soil,2,16);
            OLED_ShowNum(32,6,humi,2,16);
            OLED_ShowNum(96,6,light,2,16);
            UART2_Tx_Buffer.Data[2] = temp;
            UART2_Tx_Buffer.Data[3] = humi;
            UART2_Tx_Buffer.Data[4] = soil;
            UART2_Tx_Buffer.Data[5] = light;
        }
        delay_ms(10);
        t++;
        //��������

        //���⴦��

        //���ݰ����ճɹ����д���
        if (UART2_Rx_Buffer.Flag == 1){
            //ָʾ��
            LED1 = !UART2_Rx_Buffer.Data[3];
            LED2 = !UART2_Rx_Buffer.Data[4];
            LED3 = !UART2_Rx_Buffer.Data[5];
            LED4 = !UART2_Rx_Buffer.Data[6];

            pwm = UART2_Rx_Buffer.Data[2];
            pwmpw = UART2_Rx_Buffer.Data[3];
            beeptemp = UART2_Rx_Buffer.Data[4];
            LIGHT = UART2_Rx_Buffer.Data[5];
            Shuibeng = UART2_Rx_Buffer.Data[6];

            UART2_Rx_Buffer.Flag = 0;
            for(int i = 0; i < 6; i ++)UART2_Tx_Buffer.Data[i+6] = UART2_Rx_Buffer.Data[i+2];
            for(int i = 0; i < 20; i ++)UART2_Rx_Buffer.Data[i] = 0;
            //��������
            if(beeptemp == 1){
                TIM_Cmd(TIM3, ENABLE);
            } else if (beeptemp == 0){
                TIM_Cmd(TIM3, DISABLE);
                BEEP = 1;
                beeptemp = 0;
            }
            //��������
            if(pwmpw == 1){
                TIM_SetCompare2(TIM2,pwm*9);
            } else if (pwmpw == 0){
                TIM_SetCompare2(TIM2,0);
            }
        }


        //��ӡ���ܵ����ݰ�
//        if(t == 4){
//            for (int i = 0; i < sizeof (UART2_Rx_Buffer.Data); i++) {
//                printf(" %x",UART2_Rx_Buffer.Data[i]);
//            }
//        }
        //ÿ5�뷢��һ�����ݰ�
        if(t == 40){
            t = 0;
            //���ƴ������ݷ��ͣ�������������
            LED0 = !LED0;
            //�������ݰ�
            USART2_TxData(UART2_Tx_Buffer,13);
        }
    }
}
