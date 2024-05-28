#include "main.h"


uint8_t flag11=0;		//全局标志


int main(void)
{
	Hardware_Init.System_Init();//系统初始化
	while(true)
	{
		UART1.Protocol();//Modbus协议
	}
}


//KEY1(P32)中断服务函数
void INT0_isr() interrupt 0
{
	P34=~P34;
}