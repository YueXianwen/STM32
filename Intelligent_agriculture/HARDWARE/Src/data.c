//
// Created by 36348 on 2023/10/17.
//
#include "stm32f10x.h"
#include "data.h"
#include "oled.h"
#include "dht11.h"
#include "adc.h"
#include "delay.h"
#include "pwm.h"
#include "relay.h"
#include "beep.h"
#include "led.h"
#include "remote.h"

myData nowData;
myStatus nowStatus;

//自动控制标志位
u8 Control_PW = 0;

//红外连按标志位
static u8 down_long = 1;

//红外接受的数据
u8 remote_num;

/*
 *      温度：XX'湿度：XX%
 *      土壤：XX%光照：XXH
 *      水泵：关 补光：关
 *      风扇：关 自动：关 ！警告 ！
 */
//OLED初始化界面
void OLED_Show(){
    OLED_ShowString(32,0,":  '    :  %");
    OLED_ShowString(32,2,":  %    :  H");
    OLED_ShowString(32,4,":       :");
    OLED_ShowString(32,6,":");
    OLED_ShowCHinese(0,0,0);    //温
    OLED_ShowCHinese(16,0,1);   //度
    OLED_ShowCHinese(64,0,2);   //湿
    OLED_ShowCHinese(80,0,3);   //度

    OLED_ShowCHinese(0,2,6);    //光
    OLED_ShowCHinese(16,2,7);   //照
    OLED_ShowCHinese(64,2,4);   //土
    OLED_ShowCHinese(80,2,5);   //壤

    OLED_ShowCHinese(0,4,8);    //水
    OLED_ShowCHinese(16,4,9);   //泵
    OLED_ShowCHinese(64,4,10);  //补
    OLED_ShowCHinese(80,4,11);  //光

    OLED_ShowCHinese(0,6,12);   //风
    OLED_ShowCHinese(16,6,13);  //扇
}

//环境数据获取
void Read_Data(){
    DHT11_Read_Data(&nowData.temp,&nowData.humi);
    nowData.light = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_9, 10)) / 4095));
    nowData.soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
}

//OLED实时刷新
void OLED_Refresh(){
    OLED_ShowNum(40,0,nowData.temp,2,16);
    OLED_ShowNum(104,0,nowData.humi,2,16);
    OLED_ShowNum(40,2,nowData.soil,2,16);
    OLED_ShowNum(104,2,nowData.light,2,16);
    if (nowStatus.pump_pw == 0){
        OLED_ShowCHinese(40,4,15);
    } else{
        OLED_ShowCHinese(40,4,14);
    }
    switch (nowStatus.light_pw) {
        case 0:
            OLED_ShowCHinese(104,4,15);
            break;
        case 1:
            OLED_ShowCHinese(104,4,22);
            break;
        case 2:
            OLED_ShowCHinese(104,4,23);
            break;
        case 3:
            OLED_ShowCHinese(104,4,24);
            break;
        default:
            break;
    }
    switch (nowStatus.feng_num) {
        case 0:
            OLED_ShowCHinese(40,6,15);
            break;
        case 1:
            OLED_ShowCHinese(40,6,22);
            break;
        case 2:
            OLED_ShowCHinese(40,6,23);
            break;
        case 3:
            OLED_ShowCHinese(40,6,24);
            break;
        default:
            break;
    }
    if (Control_PW){
        OLED_ShowCHinese(60,6,27);
        OLED_ShowCHinese(76,6,28);
    } else{
        OLED_ShowCHinese(60,6,29);
        OLED_ShowCHinese(76,6,28);
    }
    if (nowStatus.beep_pw == 0){
        OLED_ShowCHinese(96,6,25);
        OLED_ShowCHinese(112,6,26);
    }
    if(nowStatus.beep_pw == 1 && nowStatus.pump_pw == 1){
        OLED_ShowString(96,6,"    ");
        delay_ms(150);
        OLED_ShowCHinese(96,6,20);
        OLED_ShowCHinese(112,6,21);
        delay_ms(150);
        OLED_ShowString(96,6,"    ");
    }
    if(nowStatus.beep_pw == 1 && nowStatus.feng_num != 0){
        OLED_ShowString(96,6,"    ");
        delay_ms(150);
        OLED_ShowCHinese(96,6,18);
        OLED_ShowCHinese(112,6,19);
        delay_ms(150);
        OLED_ShowString(96,6,"    ");
    }
}

