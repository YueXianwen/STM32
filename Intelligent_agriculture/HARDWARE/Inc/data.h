//
// Created by 36348 on 2023/10/17.
//

#ifndef INTELLIGENT_AGRICULTURE_DATA_H
#define INTELLIGENT_AGRICULTURE_DATA_H

//数据结构体
typedef struct{
    u8 temp;//实时温度
    u8 humi;//实时空气湿度
    u8 soil;//实时土壤湿度
    u8 light;//实时光照强度
} myData;

//状态结构体
typedef struct{
    u8 pump_pw;//水泵开关：0、1
    u8 light_pw;//光源开关：0、1
    u8 feng_num;//风扇速度：0、1、2、3
    u8 beep_pw;//报警状态：0、1
} myStatus;

void OLED_Show();
void Read_Data();
void OLED_Refresh();
void Control();
void Beep_Run();
void Beep_40ms();
void Auto_Remote();

void Manual_Shuibeng();
void Manual_Light();
void Manual_Feng();

#endif //INTELLIGENT_AGRICULTURE_DATA_H
