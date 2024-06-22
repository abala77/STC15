#include "main.h"


uint8_t idata flag11=0;		//ȫ�ֱ�־

/* -----------------------------˵�� -----------------------------------------
1�� ��Ƭ���ͺ� -> STC15L2K32S2   LQFP32
2�� ϵͳʱ��   -> �ⲿ���壬11.0592MHz 
3�� ��λ       -> �ڲ��ϵ縴λ
4�� MyPrintf   -> ӳ�䵽����1��ͨ��USB�ӿ����������Ϣ

5�� ��ʱ��0    -> 5ms�ж�һ�Σ��ڲ���ʱ��
6�� ����1      -> RS-485
6�� ����2      -> WIFIģ��
-----------------------------------------------------------------------------*/
int main(void)
{
	uint8_t main_i=0;
	uint8_t array[8]={1,2,3,4,5,6,7,8};
	uint8_t array1[8]={0};
	
	Hardware_Init.System_Init();//ϵͳ��ʼ��
	UART1.SendString("Running!\n");
	while(true)
	{
		AT24C02.Sendstr(array,8);			//д����

		AT24C02.Receivestr(array1,8);		//������
		
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