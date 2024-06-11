#include "main.h"


uint8_t idata flag11=0;		//全局标志




/* -----------------------------说明 -----------------------------------------
1、 单片机型号 -> STC15L2K32S2   LQFP32
2、 系统时钟   -> 外部晶体，11.0592MHz 
3、 复位       -> 内部上电复位
4、 MyPrintf   -> 映射到串口1，通过USB接口输出调试信息

5、 定时器0    -> 5ms中断一次，内部定时用
6、 串口1      -> RS-485
6、 串口2      -> WIFI模块
-----------------------------------------------------------------------------*/
int main(void)
{
	uint8_t times=0;
	Hardware_Init.System_Init();//系统初始化
	UART1.SendString("Running!\n");
	while(true)
	{
		//连接服务器
		if((esp12s.TCP_Connect_Status==false) && (esp12s.TCP_Reconnect_Timer>TIMER0_5s))
		{
			esp12s.TCP_Connect_Server();
		}
		
		//心跳
		esp12s.Heart();

//		UART2.Protocol();
		
		//按键检测
		Key1.KEY_Detect();
		//配网
		esp12s.SmartConfig();
	
//		//调试
//		WiFi_Debug.Computer_to_ESP12S();
//		WiFi_Debug.ESP12S_to_Computer();
		
	}
}

