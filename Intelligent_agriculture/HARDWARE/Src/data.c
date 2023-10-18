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
#include "connect.h"

myData nowData;
myStatus nowStatus;

//����������־λ
static u8 down_long = 1;

//������ܵ�����
u8 remote_num;

//�̶���ͷ��������
char format[]="{\"ack\":\"1\",\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{%s},\"version\":\"1.0.0\"}";
char temp_buf[150];
char temp[1536];

//OLED��ʼ����
void OLED_Start(){
    OLED_Clear();
    OLED_ShowString(32,0,":  '    :  %");
    OLED_ShowString(32,2,":  %    :  H");
    OLED_ShowString(32,4,":       :");
    OLED_ShowString(32,6,":");
    OLED_ShowCHinese(0,0,0);    //��
    OLED_ShowCHinese(16,0,1);   //��
}

/*
 *      �¶ȣ�XX'ʪ�ȣ�XX%
 *      ������XX%���գ�XXH
 *      ˮ�ã��� ���⣺��
 *      ���ȣ��� �Զ����� ������ ��
 */
//OLED��ʼ������
void OLED_Show(){
    OLED_Clear();
    OLED_ShowString(32,0,":  '    :  %");
    OLED_ShowString(32,2,":  %    :  H");
    OLED_ShowString(32,4,":       :");
    OLED_ShowString(32,6,":");
    OLED_ShowCHinese(0,0,0);    //��
    OLED_ShowCHinese(16,0,1);   //��

    OLED_ShowCHinese(64,0,2);   //ʪ
    OLED_ShowCHinese(80,0,3);   //��

    OLED_ShowCHinese(0,2,6);    //��
    OLED_ShowCHinese(16,2,7);   //��

    OLED_ShowCHinese(64,2,4);   //��
    OLED_ShowCHinese(80,2,5);   //��

    OLED_ShowCHinese(0,4,8);    //ˮ
    OLED_ShowCHinese(16,4,9);   //��

    OLED_ShowCHinese(64,4,10);  //��
    OLED_ShowCHinese(80,4,11);  //��

    OLED_ShowCHinese(0,6,12);   //��
    OLED_ShowCHinese(16,6,13);  //��
}

//�������ݻ�ȡ
void Read_Data(){
    DHT11_Read_Data(&nowData.temp,&nowData.humi);
    nowData.light = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_9, 10)) / 4095));
    nowData.soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
}

