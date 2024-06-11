#include "main.h"

#define key1_status P32		//P32״̬


static void KEY_Detect(void);


Key_t xdata Key1=
{
	false,
	false,
	false,
	false,
	KEY_Detect
};


//�������
static void KEY_Detect()
{
	uint16_t key_i=0;
	
	//�������״̬����
	if(key1_status==1)
	{
		Key1.Click=false;
		Key1.Press=false;
	}
	
	//�������
	if(Key1.KEY_FLAG==true)
	{
		Key1.KEY_FLAG=false;
		EX0 = 0; //���ж�
		
		if(key1_status==0)
		{
			Public.Delayms(10);
			if(key1_status==0)
			{
				Key1.Click=true;
			}
		}
	}

	//�������
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

	EX0 = 1; //���ж�
}



//KEY1(P32)�жϷ�����
void INT0_isr() interrupt 0
{
	Key1.KEY_FLAG=true;
}