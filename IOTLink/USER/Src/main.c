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
                MQTT_PublishQs0 (P_TOPIC_NAME ,temp,strlen(temp)); //判断开关状态，并发布给服务器
            }
        }
        if(t%2==0) {
            if (ConnectPack_flag == 1) {
                if (MQTT_CMDOutPtr != MQTT_CMDInPtr)                     //if成立的话，说明命令接收缓冲区有数据了
                {
                    printf("命令:%s\r\n", &MQTT_CMDOutPtr[2]);           //串口输出信息

                    if (strstr((char *) MQTT_CMDOutPtr + 2,"\"LightSwitch\":1")) {           //如果搜索到"params":{"PowerSwitch":1}说明服务器下发打开开关1
                        LED0 = 0;                                                                    //打开LED1
                        LED0_State();                                                              //判断开关状态，并发布给服务器
                    } else if (strstr((char *) MQTT_CMDOutPtr + 2,"{\"LightSwitch\":0")) {   //如果搜索到"params":{"PowerSwitch":0}说明服务器下发关闭开关1
                        LED0 = 1;                                                                    //关闭LED1
                        LED0_State();                                                              //判断开关状态，并发布给服务器
                    }

                    if (strstr((char *) MQTT_CMDOutPtr + 2,"\"BEEP\":1")) {           //如果搜索到"params":{"PowerSwitch":1}说明服务器下发打开开关1
                        BEEP = 0;                                                                    //打开LED1
                        BEEP_State();                                                              //判断开关状态，并发布给服务器
                    } else if (strstr((char *) MQTT_CMDOutPtr + 2,"\"BEEP\":0")) {   //如果搜索到"params":{"PowerSwitch":0}说明服务器下发关闭开关1
                        BEEP = 1;                                                                    //关闭LED1
                        BEEP_State();                                                              //判断开关状态，并发布给服务器
                    }

                    MQTT_CMDOutPtr += BUFF_UNIT;                        //指针下移
                    if (MQTT_CMDOutPtr == MQTT_CMDEndPtr)                //如果指针到缓冲区尾部了
                        MQTT_CMDOutPtr = MQTT_CMDBuf[0];                //指针归位到缓冲区开头
                }
            }
        }
    }
}