//OLEDʵʱˢ��
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
    switch (nowStatus.light_num) {
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
    if (nowStatus.Control_PW){
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

//�豸�Զ�����
void Control(){
    //�¶�
    if (nowData.temp <= 28){
        nowStatus.feng_num = 0;
    }else if(nowData.temp > 28 && nowData.temp <= 30) {
        nowStatus.feng_num = 1;
    }else if(nowData.temp > 30 && nowData.temp <= 32){
        nowStatus.feng_num = 2;
    }else if(nowData.temp > 32){
        nowStatus.feng_num = 3;
    }
    //����
    if (nowData.soil >= 40){
        nowStatus.pump_pw = 0;
    } else{
        nowStatus.pump_pw = 1;
    }
    //����
    if (nowData.light >= 40){
        nowStatus.light_num = 0;
    } else if (nowData.light < 40 && nowData.light >= 30){
        nowStatus.light_num = 1;
    } else if (nowData.light < 30 && nowData.light >= 20){
        nowStatus.light_num = 2;
    } else if (nowData.light < 20){
        nowStatus.light_num = 3;
    }
    //����
    if (nowData.temp <= 28 && nowData.soil >= 40){
        nowStatus.beep_pw = 0;
    } else{
        nowStatus.beep_pw = 1;
    }
    LED2 = !nowStatus.pump_pw;
    Shuibeng = nowStatus.pump_pw;
    LED3 = (nowStatus.light_num?0:1);
    Light_PWM(nowStatus.light_num);
    LED4 = (nowStatus.feng_num?0:1);
    Feng_PWM(nowStatus.feng_num);
}

//����
void Beep_Run(){
    if (nowStatus.beep_pw){
        BEEP = !BEEP;
        LED1 = !LED1;
    } else{
        BEEP = 1;
        LED1 = 1;
    }
}

//��ʾ��
void Beep_40ms(){
    BEEP = 0;
    delay_ms(40);
    BEEP = 1;
}

//�������
void Auto_Remote(){
    if (Remote_Scan() && down_long == 0){
        remote_num = Remote_Scan();
        down_long = 1;
    }
    if (!Remote_Scan() && down_long == 1) {
        down_long = 0;
    }
    //ALIENTEK���Զ�����
    if (remote_num == 226){
        Cut_Control();
    }
    if (!nowStatus.Control_PW){
        switch (remote_num) {
            case 0:
                break;
            case 162:           //��Դ������
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
            case 104:       //1:ˮ��
                Manual_Shuibeng();
                break;
            case 152:       //2:����
                Manual_Light();
                break;
            case 176:       //3:����
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
                Manual_Beep();
                break;
            case 82:
                break;
            default:
                break;
        }
    }
    remote_num = 0;     //�������
}

//�ֶ�ˮ��
void Manual_Shuibeng(){
    nowStatus.pump_pw = !nowStatus.pump_pw;
    LED2 = !nowStatus.pump_pw;
    Shuibeng = nowStatus.pump_pw;
    Beep_40ms();
}

//�ֶ�����
void Manual_Light(){
    LED3 = (++nowStatus.light_num!=4?0:1);
    Light_PWM((nowStatus.light_num==4?0:nowStatus.light_num));
    if (nowStatus.light_num == 4) nowStatus.light_num = 0;
    Beep_40ms();
}

//�ֶ�����
void Manual_Feng(){
    LED4 = (++nowStatus.feng_num!=4?0:1);
    Feng_PWM((nowStatus.feng_num==4?0:nowStatus.feng_num));
    if (nowStatus.feng_num == 4) nowStatus.feng_num = 0;
    Beep_40ms();
}

//�ֶ�����
void Manual_Beep(){
    nowStatus.beep_pw = !nowStatus.beep_pw;
}

//�ֶ��Զ��л�
void Cut_Control(){
    nowStatus.Control_PW = !nowStatus.Control_PW;
    LED7 = !LED7;
    //�����豸��ʼ��
    LED2 = 1;LED3 = 1;LED4 = 1;Shuibeng = 0;
    nowStatus.pump_pw = 0;
    nowStatus.light_num = 0;
    nowStatus.feng_num = 0;
    nowStatus.beep_pw = 0;
    Light_PWM(0);
    Feng_PWM(0);
    Beep_40ms();
}

//������Ϣ����
void Publish_Trans(){
    sprintf(temp_buf, "\"temp\":%d,\"humi\":%d,\"soil\":%d,\"light\":%d,\"pump_pw\":%d,\"light_num\":%d,\"feng_num\":%d,\"beep_pw\":%d,\"Control_PW\":%d", nowData.temp, nowData.humi, nowData.soil, nowData.light, nowStatus.pump_pw, nowStatus.light_num, nowStatus.feng_num, nowStatus.beep_pw, nowStatus.Control_PW);
    sprintf(temp,format,temp_buf);
    MQTT_PublishQs0(P_TOPIC_NAME,temp, strlen(temp));
}

//�������ձ���
void Analysis_Subs(){
    if (nowStatus.Control_PW == 0){
        //ˮ��
        if(strstr((char *)MQTT_CMDOutPtr+2,"\"pump_pw\":0")){
            nowStatus.pump_pw = 1;
            Manual_Shuibeng();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"pump_pw\":1")) {
            nowStatus.pump_pw = 0;
            Manual_Shuibeng();
        }
        //�����
        if(strstr((char *)MQTT_CMDOutPtr+2,"\"light_num\":0")){
            nowStatus.light_num = 3;
            Manual_Light();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"light_num\":1")){
            nowStatus.light_num = 0;
            Manual_Light();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"light_num\":2")){
            nowStatus.light_num = 1;
            Manual_Light();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"light_num\":3")){
            nowStatus.light_num = 2;
            Manual_Light();
        }
        //����
        if(strstr((char *)MQTT_CMDOutPtr+2,"\"feng_num\":0")){
            nowStatus.feng_num = 3;
            Manual_Feng();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"feng_num\":1")){
            nowStatus.feng_num = 0;
            Manual_Feng();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"feng_num\":2")){
            nowStatus.feng_num = 1;
            Manual_Feng();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"feng_num\":3")){
            nowStatus.feng_num = 2;
            Manual_Feng();
        }
        //����
        if(strstr((char *)MQTT_CMDOutPtr+2,"\"beep_pw\":0")){
            nowStatus.beep_pw = 0;
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"beep_pw\":1")){
            nowStatus.beep_pw = 1;
        }
    }
    //�Զ���
    if(strstr((char *)MQTT_CMDOutPtr+2,"\"Control_PW\":0")){
        nowStatus.Control_PW = 1;
        LED7 = 0;
        Cut_Control();
    }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"Control_PW\":1")){
        nowStatus.Control_PW = 0;
        LED7 = 1;
        Cut_Control();
    }
}
