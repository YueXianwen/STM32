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
#include "w25qxx.h"

myData nowData = {0,0,0,0};
myStatus nowStatus = {0,0,0,0,0,0,0,0,0};

//红外连按标志位
static u8 down_long = 1;

//红外接受的数据
u8 remote_num;

//OLED初始化标志位
u8 OLED_show_ok = 0;

//固定报头及缓存区
char format[]="{\"ack\":\"1\",\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{%s},\"version\":\"1.0.0\"}";
char temp_buf[150];
char temp[1536];

//OLED开始界面
void OLED_Start(){
    OLED_ShowChinese_big(4,0,0);        //智
    OLED_ShowChinese_big(36,0,1);       //慧
    OLED_ShowChinese_big(68,0,2);       //农
    OLED_ShowChinese_big(100,0,3);      //业
    OLED_ShowChinese_big(16,3,6);       //正
    OLED_ShowChinese_big(40,3,7);       //在
    OLED_ShowChinese_big(64,3,8);       //启
    OLED_ShowChinese_big(88,3,9);       //动
    OLED_ShowString(12,6,"WIFI      ....");
    OLED_ShowChinese(44,6,32);          //连
    OLED_ShowChinese(60,6,33);          //接
    OLED_ShowChinese(76,6,34);          //中
}

/*
 *      温度：XX'湿度：XX%
 *      土壤：XX%光照：XXH
 *      水泵：关 补光：关
 *      风扇：关 自动 警告
 */
//OLED初始化界面
void OLED_Show(){
    OLED_Clear();
    OLED_ShowString(32,0,":  '    :  %");
    OLED_ShowString(32,2,":  %    :  H");
    OLED_ShowString(32,4,":       :");
    OLED_ShowString(32,6,":");
    OLED_ShowChinese(0,0,0);    //温
    OLED_ShowChinese(16,0,1);   //度
    OLED_ShowChinese(64,0,2);   //湿
    OLED_ShowChinese(80,0,3);   //度
    OLED_ShowChinese(0,2,6);    //光
    OLED_ShowChinese(16,2,7);   //照
    OLED_ShowChinese(64,2,4);   //土
    OLED_ShowChinese(80,2,5);   //壤
    OLED_ShowChinese(0,4,8);    //水
    OLED_ShowChinese(16,4,9);   //泵
    OLED_ShowChinese(64,4,10);  //补
    OLED_ShowChinese(80,4,11);  //光
    OLED_ShowChinese(0,6,12);   //风
    OLED_ShowChinese(16,6,13);  //扇
}

//环境数据获取
void Read_Data(){
    DHT11_Read_Data(&nowData.temp,&nowData.humi);
    nowData.light = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_9, 10)) / 4095));
    nowData.soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
}

//OLED实时刷新
void OLED_Refresh(){
    if(OLED_show_ok == 0){
        if (Connect_flag == 1){
            OLED_ShowChinese(76,6,35);
            OLED_ShowChinese(92,6,36);
        } else{
            OLED_ShowChinese(76,6,37);
            OLED_ShowChinese(92,6,38);
        }
        delay_ms(3000);
        OLED_Clear();
        delay_ms(100);
        OLED_Show();
        OLED_show_ok = 1;
    }
    if (!nowStatus.threshold_PW){
        OLED_ShowNum(40,0,nowData.temp,2,16);
        OLED_ShowNum(40,2,nowData.soil,2,16);
    } else if (nowStatus.threshold_num == 0){
        OLED_ShowString(40,0,"  ");
        delay_ms(150);
        OLED_ShowNum(40,0,nowStatus.temp_th,2,16);
        OLED_ShowNum(40,2,nowStatus.soil_th,2,16);
        delay_ms(150);
        OLED_ShowString(40,0,"  ");
    } else if (nowStatus.threshold_num == 1){
        OLED_ShowString(40,2,"  ");
        delay_ms(150);
        OLED_ShowNum(40,0,nowStatus.temp_th,2,16);
        OLED_ShowNum(40,2,nowStatus.soil_th,2,16);
        delay_ms(150);
        OLED_ShowString(40,2,"  ");
    }
    OLED_ShowNum(104,0,nowData.humi,2,16);
    OLED_ShowNum(104,2,nowData.light,2,16);
    if (nowStatus.pump_pw == 0){
        OLED_ShowChinese(40,4,15);
    } else{
        OLED_ShowChinese(40,4,14);
    }
    switch (nowStatus.light_num) {
        case 0:
            OLED_ShowChinese(104,4,15);
            break;
        case 1:
            OLED_ShowChinese(104,4,22);
            break;
        case 2:
            OLED_ShowChinese(104,4,23);
            break;
        case 3:
            OLED_ShowChinese(104,4,24);
            break;
        default:
            break;
    }
    switch (nowStatus.feng_num) {
        case 0:
            OLED_ShowChinese(40,6,15);
            break;
        case 1:
            OLED_ShowChinese(40,6,22);
            break;
        case 2:
            OLED_ShowChinese(40,6,23);
            break;
        case 3:
            OLED_ShowChinese(40,6,24);
            break;
        default:
            break;
    }
    if (nowStatus.Control_PW){
        OLED_ShowChinese(60,6,27);
        OLED_ShowChinese(76,6,28);
    } else if (nowStatus.threshold_PW){
        OLED_ShowChinese(60,6,30);
        OLED_ShowChinese(76,6,31);
    } else{
        OLED_ShowChinese(60,6,29);
        OLED_ShowChinese(76,6,28);
    }
    if (nowStatus.beep_pw == 0){
        OLED_ShowChinese(96,6,25);
        OLED_ShowChinese(112,6,26);
    }
    if(nowStatus.beep_pw == 1 && nowStatus.pump_pw == 1){
        OLED_ShowString(96,6,"    ");
        delay_ms(150);
        OLED_ShowChinese(96,6,20);
        OLED_ShowChinese(112,6,21);
        delay_ms(150);
        OLED_ShowString(96,6,"    ");
    }
    if(nowStatus.beep_pw == 1 && nowStatus.feng_num != 0){
        OLED_ShowString(96,6,"    ");
        delay_ms(150);
        OLED_ShowChinese(96,6,18);
        OLED_ShowChinese(112,6,19);
        delay_ms(150);
        OLED_ShowString(96,6,"    ");
    }
}

