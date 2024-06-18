#include "main.h"

#define CCP_S1 bit5
#define CCP_S0 bit4

#define EPC0H	 bit1	//PCA_PWM0寄存器设置比较值(第九位)
#define EPC0L  bit2


static void PWM_Init(void);
static void PWM_LED_Adjust_Brightness(void);

PWM_t PWM=
{
	Duty_20,
	PWM_Init,
	PWM_LED_Adjust_Brightness
};

//PWM初始化
static void PWM_Init()
{
	//CCP映射到P35
	AUXR1 &= ~CCP_S1;
	AUXR1 |=  CCP_S0;
	
	CCON=0x00;
	CMOD=0x0C;//设置PCA时钟源  ->  SYSclk/6
						//PWM频率=SYSclk/6/256=7.2kHz
	
	CL=0;
	CH=0;//复位PCA计数器
	
	CCAPM0	=0x42;		//允许比较器公功能
	PCA_PWM0	=0x00;	//工作于8位PWM模式
	
	//设置默认占空比
	PCA_PWM0 &= (~EPC0H);	//第九位为0
	CCAP0H=256*(1-0.2);		//设置占空比为20%
	
	CR=1;//启动PCA计时器
}

//LED调整亮度
static void PWM_LED_Adjust_Brightness()
{
	//切换占空比
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