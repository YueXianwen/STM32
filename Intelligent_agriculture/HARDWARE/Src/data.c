//
// Created by 36348 on 2023/10/17.
//

#include "stm32f10x.h"
#include "data.h"
#include "oled.h"
#include "dht11.h"
#include "adc.h"
#include "delay.h"

myData nowData;
myStatus nowStatus;

/*
 *      �¶ȣ�XX'ʪ�ȣ�XX%
 *      ������XX%���գ�XXH
 *      ˮ�ã��� ���⣺��
 *      ���ȣ��� ������ ��
 */
//OLED��ʼ������
void OLED_show(){
    OLED_ShowString(32,0,":  '    :  %");
    OLED_ShowString(32,2,":  %    :  H");
    OLED_ShowString(32,4,":       :   ");
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

//�������ݻ�ȡ,�Լ��ж�
void Read_Data(){
    DHT11_Read_Data(&nowData.temp,&nowData.humi);
    nowData.light = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_9, 10)) / 4095));
    nowData.soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
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
    if (nowData.light >= 30){
        nowStatus.light_pw = 0;
    } else{
        nowStatus.light_pw = 1;
    }

    if (nowData.temp <= 28 && nowData.soil >= 40){
        nowStatus.beep_pw = 0;
    } else{
        nowStatus.beep_pw = 1;
    }
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
    if (nowStatus.light_pw == 0){
        OLED_ShowCHinese(104,4,15);
    } else{
        OLED_ShowCHinese(104,4,14);
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
    if (nowStatus.beep_pw == 0){
        OLED_ShowCHinese(80,6,25);
        OLED_ShowCHinese(96,6,26);
    }
    if(nowStatus.beep_pw == 1 && nowStatus.pump_pw == 1){
        OLED_ShowString(64,6,"        ");
        delay_ms(150);
        OLED_ShowString(64,6," !    ! ");
        OLED_ShowCHinese(80,6,20);
        OLED_ShowCHinese(96,6,21);
        delay_ms(150);
        OLED_ShowString(64,6,"        ");
    }
    if(nowStatus.beep_pw == 1 && nowStatus.feng_num != 0){
        OLED_ShowString(64,6,"        ");
        delay_ms(150);
        OLED_ShowString(64,6," !    ! ");
        OLED_ShowCHinese(80,6,18);
        OLED_ShowCHinese(96,6,19);
        delay_ms(150);
        OLED_ShowString(64,6,"        ");
    }
}