//设备自动控制
void Control(){
    //温度
    if (nowData.temp <= nowStatus.temp_th){
        nowStatus.feng_num = 0;
    }else if(nowData.temp > nowStatus.temp_th && nowData.temp <= (nowStatus.temp_th + 2)) {
        nowStatus.feng_num = 1;
    }else if(nowData.temp > (nowStatus.temp_th + 2) && nowData.temp <= (nowStatus.temp_th + 4)){
        nowStatus.feng_num = 2;
    }else if(nowData.temp > (nowStatus.temp_th + 4)){
        nowStatus.feng_num = 3;
    }
    //土壤
    if (nowData.soil >= nowStatus.soil_th){
        nowStatus.pump_pw = 0;
    } else{
        nowStatus.pump_pw = 1;
    }
    //光照
    if (nowData.light >= 40){
        nowStatus.light_num = 0;
    } else if (nowData.light < 40 && nowData.light >= 30){
        nowStatus.light_num = 1;
    } else if (nowData.light < 30 && nowData.light >= 20){
        nowStatus.light_num = 2;
    } else if (nowData.light < 20){
        nowStatus.light_num = 3;
    }
    //报警
    if (nowData.temp <= nowStatus.temp_th && nowData.soil >= nowStatus.soil_th){
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
    if (remote_num == 162){
        Cut_Control();
    }
    //设置阈值
    if (remote_num == 226){
        nowStatus.threshold_PW = !nowStatus.threshold_PW;
        LED6 = !LED6;
        Beep_40ms();
    }
    if (!nowStatus.Control_PW && !nowStatus.threshold_PW){
        switch (remote_num) {
            case 104:       //1:水泵
                Manual_Shuibeng();
                break;
            case 152:       //2:补光
                Manual_Light();
                break;
            case 176:       //3:风扇
                Manual_Feng();
                break;
            case 66:        //0:报警
                Manual_Beep();
                break;
            default:
                break;
        }
    }
    if (nowStatus.threshold_PW){
        switch (remote_num) {
            case 98:        //UP
                if (nowStatus.threshold_num == 0){
                    nowStatus.temp_th++;
                    if (nowStatus.temp_th > 50) nowStatus.temp_th = 10;
                } else if (nowStatus.threshold_num == 1){
                    nowStatus.soil_th++;
                    if (nowStatus.soil_th > 90) nowStatus.soil_th = 10;
                }
                Beep_40ms();
                break;
            case 194:       //RIGHT
                nowStatus.threshold_num = !nowStatus.threshold_num;
                Beep_40ms();
                break;
            case 34:        //LEFT
                nowStatus.threshold_num = !nowStatus.threshold_num;
                Beep_40ms();
                break;
            case 168:       //DOWN
                if (nowStatus.threshold_num == 0){
                    nowStatus.temp_th--;
                    if (nowStatus.soil_th < 10) nowStatus.soil_th = 50;
                } else if (nowStatus.threshold_num == 1){
                    nowStatus.soil_th--;
                    if (nowStatus.soil_th < 10) nowStatus.soil_th = 90;
                }
                Beep_40ms();
                break;
            case 2:         //PLAY
                nowStatus.threshold_PW = !nowStatus.threshold_PW;
                LED6 = !LED6;
                Write_Threshold();
                break;
            default:
                break;
        }
    }
    remote_num = 0;     //清除缓存
}

//读取阈值
void Read_Threshold(){
    W25QXX_Read(read_data_w25q16,FLASH_SIZE-20,3);
    if (read_data_w25q16[0] == 102){
        nowStatus.temp_th = read_data_w25q16[1];
        nowStatus.soil_th = read_data_w25q16[2];
    } else{
        nowStatus.temp_th = 28;
        nowStatus.soil_th = 40;
    }
    printf("当前阈值：\r\n空气温度阈值: %d\r\n土壤湿度阈值: %d\r\n", nowStatus.temp_th, nowStatus.soil_th);
}

//修改阈值
void Write_Threshold(){
    read_data_w25q16[0] = 102;
    read_data_w25q16[1] = nowStatus.temp_th;
    read_data_w25q16[2] = nowStatus.soil_th;
    W25QXX_Write(read_data_w25q16,FLASH_SIZE-20,3);
    printf("修改阈值成功！！\r\n空气温度阈值: %d\r\n土壤湿度阈值: %d\r\n", nowStatus.temp_th, nowStatus.soil_th);
    Beep_40ms();
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
    LED3 = (++nowStatus.light_num!=4?0:1);
    Light_PWM((nowStatus.light_num==4?0:nowStatus.light_num));
    if (nowStatus.light_num == 4) nowStatus.light_num = 0;
    Beep_40ms();
}

//手动风扇
void Manual_Feng(){
    LED4 = (++nowStatus.feng_num!=4?0:1);
    Feng_PWM((nowStatus.feng_num==4?0:nowStatus.feng_num));
    if (nowStatus.feng_num == 4) nowStatus.feng_num = 0;
    Beep_40ms();
}

//手动报警
void Manual_Beep(){
    nowStatus.beep_pw = !nowStatus.beep_pw;
}

//手动自动切换
void Cut_Control(){
    nowStatus.Control_PW = !nowStatus.Control_PW;
    LED7 = !LED7;
    //所有设备初始化
    LED2 = 1;LED3 = 1;LED4 = 1;Shuibeng = 0;
    nowStatus.pump_pw = 0;
    nowStatus.light_num = 0;
    nowStatus.feng_num = 0;
    nowStatus.beep_pw = 0;
    Light_PWM(0);
    Feng_PWM(0);
    Beep_40ms();
}

//发布消息报文
void Publish_Trans(){
    sprintf(temp_buf, "\"temp\":%d,\"humi\":%d,\"soil\":%d,\"light\":%d,\"pump_pw\":%d,\"light_num\":%d,\"feng_num\":%d,\"beep_pw\":%d,\"Control_PW\":%d,\"temp_th\":%d,\"soil_th\":%d", nowData.temp, nowData.humi, nowData.soil, nowData.light, nowStatus.pump_pw, nowStatus.light_num, nowStatus.feng_num, nowStatus.beep_pw, nowStatus.Control_PW, nowStatus.temp_th, nowStatus.soil_th);
    sprintf(temp,format,temp_buf);
    MQTT_PublishQs0(P_TOPIC_NAME,temp, strlen(temp));
}

//分析接收报文
void Analysis_Subs(){
    printf("%s",(char *)MQTT_CMDOutPtr);
    if (nowStatus.Control_PW == 0){
        //水泵
        if(strstr((char *)MQTT_CMDOutPtr+2,"\"pump_pw\":0")){
            nowStatus.pump_pw = 1;
            Manual_Shuibeng();
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"pump_pw\":1")) {
            nowStatus.pump_pw = 0;
            Manual_Shuibeng();
        }
        //补光灯
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
        //风扇
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
        //报警
        if(strstr((char *)MQTT_CMDOutPtr+2,"\"beep_pw\":0")){
            nowStatus.beep_pw = 0;
        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"beep_pw\":1")){
            nowStatus.beep_pw = 1;
        }
    }
    //自动化
    if(strstr((char *)MQTT_CMDOutPtr+2,"\"Control_PW\":0")){
        nowStatus.Control_PW = 1;
        LED7 = 0;
        Cut_Control();
    }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"Control_PW\":1")){
        nowStatus.Control_PW = 0;
        LED7 = 1;
        Cut_Control();
    }
    if(strstr((char *)MQTT_CMDOutPtr+2,"\"temp_th\":")) {
        char *ptr = strstr((char *)MQTT_CMDOutPtr+2,"\"temp_th\":");
        if (ptr != NULL){
            sscanf(ptr+10, "%2d", &nowStatus.temp_th);
            Write_Threshold();
        }
    }
    if(strstr((char *)MQTT_CMDOutPtr+2,"\"soil_th\":")) {
        char *ptr = strstr((char *)MQTT_CMDOutPtr+2,"\"soil_th\":");
        if (ptr != NULL){
            sscanf(ptr+10, "%2d", &nowStatus.soil_th);
            Write_Threshold();
        }
    }
}
