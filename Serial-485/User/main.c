#include "main.h"

uint8_t flag11=0;

int main(void)
{
	Hardware_Init.System_Init();
	UART1.RS485_Send_Mode();			//����485Ϊ����ģʽ
	printf("Program is running.\n");//���ݷ���
	UART1.RS485_Receive_Mode();		//Ĭ��Ϊ����ģʽ�����͵�ƽ
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
	}
}


//KEY1(P32)�жϷ�����
void INT0_isr() interrupt 0
{
	P34=~P34;
}