#ifndef  __REMOTE_H
#define  __REMOTE_H

#include "sys.h"

#define RDATA 	PBin(0)	 	//�������������
#define REMOTE_ID 0 
//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0

void Remote_Init(void);     //���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);       //���⴫�������ܵ�����
u8 *remote_deal(void);      //���ݴ���

#endif  




