#include "main.h"

#define CCP_S1 bit5
#define CCP_S0 bit4

#define EPC0H	 bit1	//PCA_PWM0寄存器设置比较值(第九位)
#define EPC0L  bit2


static void PWM_Init(void);
static void PWM_LED_Adjust_Brightness(void);

static void BackupDuty(uint16_t ,uint8_t );
static uint8_t ReadDuty(uint16_t );

static void Set_Brightness(void);//设置亮度


PWM_t PWM=
{
	Duty_20,
	PWM_Init,
	PWM_LED_Adjust_Brightness,

	BackupDuty,
	ReadDuty
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
	PWM.Duty=ReadDuty(EEPROM_Default_Addr);
	
	Set_Brightness();//设置亮度
	
	CR=1;//启动PCA计时器
}

//使用按键调整亮度
static void PWM_LED_Adjust_Brightness()
{
	//切换占空比
	switch(PWM.Duty)
	{
		case Duty_0:   PWM.Duty=Duty_20;  break;
		case Duty_20:  PWM.Duty=Duty_40;  break;
		case Duty_40:  PWM.Duty=Duty_60;  break;
		case Duty_60:  PWM.Duty=Duty_80;  break;
		case Duty_80:  PWM.Duty=Duty_100; break;
		case Duty_100: PWM.Duty=Duty_0;   break;
		default: 		   PWM.Duty=Duty_20;  break;
	}
	
	//设置亮度
	Set_Brightness();
}

//LED设置亮度
static void Set_Brightness()
{
	//设置占空比
	switch(PWM.Duty)
	{
		case Duty_0:   CCAP0H=255; 			   break;
		case Duty_20:  CCAP0H=256*(1-0.2); break;
		case Duty_40:  CCAP0H=256*(1-0.4); break;
		case Duty_60:  CCAP0H=256*(1-0.6); break;
		case Duty_80:  CCAP0H=256*(1-0.8); break;
		case Duty_100: CCAP0H=0;   				 break;
		default: 		   CCAP0H=255; 			   break;
	}
	PWM.BackupDuty(EEPROM_Default_Addr,PWM.Duty);
}


//备份占空比
static void BackupDuty(uint16_t addr,uint8_t dat)
{
	EEPROM.ucEEPROM_Cnt=EEPROM_Cnt;
	EEPROM.ucEEPROM_Flag=false;

	while(EEPROM.ucEEPROM_Cnt--)
	{
		EEPROM.EEPROM_EraseSector(addr);
		EEPROM.EEPROM_WriteByte(addr,dat);

		//验证是否写成功
		if(EEPROM.EEPROM_ReadByte(addr)==dat)
		{
			EEPROM.ucEEPROM_Flag=true;
			break;
		}
	}
}


//读取占空比
static uint8_t ReadDuty(uint16_t addr)
{
	uint8_t Data1=0,Data2=0;
	EEPROM.ucEEPROM_Cnt=EEPROM_Cnt;
	
	while(EEPROM.ucEEPROM_Cnt--)
	{
		Data1=EEPROM.EEPROM_ReadByte(addr);
		Data2=EEPROM.EEPROM_ReadByte(addr);
		if(Data1==Data2)
		{
			break;
		}
	}
	
	if(Data1==0xFF)
		return 0;
	else
		return Data1;
	
}