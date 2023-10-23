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

u8 t = 0;//计数
extern myData nowData;
extern myStatus nowStatus;

int main(void)
{
    uart_init(115200);       //串口初始化
    PrintfInit(USART1);       //输出函数初始化
    delay_init();                   //延时函数初始化
    led_init();                     //LED初始化
    beep_init();                    //蜂鸣器初始化
    Adc_Init();                     //ADC初始化
    EXTIX_Init();                   //中断初始化(按键)
    relay_init();                   //继电器初始化
    Remote_Init();                  //红外初始化
    TIM2_PWM_Init(899, 0);  //PWM初始化
    OLED_Init();                    //OLED初始化
    OLED_Clear();                   //OLED清除
    delay_ms(1000);            //延时1秒
    W25QXX_Init();                  //W25Q16初始化
    DHT11_Init();                   //DHT11初始化
    Alink_Init();                   //Alink初始化
    OLED_Start();                   //启动界面
    Beep_40ms();

    while(1) {
        if (t%50 == 0) LED0 = !LED0;                                //程序运行指示灯
        if (t%2 == 0) connect_deal();                               //连接处理
        if (t%10 == 0) OLED_Refresh();                              //OLED刷新
        if (t%100 == 0) Read_Data();                                //数据读取
        if (t%50 == 0 && nowStatus.Control_PW == 1) Control();      //自动控制
        if (t%10 == 0) Beep_Run();                                  //报警处理
        if (Remote_Scan()) Auto_Remote();                           //红外处理
        if (t%100 == 0 && ConnectPack_flag == 1) Publish_Trans();   //发布消息报文
        t++;
        delay_ms(10);
    }
}
