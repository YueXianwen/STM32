//
// Created by 36348 on 2023/10/17.
//

#ifndef INTELLIGENT_AGRICULTURE_DATA_H
#define INTELLIGENT_AGRICULTURE_DATA_H

#define FLASH_SIZE (1<<21)

//���ݽṹ��
typedef struct{
    u8 temp;                //ʵʱ�����¶�
    u8 humi;                //ʵʱ����ʪ��
    u8 soil;                //ʵʱ����ʪ��
    u8 light;               //ʵʱ����ǿ��
} myData;

//״̬�ṹ��
typedef struct{
    u8 pump_pw;             //ˮ�ÿ��أ�0��1
    u8 light_num;           //��Դ���أ�0��1��2��3
    u8 feng_num;            //�����ٶȣ�0��1��2��3
    u8 beep_pw;             //����״̬��0��1
    u8 Control_PW;          //�Զ����Ʊ�־λ��0��1
    u8 threshold_PW;        //����ģʽ��־λ��0��1
    u8 threshold_num;       //�����Ǹ���������ֵ��0�������¶ȡ�1������ʪ��
    int temp_th;            //�����¶���ֵ��10-50
    int soil_th;            //����ʪ����ֵ��10-90
} myStatus;

//��ȡ�������ݣ�0��У���롢1������ʪ����ֵ��2������ʪ����ֵ
static u8 read_data_w25q16[3];

void OLED_Start();          //��������
void OLED_Show();           //OLED��ʼ������
void Read_Data();           //�������ݻ�ȡ
void OLED_Refresh();        //OLEDʵʱˢ��
void Control();             //�豸�Զ�����
void Beep_Run();            //����
void Beep_40ms();           //��ʾ��
void Auto_Remote();         //�������

void Read_Threshold();      //��ȡ��ֵ
void Write_Threshold();     //�޸���ֵ

void Manual_Shuibeng();     //�ֶ�ˮ��
void Manual_Light();        //�ֶ�����
void Manual_Feng();         //�ֶ�����
void Manual_Beep();         //�ֶ�����
void Cut_Control();         //�ֶ��Զ��л�

void Publish_Trans();       //������Ϣ����
void Analysis_Subs();       //�������ձ���

#endif //INTELLIGENT_AGRICULTURE_DATA_H
