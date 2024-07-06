#include "main.h"

#define KEY2_Status		P33

#define Set_Press_TIME	TIMER0_2s				//�趨����ʱ��
#define Set_Double_Click_TIME	TIMER0_200ms	//�趨˫��ʱ��


static uint8_t Click_Buf = false;	//����״̬����

static void KEY_Detect(void);		//��ť���ؼ��

Key_t KEY2	={false,false,false,false,KEY_Detect};
STA_Machine_t STA_Machine ={STA1_KEY_Up,0,0,0};


//�������
static void KEY_Detect()
{
	if(STA_Machine.ucSTA_Machine_Scan_Timer>=TIMER0_10ms)
	{
		UART1.SendString("Detect!!\n");
		switch(STA_Machine.ucSTA_Machine_Status)
		{
			case STA1_KEY_Up:
			{
				//���е������
				if(KEY2_Status==1)
				{
					if(Click_Buf==true)
					{
						//����ʱ��û�а��¾ͱ��ж�Ϊ����
						if(STA_Machine.usKEY2_Doule_Click_Timer>=Set_Double_Click_TIME)
						{
							KEY2.KEY_FLAG=true;
							KEY2.Click=true;

							Click_Buf=false;	//�����������
						}
					}
				}
				else//��⵽�͵�ƽ��״̬�л������¶���
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

					STA_Machine.usKEY2_Press_Timer=0;//������ⶨʱ�����㣬��ʼ��ʱ
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
					//���г������
					if(KEY2.Press==false)
					{
						//���³����趨ʱ���ж�Ϊ����
						if(STA_Machine.usKEY2_Press_Timer>=Set_Press_TIME)
						{
							STA_Machine.ucSTA_Machine_Status=STA4_KEY_UpShake;
							KEY2.KEY_FLAG=true;
							KEY2.Press=true;

							if(Click_Buf==true)
								Click_Buf=false;//��⵽�����������һ�εİ�������
						}
					}
				}
				else
				{
					STA_Machine.ucSTA_Machine_Status = STA4_KEY_UpShake;
					if(KEY2.Press==false)
					{
						//˫����� 

						//����ϴε�����һ�Σ�����Ϊ˫��
						if(Click_Buf==true)
						{
							KEY2.KEY_FLAG = true;
							KEY2.Double_Click=true;

							Click_Buf=false;//�����������
						}
						else
						{
							Click_Buf=true;//��������
							STA_Machine.usKEY2_Doule_Click_Timer=0;//˫����ʱ����ʼ����
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


	//���ݰ������ж�Ӧ�Ķ���
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
