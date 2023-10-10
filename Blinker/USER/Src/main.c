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

u8 temp = 0;//实时温度
u8 humi = 0;//实时空气湿度
u8 soil = 0;//实时土壤湿度
u8 light = 0;//实时光照强度
u8 pwm = 0;//风扇风速

u8 beeptemp = 0;//报警标志位  置3避免多次使能定时器2
u8 pwmpw = 0;//风扇标志位

u8 t = 0;//计数
/*
 * Data[2]:温度
 * Data[3]:空气湿度
 * Data[4]:土壤湿度
 * Data[5]:光照强度
 * Data[6]:风速
 * Data[7]:风扇开关
 * Data[8]:报警开关
 * Data[9]:补光灯开关
 * Data[10]:水泵开关
 * Data[11]:待机
 */
USART_DataStruct UART2_Tx_Buffer = {0xAA, 0x55, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66}; //用于保存串口2接收到的数据

int main(void)
{
    led_init();
    beep_init();
    delay_init();
    EXTIX_Init();
    uart_init(115200);
    PrintfInit(USART1);
    //初始化串口2
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
    while (DHT11_Init()){//DHT11初始化
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
        //按键处理

        //红外处理

        //数据包接收成功进行处理
        if (UART2_Rx_Buffer.Flag == 1){
            //指示灯
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
            //报警处理
            if(beeptemp == 1){
                TIM_Cmd(TIM3, ENABLE);
            } else if (beeptemp == 0){
                TIM_Cmd(TIM3, DISABLE);
                BEEP = 1;
                beeptemp = 0;
            }
            //报警处理
            if(pwmpw == 1){
                TIM_SetCompare2(TIM2,pwm*9);
            } else if (pwmpw == 0){
                TIM_SetCompare2(TIM2,0);
            }
        }


        //打印接受的数据包
//        if(t == 4){
//            for (int i = 0; i < sizeof (UART2_Rx_Buffer.Data); i++) {
//                printf(" %x",UART2_Rx_Buffer.Data[i]);
//            }
//        }
        //每5秒发送一次数据包
        if(t == 40){
            t = 0;
            //闪灯代表数据发送，程序正在运行
            LED0 = !LED0;
            //发送数据包
            USART2_TxData(UART2_Tx_Buffer,13);
        }
    }
}
