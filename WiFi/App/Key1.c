#include "main.h"

#define key1_status P32		//P32状态


static void KEY_Detect(void);


Key_t xdata Key1=
{
	false,
	false,
	false,
	false,
	KEY_Detect
};


//按键检测
static void KEY_Detect()
{
	uint16_t key_i=0;
	
	//如果弹起状态清零
	if(key1_status==1)
	{
		Key1.Click=false;
		Key1.Press=false;
	}
	
	//单击检测
	if(Key1.KEY_FLAG==true)
	{
		Key1.KEY_FLAG=false;
		EX0 = 0; //关中断
		
		if(key1_status==0)
		{
			Public.Delayms(10);
			if(key1_status==0)
			{
				Key1.Click=true;
			}
		}
	}

	//长按检测
	if(Key1.Click==true)
	{
		Key1.Press=true;

		Timer0.usDelay_Timer=0;
		while(Timer0.usDelay_Timer<TIMER0_3s)
		{
			if(key1_status==1)
			{
				Key1.Press=false;
				break;
			}
		}
	}

	EX0 = 1; //开中断
}



//KEY1(P32)中断服务函数
void INT0_isr() interrupt 0
{
	Key1.KEY_FLAG=true;
}