//设备自动控制
void Control(){
    //温度
    if (nowData.temp <= 28){
        nowStatus.feng_num = 0;
    }else if(nowData.temp > 28 && nowData.temp <= 30) {
        nowStatus.feng_num = 1;
    }else if(nowData.temp > 30 && nowData.temp <= 32){
        nowStatus.feng_num = 2;
    }else if(nowData.temp > 32){
        nowStatus.feng_num = 3;
    }
    //土壤
    if (nowData.soil >= 40){
        nowStatus.pump_pw = 0;
    } else{
        nowStatus.pump_pw = 1;
    }
    //光照
    if (nowData.light >= 40){
        nowStatus.light_pw = 0;
    } else if (nowData.light < 40 && nowData.light >= 30){
        nowStatus.light_pw = 1;
    } else if (nowData.light < 30 && nowData.light >= 20){
        nowStatus.light_pw = 2;
    } else if (nowData.light < 20){
        nowStatus.light_pw = 3;
    }
    //报警
    if (nowData.temp <= 28 && nowData.soil >= 40){
        nowStatus.beep_pw = 0;
    } else{
        nowStatus.beep_pw = 1;
    }
    LED2 = !nowStatus.pump_pw;
    Shuibeng = nowStatus.pump_pw;
    LED3 = (nowStatus.light_pw?0:1);
    Light_PWM(nowStatus.light_pw);
    LED4 = (nowStatus.feng_num?0:1);
    Feng_PWM(nowStatus.feng_num);
}

//报警
void Beep_Run(){
    if (nowStatus.beep_pw){
        BEEP = !BEEP;
        LED1 = !LED1;
    } else{
        BEEP = 1;
        LED1 = 1;
    }
}

//提示音
void Beep_40ms(){
    BEEP = 0;
    delay_ms(40);
    BEEP = 1;
}

//红外控制
void Auto_Remote(){
    if (Remote_Scan() && down_long == 0){
        remote_num = Remote_Scan();
        down_long = 1;
    }
    if (!Remote_Scan() && down_long == 1) {
        down_long = 0;
    }
    //ALIENTEK：自动控制
    if (remote_num == 226){
        Control_PW = !Control_PW;
        LED7 = !LED7;
        //所有设备初始化
        LED2 = 1;LED3 = 1;LED4 = 1;Shuibeng = 0;
        nowStatus.pump_pw = 0;
        nowStatus.light_pw = 0;
        nowStatus.feng_num = 0;
        nowStatus.beep_pw = 0;
        Light_PWM(0);
        Feng_PWM(0);
        Beep_40ms();
    }
    if (!Control_PW){
        switch (remote_num) {
            case 0:
                break;
            case 162:           //电源：休眠
                break;
            case 98:
                break;
            case 2:
                break;
            case 194:
                break;
            case 34:
                break;
            case 224:
                break;
            case 168:
                break;
            case 144:
                break;
            case 104:       //1:水泵
                Manual_Shuibeng();
                break;
            case 152:       //2:补光
                Manual_Light();
                break;
            case 176:       //3:风扇
                Manual_Feng();
                break;
            case 48:
                break;
            case 24:
                break;
            case 122:
                break;
            case 16:
                break;
            case 56:
                break;
            case 90:
                break;
            case 66:
                break;
            case 82:
                break;
            default:
                break;
        }
    }
    remote_num = 0;     //清除缓存
}

//手动水泵
void Manual_Shuibeng(){
    nowStatus.pump_pw = !nowStatus.pump_pw;
    LED2 = !nowStatus.pump_pw;
    Shuibeng = nowStatus.pump_pw;
    Beep_40ms();
}

//手动补光
void Manual_Light(){
    LED3 = (++nowStatus.light_pw!=4?0:1);
    Light_PWM((nowStatus.light_pw==4?0:nowStatus.light_pw));
    if (nowStatus.light_pw == 4) nowStatus.light_pw = 0;
    Beep_40ms();
}

//手动风扇
void Manual_Feng(){
    LED4 = (++nowStatus.feng_num!=4?0:1);
    Feng_PWM((nowStatus.feng_num==4?0:nowStatus.feng_num));
    if (nowStatus.feng_num == 4) nowStatus.feng_num = 0;
    Beep_40ms();
}


