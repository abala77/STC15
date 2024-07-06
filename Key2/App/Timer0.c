#include "main.h"


static void Timer0_Init(void);


Timer0_t idata Timer0=
{
	0,
	0,
	0,
	Timer0_Init
};


/*
  *  @name   Timer0_Init
  *  @brief  ��ʱ��0��ʼ��
  *  @param  None
  *  @retval None
*/
static void Timer0_Init()		//5����@11.0592MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x00;				//���ö�ʱ��ʼֵ
	TH0 = 0xEE;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}

/*
  *  @name   Timer0_isr
  *  @brief  ��ʱ��0�ж�
  *  @param  None
  *  @retval None
*/
void Timer0_isr() interrupt 1
{
	if(Timer0.usAT_SettingTips==true)
	{
		if(++Timer0.usMCU_RUN_Timer>=TIMER0_50ms)
		{
			Timer0.usMCU_RUN_Timer=0;
			Public.Led_Flip();
		}
	}
	else
	{
		if(++Timer0.usMCU_RUN_Timer>=TIMER0_1s)
		{
			Timer0.usMCU_RUN_Timer=0;
			Public.Led_Flip();
		}
	}
	
	Timer0.usDelay_Timer++;
	esp12s.TCP_Reconnect_Timer++;
	
	STA_Machine.ucSTA_Machine_Scan_Timer++;
	STA_Machine.usKEY2_Doule_Click_Timer++;
	STA_Machine.usKEY2_Press_Timer++;
}