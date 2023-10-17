//
// Created by 36348 on 2023/10/17.
//

#ifndef INTELLIGENT_AGRICULTURE_DATA_H
#define INTELLIGENT_AGRICULTURE_DATA_H

//���ݽṹ��
typedef struct{
    u8 temp;//ʵʱ�¶�
    u8 humi;//ʵʱ����ʪ��
    u8 soil;//ʵʱ����ʪ��
    u8 light;//ʵʱ����ǿ��
} myData;

//״̬�ṹ��
typedef struct{
    u8 pump_pw;//ˮ�ÿ��أ�0��1
    u8 light_pw;//��Դ���أ�0��1
    u8 feng_num;//�����ٶȣ�0��1��2��3
    u8 beep_pw;//����״̬��0��1
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
