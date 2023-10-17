#include "remote.h"
#include "delay.h"
#include "stdio.h"

//����ң�س�ʼ��
//����IO�Լ���ʱ��3�����벶��
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��PORTBʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//TIM3 ʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//�������� 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʼ��GPIOB.0
				  
 	TIM_TimeBaseStructure.TIM_Period = 10000; //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  // ѡ������� IC3ӳ�䵽TI3��
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ
    TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC3F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��
    TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig( TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//��������ж� ,����CC4IE�����ж�
}

//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 	RmtSta=0;	  	  
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec=0;	//������յ�������	   		    
u8  RmtCnt=0;	//�������µĴ���	  
//��ʱ��4�жϷ������	 
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)								//�ϴ������ݱ����յ���
		{
			RmtSta&=~0X10;							//ȡ���������Ѿ���������
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;	//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);					//���������ʶ
				RmtSta&=0XF0;						//��ռ�����
			}
		}
	}
	if(TIM_GetITStatus(TIM3,TIM_IT_CC3)!=RESET)
	{
		if(RDATA)//�����ز���
		{
  		TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);						//CC4P=1	����Ϊ�½��ز���
			TIM_SetCounter(TIM3,0);							//��ն�ʱ��ֵ
			RmtSta|=0X10;							//����������Ѿ�������
		}else //�½��ز���
		{
			Dval=TIM_GetCapture3(TIM3);					//��ȡCCR4Ҳ������CC4IF��־λ
  		TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);				//CC4P=0	����Ϊ�����ز���
			if(RmtSta&0X10)							//���һ�θߵ�ƽ����
			{
 				if(RmtSta&0X80)//���յ���������
				{

					if(Dval>300&&Dval<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec<<=1;					//����һλ.
						RmtRec|=0;					//���յ�0
					}else if(Dval>1400&&Dval<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec<<=1;					//����һλ.
						RmtRec|=1;					//���յ�1
					}else if(Dval>2200&&Dval<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt++; 					//������������1��
						RmtSta&=0XF0;				//��ռ�ʱ��
					}
 				}else if(Dval>4200&&Dval<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta|=1<<7;					//��ǳɹ����յ���������
					RmtCnt=0;						//�����������������
				}
			}
			RmtSta&=~(1<<4);
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update|TIM_IT_CC3);
}

//����������
//����ֵ:
//0,û���κΰ�������
//����,���µİ�����ֵ.
u8 Remote_Scan(void)
{        
	u8 sta=0;       
    u8 t1,t2;
	if(RmtSta&(1<<6))//�õ�һ��������������Ϣ��
	{ 
	    t1=RmtRec>>24;			//�õ���ַ��
	    t2=(RmtRec>>16)&0xff;	//�õ���ַ���� 
 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//����ң��ʶ����(ID)����ַ 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)sta=t1;//��ֵ��ȷ	 
			}   
			if((sta==0)||((RmtSta&0X80)==0))//�������ݴ���/ң���Ѿ�û�а�����
			{
				RmtSta&=~(1<<6);//������յ���Ч������ʶ
				RmtCnt=0;		//�����������������
			}
	}  
  return sta;
}


u8 key;
u8 *str=0;
u8 *remote_deal(void)
{
    key=Remote_Scan();
    if(key)
    {
        switch(key)
        {
            case 0:str="ERROR";break;
            case 162:str="POWER";break;
            case 98:str="UP";break;
            case 2:str="PLAY";break;
            case 226:str="ALIENTEK";break;
            case 194:str="RIGHT";break;
            case 34:str="LEFT";break;
            case 224:str="VOL-";break;
            case 168:str="DOWN";break;
            case 144:str="VOL+";break;
            case 104:str="1";break;
            case 152:str="2";break;
            case 176:str="3";break;
            case 48:str="4";break;
            case 24:str="5";break;
            case 122:str="6";break;
            case 16:str="7";break;
            case 56:str="8";break;
            case 90:str="9";break;
            case 66:str="0";break;
            case 82:str="DELETE";break;
        }
        printf("Signal is %s.\r\n",str);
    }
    return str;
}