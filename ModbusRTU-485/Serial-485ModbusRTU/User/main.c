#include "main.h"


uint8_t flag11=0;		//ȫ�ֱ�־


int main(void)
{
	Hardware_Init.System_Init();//ϵͳ��ʼ��
	while(true)
	{
		UART1.Protocol();//ModbusЭ��
	}
}


//KEY1(P32)�жϷ�����
void INT0_isr() interrupt 0
{
	P34=~P34;
}