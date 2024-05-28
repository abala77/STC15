#include "main.h"


static void Timer0_Init(void);


Timer0_t Timer0=
{
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
	if(++Timer0.usMCU_RUN_Timer>=TIMER0_1S)
	{
		Timer0.usMCU_RUN_Timer=0;
		P34=~P34;
	}
	Timer0.usDelay_Timer++;
}