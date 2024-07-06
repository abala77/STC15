#ifndef __WIFI_H__
#define __WIFI_H__

//TCP数据服务和配网
typedef struct
{
	uint8_t volatile TCP_Connect_Status;		//TCP连接状态
	uint16_t TCP_Reconnect_Timer;						//TCP重连定时器
	
	void (*Init)(void);											//初始化
	void (*SmartConfig)(void);							//WiFi模块配网
	void (*TCP_Connect_Server)(void);				//通过TCP连接服务器
	void (*Send_data)(void);								//通过TCP发送数据
	void (*Receive_data)(void);							//通过TCP接收数据
	void (*Heart)(void);							//心跳
}WiFi_t;

//调试函数
typedef struct
{
	void (*Computer_to_ESP12S)(void);				//电脑串口->单片机->ESP12S
	void (*ESP12S_to_Computer)(void);				//ESP12S->单片机->电脑串口
}WiFi_Debug_t;


extern WiFi_t xdata esp12s;
extern WiFi_Debug_t xdata WiFi_Debug;

#endif