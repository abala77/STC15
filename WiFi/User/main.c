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
	uint8_t times=0;
	Hardware_Init.System_Init();//ϵͳ��ʼ��
	UART1.SendString("Running!\n");
	while(true)
	{
		//���ӷ�����
		if((esp12s.TCP_Connect_Status==false) && (esp12s.TCP_Reconnect_Timer>TIMER0_5s))
		{
			esp12s.TCP_Connect_Server();
		}
		
		//����
		esp12s.Heart();

//		UART2.Protocol();
		
		//�������
		Key1.KEY_Detect();
		//����
		esp12s.SmartConfig();
	
//		//����
//		WiFi_Debug.Computer_to_ESP12S();
//		WiFi_Debug.ESP12S_to_Computer();
		
	}
}

