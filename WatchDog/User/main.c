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
	uint8_t ret=0;
	
	Hardware_Init.System_Init();//ϵͳ��ʼ��
	UART1.SendString("Running!\n");
	
	while(true)
	{
		NTC.Formula_CalcTemp();	//��ʽ�����¶�
		TM1620.Disp_Number(1,(uint8_t)NTC.Recent_Tempurature/10);
		TM1620.Disp_Number(2,(uint8_t)NTC.Recent_Tempurature%10);
		Public.Delayms(500);
		Watchdog.Clear();
	}

}