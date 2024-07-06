#include "main.h"

#define KEY2_Status		P33

#define Set_Press_TIME	TIMER0_2s				//设定长按时间
#define Set_Double_Click_TIME	TIMER0_200ms	//设定双击时间


static uint8_t Click_Buf = false;	//单击状态缓存

static void KEY_Detect(void);		//按钮开关检测

Key_t KEY2	={false,false,false,false,KEY_Detect};
STA_Machine_t STA_Machine ={STA1_KEY_Up,0,0,0};


//按键检测
static void KEY_Detect()
{
	if(STA_Machine.ucSTA_Machine_Scan_Timer>=TIMER0_10ms)
	{
		UART1.SendString("Detect!!\n");
		switch(STA_Machine.ucSTA_Machine_Status)
		{
			case STA1_KEY_Up:
			{
				//进行单击检测
				if(KEY2_Status==1)
				{
					if(Click_Buf==true)
					{
						//超过时间没有按下就被判断为单击
						if(STA_Machine.usKEY2_Doule_Click_Timer>=Set_Double_Click_TIME)
						{
							KEY2.KEY_FLAG=true;
							KEY2.Click=true;

							Click_Buf=false;	//清除单击缓存
						}
					}
				}
				else//检测到低电平，状态切换到按下抖动
				{
					STA_Machine.ucSTA_Machine_Status=STA2_KEY_DownShake;
				}
				break;
			}
			case STA2_KEY_DownShake:
			{
				if(KEY2_Status==0)
				{
					STA_Machine.ucSTA_Machine_Status=STA3_KEY_Down;

					STA_Machine.usKEY2_Press_Timer=0;//长按检测定时器清零，开始计时
				}
				else
				{
					STA_Machine.ucSTA_Machine_Status=STA1_KEY_Up;
				}
				break;
			}
			case STA3_KEY_Down:
			{
				if(KEY2_Status==0)
				{
					//进行长按检测
					if(KEY2.Press==false)
					{
						//按下超过设定时间判断为长按
						if(STA_Machine.usKEY2_Press_Timer>=Set_Press_TIME)
						{
							STA_Machine.ucSTA_Machine_Status=STA4_KEY_UpShake;
							KEY2.KEY_FLAG=true;
							KEY2.Press=true;

							if(Click_Buf==true)
								Click_Buf=false;//检测到长按，清除上一次的按键单击
						}
					}
				}
				else
				{
					STA_Machine.ucSTA_Machine_Status = STA4_KEY_UpShake;
					if(KEY2.Press==false)
					{
						//双击检测 

						//如果上次单击了一次，就置为双击
						if(Click_Buf==true)
						{
							KEY2.KEY_FLAG = true;
							KEY2.Double_Click=true;

							Click_Buf=false;//清除单击缓存
						}
						else
						{
							Click_Buf=true;//单击缓存
							STA_Machine.usKEY2_Doule_Click_Timer=0;//双击定时器开始清零
						}
					}
					
				}
				break;
			}
			case STA4_KEY_UpShake:
			{
				if(KEY2_Status==1)
					STA_Machine.ucSTA_Machine_Status=STA1_KEY_Up;
				break;
			}
			default:STA_Machine.ucSTA_Machine_Status=STA1_KEY_Up;
		}	
	}


	//根据按键进行对应的动作
	if(KEY2.KEY_FLAG == true)
	{
		KEY2.KEY_FLAG=false;
		if(KEY2.Click==true)
		{
			PWM.PWM_LED_Adjust_Brightness();
		}
		else if(KEY2.Double_Click==true)
		{
			PWM.Duty=0;
			PWM.Set_Brightness();
		}
		else if(KEY2.Press==true)
		{
			PWM.Duty=100;
			PWM.Set_Brightness();
		}
        
        KEY2.Click        = false;
        KEY2.Double_Click = false;
        KEY2.Press        = false;
	}
    
	STA_Machine.ucSTA_Machine_Scan_Timer = 0;
}
