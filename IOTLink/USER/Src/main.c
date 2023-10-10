#include "stm32f10x.h"

#include "led.h"
#include "delay.h"
#include "usart.h"
#include "connect.h"
#include "dht11.h"
#include "light.h"
#include "adc.h"
#include "beep.h"
#include "pwm.h"

char format[]="{\"ack\":\"0\",\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{%s},\"version\":\"1.0.0\"}";
char temp_buf[96];
char temp[1536];

u8 temperature=0;
u8 humidity=0;
u16 light=0;
float soil=0;

u16 adcx=0;
u8 t = 0;

int main(void)
{
    led_init ();
    beep_init();
    delay_init();
    Adc_Init();
    uart_init(115200);
    PrintfInit(USART1);

    Usart2_Init (115200);
    WiFi_ResetIO_Init ();
    MQTT_Buff_Init ();
    AliIoT_Parameter_Init ();

    TIM1_Init (20,1440);
    //TIM2_PWM_Init(899,0);
    TIM4_Init (299,7199);

    BEEP = 1;

//    while(DHT11_Init())
//    {
//        printf ("DHT11 Error!\r\n");
//    }
//    printf ("DHT11 is OK!\r\n");

    while(1)
    {
        t++;
        delay_ms(20);
        if(t%100==0){
            LED2=!LED2;
            printf("The weather has cooled down.\r\n");
        }

        if(t%100==0){
            t=0;
            DHT11_Read_Data (&temperature ,&humidity );

            adcx=Get_Adc_Average (ADC_Channel_9,10 );
            light=20000-(u16)(20000.0*((float)adcx/4095));

            adcx=Get_Adc_Average (ADC_Channel_4,10 );
            soil=100-(100*((float)adcx/4095));

            if(ConnectPack_flag ==1)
            {
                sprintf (temp_buf,"\"temperature\":%d,\"humidity\":%d,\"light\":%d,\"soil\":%f",temperature ,humidity,light,soil);
                sprintf (temp,format,temp_buf);
                MQTT_PublishQs0 (P_TOPIC_NAME ,temp,strlen(temp)); //�жϿ���״̬����������������
            }
        }
        if(t%2==0) {
            if (ConnectPack_flag == 1) {
                if (MQTT_CMDOutPtr != MQTT_CMDInPtr)                     //if�����Ļ���˵��������ջ�������������
                {
                    printf("����:%s\r\n", &MQTT_CMDOutPtr[2]);           //���������Ϣ

                    if (strstr((char *) MQTT_CMDOutPtr + 2,"\"LightSwitch\":1")) {           //���������"params":{"PowerSwitch":1}˵���������·��򿪿���1
                        LED0 = 0;                                                                    //��LED1
                        LED0_State();                                                              //�жϿ���״̬����������������
                    } else if (strstr((char *) MQTT_CMDOutPtr + 2,"{\"LightSwitch\":0")) {   //���������"params":{"PowerSwitch":0}˵���������·��رտ���1
                        LED0 = 1;                                                                    //�ر�LED1
                        LED0_State();                                                              //�жϿ���״̬����������������
                    }

                    if (strstr((char *) MQTT_CMDOutPtr + 2,"\"BEEP\":1")) {           //���������"params":{"PowerSwitch":1}˵���������·��򿪿���1
                        BEEP = 0;                                                                    //��LED1
                        BEEP_State();                                                              //�жϿ���״̬����������������
                    } else if (strstr((char *) MQTT_CMDOutPtr + 2,"\"BEEP\":0")) {   //���������"params":{"PowerSwitch":0}˵���������·��رտ���1
                        BEEP = 1;                                                                    //�ر�LED1
                        BEEP_State();                                                              //�жϿ���״̬����������������
                    }

                    MQTT_CMDOutPtr += BUFF_UNIT;                        //ָ������
                    if (MQTT_CMDOutPtr == MQTT_CMDEndPtr)                //���ָ�뵽������β����
                        MQTT_CMDOutPtr = MQTT_CMDBuf[0];                //ָ���λ����������ͷ
                }
            }
        }
    }
}






