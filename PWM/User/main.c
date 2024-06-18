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
	uint8_t main_j=0;
	
	Hardware_Init.System_Init();//系统初始化
	UART1.SendString("Running!\n");
	while(true)
	{
		
		for(main_i=0;main_i<10;main_i++)
		{
			TM1620.Disp_Number(1,main_i);
			for(main_j=0;main_j<10;main_j++)
			{
				TM1620.Disp_Number(2,main_j);
				TM1620.Delay_Bright_ADJ(20);	//10ms扫描调整亮度
				if(((main_i*10+main_j)%10)==0)
				{
					PWM.PWM_LED_Adjust_Brightness();//根据当前显示的数据调整PWM
				}
			}
		}
	}
}