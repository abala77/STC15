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
	uint8_t ret=0;
	
	Hardware_Init.System_Init();//系统初始化
	UART1.SendString("Running!\n");
	
	while(true)
	{
		KEY2.KEY_Detect();
		Watchdog.Clear();			//看门狗清零
	}

}