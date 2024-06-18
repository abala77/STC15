#include "main.h"

#define CCP_S1 bit5
#define CCP_S0 bit4

#define EPC0H	 bit1	//PCA_PWM0�Ĵ������ñȽ�ֵ(�ھ�λ)
#define EPC0L  bit2


static void PWM_Init(void);
static void PWM_LED_Adjust_Brightness(void);

PWM_t PWM=
{
	Duty_20,
	PWM_Init,
	PWM_LED_Adjust_Brightness
};

//PWM��ʼ��
static void PWM_Init()
{
	//CCPӳ�䵽P35
	AUXR1 &= ~CCP_S1;
	AUXR1 |=  CCP_S0;
	
	CCON=0x00;
	CMOD=0x0C;//����PCAʱ��Դ  ->  SYSclk/6
						//PWMƵ��=SYSclk/6/256=7.2kHz
	
	CL=0;
	CH=0;//��λPCA������
	
	CCAPM0	=0x42;		//����Ƚ���������
	PCA_PWM0	=0x00;	//������8λPWMģʽ
	
	//����Ĭ��ռ�ձ�
	PCA_PWM0 &= (~EPC0H);	//�ھ�λΪ0
	CCAP0H=256*(1-0.2);		//����ռ�ձ�Ϊ20%
	
	CR=1;//����PCA��ʱ��
}

//LED��������
static void PWM_LED_Adjust_Brightness()
{
	//�л�ռ�ձ�
	switch(PWM.Duty)
	{
		case Duty_0:  	PWM.Duty=Duty_20;  CCAP0H=256*(1-0.2); break;
		case Duty_20: 	PWM.Duty=Duty_40;  CCAP0H=256*(1-0.4); break;
		case Duty_40:	 	PWM.Duty=Duty_60;  CCAP0H=256*(1-0.6); break;
		case Duty_60: 	PWM.Duty=Duty_80;  CCAP0H=256*(1-0.8); break;
		case Duty_80: 	PWM.Duty=Duty_100; CCAP0H=0;   				 break;
		case Duty_100:  PWM.Duty=Duty_0;   CCAP0H=255; 			   break;
	}
}