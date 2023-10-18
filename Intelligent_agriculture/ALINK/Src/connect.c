#include "../Inc/connect.h"
#include "../Inc/wifi.h"
#include "../Inc/mqtt.h"
#include "usart.h"	 
#include "../Inc/timer1.h"
#include "led.h"
#include "data.h"

/*-------------------------------------------------*/
/*函数名：终端与阿里云服务器之间的数据接收及发送处理*/
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void connect_deal(void)
{
	if(Connect_flag==1) 
		{    
			//TX 表示有发送数据在缓存内了
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)  
			 {
				//3种情况可进入if，进入到发送的流程中
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：0x30 发布报文，且ConnectPack_flag置位，表示要进行TOPIC的发布
				//第4种：0xc0 
				//第5种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||((MQTT_TxDataOutPtr[2]==0x30)&&(ConnectPack_flag==1))||((MQTT_TxDataOutPtr[2]==0xc0)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
						printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]); 	  //串口提示信息
					
						MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据！！！！！
					
						MQTT_TxDataOutPtr += BUFF_UNIT;                       //指针下移，下移512个地址
					
						if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
							MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			 }				
            
			//RX
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) 	
			{ 												
				printf("接收到数据:");
				if(MQTT_RxDataOutPtr[2]==0x20)   //处理CONNACK报文 ,如果第一个字节是0x20，表示收到的是CONNACK报文 
				{             			
				    switch(MQTT_RxDataOutPtr[5]) //接着我们要判断第4个字节，看CONNECT报文是否成功，根据发回的信息判断错误形式
						{					
							case 0x00 : printf("CONNECT报文成功\r\n");                          //串口输出信息	
										ConnectPack_flag = 1;                                 		//CONNECT报文成功，订阅报文可发
										Ping_flag = 0;                        //【gwd】Ping_flag清零
										TIM1_ENABLE_30S();                  	//【gwd】启动30s的PING定时器
										break;                                                    	//跳出分支case 0x00                                              
							case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
										Connect_flag = 0;                                            //Connect_flag置零，重启连接
										break;                                                       //跳出分支case 0x01   
							case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
										Connect_flag = 0;                                            //Connect_flag置零，重启连接
										break;                                                       //跳出分支case 0x02 
							case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
										Connect_flag = 0;                                            //Connect_flag置零，重启连接
										break;                                                       //跳出分支case 0x03
							case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
										Connect_flag = 0;                                            //Connect_flag置零，重启连接						
										break;                                                       //跳出分支case 0x04
							case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
										Connect_flag = 0;                                            //Connect_flag置零，重启连接						
										break;                                                       //跳出分支case 0x05 		
							default   : printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
										Connect_flag = 0;                                            //Connect_flag置零，重启连接					
										break;                                                       //跳出分支case default 								
					   }				
				}			
				else if(MQTT_RxDataOutPtr[2]==0x90)   //处理SUBACK报文，如果第一个字节是0x90，表示收到的是SUBACK报文
				{ 
					switch(MQTT_RxDataOutPtr[6])        //接着我们要判断订阅回复，看看是不是成功,第5个字节
						{					
						case 0x00 :
						case 0x01 : printf("订阅成功\r\n");            	  
									SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
									Ping_flag = 0;                        //Ping_flag清零
									TIM1_ENABLE_30S();                    //启动30s的PING定时器
									break;                                //跳出分支                                             
						default: printf("订阅失败，准备重启\r\n");  	 	 
									Connect_flag = 0;                     //Connect_flag置零，重启连接
									break;                                //跳出分支 								
						}					
				 }
				else if(MQTT_RxDataOutPtr[2]==0xD0)   //第一个字节是0xD0，表示收到的是PINGRESP报文
				{ 
					printf("PING报文回复!!!!!!!!!!!!!!!!!!\r\n");//串口输出信息 
					
					if(Ping_flag==1)
						{                        //如果Ping_flag=1，表示第一次发送
							Ping_flag = 0;    		 //要清除Ping_flag标志
						}
					else if(Ping_flag>1)
						{ 				               //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
							Ping_flag = 0;     		 //要清除Ping_flag标志
							TIM1_ENABLE_30S(); 		 //PING定时器重回30s的时间,30s时间会Ping一次，以此来确定保活
						}				
				}	
				else if(MQTT_RxDataOutPtr[2]==0x30)  	//如果第一个字节是0x30，表示收到的是服务器发来的推送数据！！服务器发送过来的推送数据
				{                                    	  //提出控制命令
					printf("服务器等级0推送\r\n"); 		       //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据,即PUBLISH数据，发布的数据！！！！非常关键的一个处理功能
				}                                              //gwd 作用巨大，加入到命令缓冲区中！
					                                           //TIM2中断中处理的UART2接收的数据，全部放到了MQTT_RxDataBuf数组中，这里若不是以上MQTT协议数据帧，则这里处理后放置到MQTT_CMDOutPtr缓冲数组中！
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
				   MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];           //指针归位到缓冲区开头                        
			}
			
			//CMD RX
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)                     //if成立的话，说明命令接收缓冲区有数据了
			{
//				printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);           //串口输出信息
                Analysis_Subs();                                    //处理接收的数据
				MQTT_CMDOutPtr += BUFF_UNIT;                        //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	    //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	    //指针归位到缓冲区开头
			}
		} 
	else{ 
			printf("需要连接服务器\r\n");                 //串口输出信息
	
			TIM_Cmd(TIM4,DISABLE);                        //关闭TIM4 
			TIM_Cmd(TIM1,DISABLE);                        //关闭TIM3  
	
			WiFi_RxCounter=0;                             //WiFi接收数据量变量清零   
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //清空WiFi接收缓冲区 
	
			if(WiFi_Connect_IoTServer()==0)               //如果WiFi连接云服务器函数返回0，表示正确，进入if
			{   			         
				printf("建立TCP连接成功\r\n");               //串口输出信息，已经和云服务器平台建立起连接了
				Connect_flag = 1;                           //Connect_flag置1，表示连接成功	
				
				WiFi_RxCounter=0;                           //WiFi接收数据量变量清零  
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 		
				
				MQTT_Buff_ReInit(); 					      //重新初始化发送缓冲区
				TIM1_ENABLE_5S();                  //【gwd】启动5sPING检测处理【gwd】前置Ping的报文处理，避免CONNECT的处理问题
			}				
		}			

}

/*-------------------------------------------------*/
/*函数名：判断开关状态，并发布给服务器            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED0_State(void)
{
//    char temp[BUFF_UNIT];                   //定义一个临时缓冲区
//    if(LED0_IN_STA) sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"LightSwitch\":0},\"version\":\"1.0.0\"}");  //如果LED1是高电平，说明是熄灭状态，需要回复关闭状态给服务器
//    else  sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"LightSwitch\":1},\"version\":\"1.0.0\"}");  //如果LED1是低电平，说明是点亮状态，需要回复打开状态给服务器
//    MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
}


/*-------------------------------------------------*/
/*函数名：Alink 驱动初始化                         */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Alink_Init(void)
{
	Usart3_Init(115200);
	WiFi_ResetIO_Init();    	//初始化ESP8266的复位引脚 PA1 
	TIM4_Init(299,7199);        //TIM4初始化，定时时间 300*7200*1000/72000000 = 30ms
	MQTT_Buff_Init();       	//MQTT缓存区初始化
	AliIoT_Parameter_Init();	//阿里云连接设置初始化
}



