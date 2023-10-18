#include "../Inc/connect.h"
#include "../Inc/wifi.h"
#include "../Inc/mqtt.h"
#include "usart.h"	 
#include "../Inc/timer1.h"
#include "led.h"
#include "data.h"

/*-------------------------------------------------*/
/*���������ն��밢���Ʒ�����֮������ݽ��ռ����ʹ���*/
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void connect_deal(void)
{
	if(Connect_flag==1) 
		{    
			//TX ��ʾ�з��������ڻ�������
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)  
			 {
				//3������ɽ���if�����뵽���͵�������
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�0x30 �������ģ���ConnectPack_flag��λ����ʾҪ����TOPIC�ķ���
				//��4�֣�0xc0 
				//��5�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||((MQTT_TxDataOutPtr[2]==0x30)&&(ConnectPack_flag==1))||((MQTT_TxDataOutPtr[2]==0xc0)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
						printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]); 	  //������ʾ��Ϣ
					
						MQTT_TxData(MQTT_TxDataOutPtr);                       //�������ݣ���������
					
						MQTT_TxDataOutPtr += BUFF_UNIT;                       //ָ�����ƣ�����512����ַ
					
						if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
							MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
				} 				
			 }				
            
			//RX
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) 	
			{ 												
				printf("���յ�����:");
				if(MQTT_RxDataOutPtr[2]==0x20)   //����CONNACK���� ,�����һ���ֽ���0x20����ʾ�յ�����CONNACK���� 
				{             			
				    switch(MQTT_RxDataOutPtr[5]) //��������Ҫ�жϵ�4���ֽڣ���CONNECT�����Ƿ�ɹ������ݷ��ص���Ϣ�жϴ�����ʽ
						{					
							case 0x00 : printf("CONNECT���ĳɹ�\r\n");                          //���������Ϣ	
										ConnectPack_flag = 1;                                 		//CONNECT���ĳɹ������ı��Ŀɷ�
										Ping_flag = 0;                        //��gwd��Ping_flag����
										TIM1_ENABLE_30S();                  	//��gwd������30s��PING��ʱ��
										break;                                                    	//������֧case 0x00                                              
							case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
										Connect_flag = 0;                                            //Connect_flag���㣬��������
										break;                                                       //������֧case 0x01   
							case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
										Connect_flag = 0;                                            //Connect_flag���㣬��������
										break;                                                       //������֧case 0x02 
							case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
										Connect_flag = 0;                                            //Connect_flag���㣬��������
										break;                                                       //������֧case 0x03
							case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
										Connect_flag = 0;                                            //Connect_flag���㣬��������						
										break;                                                       //������֧case 0x04
							case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
										Connect_flag = 0;                                            //Connect_flag���㣬��������						
										break;                                                       //������֧case 0x05 		
							default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
										Connect_flag = 0;                                            //Connect_flag���㣬��������					
										break;                                                       //������֧case default 								
					   }				
				}			
				else if(MQTT_RxDataOutPtr[2]==0x90)   //����SUBACK���ģ������һ���ֽ���0x90����ʾ�յ�����SUBACK����
				{ 
					switch(MQTT_RxDataOutPtr[6])        //��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�,��5���ֽ�
						{					
						case 0x00 :
						case 0x01 : printf("���ĳɹ�\r\n");            	  
									SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									Ping_flag = 0;                        //Ping_flag����
									TIM1_ENABLE_30S();                    //����30s��PING��ʱ��
									break;                                //������֧                                             
						default: printf("����ʧ�ܣ�׼������\r\n");  	 	 
									Connect_flag = 0;                     //Connect_flag���㣬��������
									break;                                //������֧ 								
						}					
				 }
				else if(MQTT_RxDataOutPtr[2]==0xD0)   //��һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				{ 
					printf("PING���Ļظ�!!!!!!!!!!!!!!!!!!\r\n");//���������Ϣ 
					
					if(Ping_flag==1)
						{                        //���Ping_flag=1����ʾ��һ�η���
							Ping_flag = 0;    		 //Ҫ���Ping_flag��־
						}
					else if(Ping_flag>1)
						{ 				               //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
							Ping_flag = 0;     		 //Ҫ���Ping_flag��־
							TIM1_ENABLE_30S(); 		 //PING��ʱ���ػ�30s��ʱ��,30sʱ���Pingһ�Σ��Դ���ȷ������
						}				
				}	
				else if(MQTT_RxDataOutPtr[2]==0x30)  	//�����һ���ֽ���0x30����ʾ�յ����Ƿ������������������ݣ������������͹�������������
				{                                    	  //�����������
					printf("�������ȼ�0����\r\n"); 		       //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������,��PUBLISH���ݣ����������ݣ��������ǳ��ؼ���һ��������
				}                                              //gwd ���þ޴󣬼��뵽��������У�
					                                           //TIM2�ж��д����UART2���յ����ݣ�ȫ���ŵ���MQTT_RxDataBuf�����У���������������MQTTЭ������֡�������ﴦ�����õ�MQTT_CMDOutPtr���������У�
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //���ָ�뵽������β����
				   MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];           //ָ���λ����������ͷ                        
			}
			
			//CMD RX
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)                     //if�����Ļ���˵��������ջ�������������
			{
//				printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);           //���������Ϣ
                Analysis_Subs();                                    //������յ�����
				MQTT_CMDOutPtr += BUFF_UNIT;                        //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	    //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	    //ָ���λ����������ͷ
			}
		} 
	else{ 
			printf("��Ҫ���ӷ�����\r\n");                 //���������Ϣ
	
			TIM_Cmd(TIM4,DISABLE);                        //�ر�TIM4 
			TIM_Cmd(TIM1,DISABLE);                        //�ر�TIM3  
	
			WiFi_RxCounter=0;                             //WiFi������������������   
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //���WiFi���ջ����� 
	
			if(WiFi_Connect_IoTServer()==0)               //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			{   			         
				printf("����TCP���ӳɹ�\r\n");               //���������Ϣ���Ѿ����Ʒ�����ƽ̨������������
				Connect_flag = 1;                           //Connect_flag��1����ʾ���ӳɹ�	
				
				WiFi_RxCounter=0;                           //WiFi������������������  
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //���WiFi���ջ����� 		
				
				MQTT_Buff_ReInit(); 					      //���³�ʼ�����ͻ�����
				TIM1_ENABLE_5S();                  //��gwd������5sPING��⴦��gwd��ǰ��Ping�ı��Ĵ�������CONNECT�Ĵ�������
			}				
		}			

}

/*-------------------------------------------------*/
/*���������жϿ���״̬����������������            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED0_State(void)
{
//    char temp[BUFF_UNIT];                   //����һ����ʱ������
//    if(LED0_IN_STA) sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"LightSwitch\":0},\"version\":\"1.0.0\"}");  //���LED1�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
//    else  sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"LightSwitch\":1},\"version\":\"1.0.0\"}");  //���LED1�ǵ͵�ƽ��˵���ǵ���״̬����Ҫ�ظ���״̬��������
//    MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
}


/*-------------------------------------------------*/
/*��������Alink ������ʼ��                         */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Alink_Init(void)
{
	Usart3_Init(115200);
	WiFi_ResetIO_Init();    	//��ʼ��ESP8266�ĸ�λ���� PA1 
	TIM4_Init(299,7199);        //TIM4��ʼ������ʱʱ�� 300*7200*1000/72000000 = 30ms
	MQTT_Buff_Init();       	//MQTT��������ʼ��
	AliIoT_Parameter_Init();	//�������������ó�ʼ��
}



