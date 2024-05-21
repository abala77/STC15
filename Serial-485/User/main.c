#include "main.h"

uint8_t flag11=0;

int main(void)
{
	Hardware_Init.System_Init();
	UART1.RS485_Send_Mode();			//设置485为发送模式
	printf("Program is running.\n");//数据发送
	UART1.RS485_Receive_Mode();		//默认为接收模式，拉低电平
	while(true)
	{
		if((strstr(UART1.Receive_Data,"P34=1"))!=0)
		{
			UART1.RS485_Send_Mode();
			printf("开灯%s\n",UART1.Receive_Data);//向串口发送信息
			UART1.RS485_Receive_Mode();
			P34=1;
			memset(UART1.Receive_Data,0,strlen(UART1.Receive_Data));//缓冲数据清零，确保下次成功接收
			UART1.ucRec_Cnt=0;
		}
		else if((strstr(UART1.Receive_Data,"P34=0"))!=0)
		{
			UART1.RS485_Send_Mode();
			printf("关灯%s\n",UART1.Receive_Data);
			UART1.RS485_Receive_Mode();
			P34=0;
			memset(UART1.Receive_Data,0,strlen(UART1.Receive_Data));
			UART1.ucRec_Cnt=0;
		}
	}
}


//KEY1(P32)中断服务函数
void INT0_isr() interrupt 0
{
	P34=~P34;
}