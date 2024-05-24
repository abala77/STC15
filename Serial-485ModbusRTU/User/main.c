#include "main.h"

#define TestPrint 1

uint8_t flag11=0;

int main(void)
{
	Hardware_Init.System_Init();
	UART1.SendString("Program is running.\n");
	while(true)
	{
		if((strstr(UART1.Receive_Data,"P34=1"))!=0)
		{
			UART1.RS485_Send_Mode();
			printf("����%s\n",UART1.Receive_Data);//�򴮿ڷ�����Ϣ
			UART1.RS485_Receive_Mode();
			P34=1;
			memset(UART1.Receive_Data,0,strlen(UART1.Receive_Data));//�����������㣬ȷ���´γɹ�����
			UART1.ucRec_Cnt=0;
		}
		else if((strstr(UART1.Receive_Data,"P34=0"))!=0)
		{
			UART1.RS485_Send_Mode();
			printf("�ص�%s\n",UART1.Receive_Data);
			UART1.RS485_Receive_Mode();
			P34=0;
			memset(UART1.Receive_Data,0,strlen(UART1.Receive_Data));
			UART1.ucRec_Cnt=0;
		}
		#if TestPrint
			UART1.RS485_Send_Mode();
			//printf("flag11==%bd,UART1.Receive_Data=%s\n",flag11,UART1.Receive_Data);
			printf("sizeof(BaudRate)=%bd\nBaud_115200=%bd\n",sizeof(BaudRate_t),sizeof(Baud_115200));
			UART1.RS485_Receive_Mode();
			Public.Delayms(100);
		#endif
	}
}


//KEY1(P32)�жϷ�����
void INT0_isr() interrupt 0
{
	P34=~P34;
}