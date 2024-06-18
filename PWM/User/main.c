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
	uint8_t main_j=0;
	
	Hardware_Init.System_Init();//ϵͳ��ʼ��
	UART1.SendString("Running!\n");
	while(true)
	{
		
		for(main_i=0;main_i<10;main_i++)
		{
			TM1620.Disp_Number(1,main_i);
			for(main_j=0;main_j<10;main_j++)
			{
				TM1620.Disp_Number(2,main_j);
				TM1620.Delay_Bright_ADJ(20);	//10msɨ���������
				if(((main_i*10+main_j)%10)==0)
				{
					PWM.PWM_LED_Adjust_Brightness();//���ݵ�ǰ��ʾ�����ݵ���PWM
				}
			}
		}
	}
}