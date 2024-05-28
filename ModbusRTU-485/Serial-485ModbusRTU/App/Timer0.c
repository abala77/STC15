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
  *  @brief  定时器0初始化
  *  @param  None
  *  @retval None
*/
static void Timer0_Init()		//5毫秒@11.0592MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0xEE;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

/*
  *  @name   Timer0_isr
  *  @brief  定时器0中断
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