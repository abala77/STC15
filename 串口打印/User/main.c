#include "main.h"


int main(void)
{
	Hardware_Init.System_Init();
	printf("Program is running.");
	while(true)
	{
		if((strstr(UART1.Receive_Data,"P34=1"))!=0)
		{
			printf("����%s\n",UART1.Receive_Data);//�򴮿ڷ�����Ϣ
			P34=1;
			memset(UART1.Receive_Data,0,strlen(UART1.Receive_Data));//�����������㣬ȷ���´γɹ�����
			UART1.ucRec_Cnt=0;
		}
		else if((strstr(UART1.Receive_Data,"P34=0"))!=0)
		{
			printf("�ص�%s\n",UART1.Receive_Data);
			P34=0;
			memset(UART1.Receive_Data,0,strlen(UART1.Receive_Data));
			UART1.ucRec_Cnt=0;
		}
	}
}


//KEY1(P32)�жϷ�����
void INT0_isr() interrupt 0
{
	P34=~P34;
}