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
	uint8_t main_i=0;
	uint8_t array[8]={1,2,3,4,5,6,7,8};
	uint8_t array1[8]={0};
	
	Hardware_Init.System_Init();//系统初始化
	UART1.SendString("Running!\n");
	while(true)
	{
		AT24C02.Sendstr(array,8);			//写数据

		AT24C02.Receivestr(array1,8);		//读数据
		
		UART1.RS485_Send_Mode();
		for(main_i=0;main_i<8;main_i++)
		{
			printf("array1[%bu]=%bu",main_i,array1[main_i]);
		}
		UART1.RS485_Receive_Mode();
		
		for(main_i=0;main_i<8;main_i++)
		{
			array[main_i]+=1;
		}

		Public.Delayms(500);
	}

}