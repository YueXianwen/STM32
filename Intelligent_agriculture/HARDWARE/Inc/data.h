//
// Created by 36348 on 2023/10/17.
//

#ifndef INTELLIGENT_AGRICULTURE_DATA_H
#define INTELLIGENT_AGRICULTURE_DATA_H

#define FLASH_SIZE (1<<21)

//数据结构体
typedef struct{
    u8 temp;                //实时空气温度
    u8 humi;                //实时空气湿度
    u8 soil;                //实时土壤湿度
    u8 light;               //实时光照强度
} myData;

//状态结构体
typedef struct{
    u8 pump_pw;             //水泵开关：0、1
    u8 light_num;           //光源开关：0、1、2、3
    u8 feng_num;            //风扇速度：0、1、2、3
    u8 beep_pw;             //报警状态：0、1
    u8 Control_PW;          //自动控制标志位：0、1
    u8 threshold_PW;        //设置模式标志位：0、1
    u8 threshold_num;       //设置那个参数的阈值：0：空气温度、1：土壤湿度
    int temp_th;            //空气温度阈值：10-50
    int soil_th;            //土壤湿度阈值：10-90
} myStatus;

//读取储存数据：0：校验码、1：土壤湿度阈值、2：空气湿度阈值
static u8 read_data_w25q16[3];

void OLED_Start();          //启动界面
void OLED_Show();           //OLED初始化界面
void Read_Data();           //环境数据获取
void OLED_Refresh();        //OLED实时刷新
void Control();             //设备自动控制
void Beep_Run();            //报警
void Beep_40ms();           //提示音
void Auto_Remote();         //红外控制

void Read_Threshold();      //读取阈值
void Write_Threshold();     //修改阈值

void Manual_Shuibeng();     //手动水泵
void Manual_Light();        //手动补光
void Manual_Feng();         //手动风扇
void Manual_Beep();         //手动报警
void Cut_Control();         //手动自动切换

void Publish_Trans();       //发布消息报文
void Analysis_Subs();       //分析接收报文

#endif //INTELLIGENT_AGRICULTURE_DATA_H
