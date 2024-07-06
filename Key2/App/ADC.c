#include "main.h"

#define NTC_CHAN 0x04


void ADC_Init(void); 				//ADC初始化
uint16_t ADC_Measure(void);			//ADC测量
uint16_t ADC_MeasureFilter(void);	//ADC滤波处理

ADC_t ADC= 
{
	0,					

	ADC_Init,			
	ADC_Measure,		
	ADC_MeasureFilter	
};

//ADC初始化
void ADC_Init()
{
	P1ASF=bit4;		//设置P1口为AD口
	CLK_DIV |=bit5;	//设置ADC_RES放高2位，ADC_RESL放低8位
	EADC=0;			//禁止ADC中断
	ADC_CONTR=NTC_CHAN|ADC_POWER|ADC_Speed_180;	//选择通道、给电、设置速度
	Public.Delayms(2);	//等待电源稳定
}


//ADC测量
uint16_t ADC_Measure()
{
	uint16_t result=0;

	//清零换算结果
	ADC_RES=0;
	ADC_RESL=0;

	//清零采集完成标志
	ADC_CONTR&=(~ADC_FLAG);

	ADC_CONTR|=ADC_START;
	_nop_();							//等待采集完成
	_nop_();
	_nop_();
	_nop_();

	while(!(ADC_CONTR & ADC_FLAG));		//等待ADC转换完成
	ADC_CONTR &= ~ADC_FLAG;				//清零采集完成标志

	//获取转换结果 ->ADC_RES高2位 + ADC_RESL低8位
	result=ADC_RES;
	result=(result<<8)+ADC_RESL;

	return result;
}



//ADC滤波处理
uint16_t ADC_MeasureFilter()
{
	uint16_t ADC_Result=0,ADC_Return=0,ADC_min,ADC_max,ADC_temp;
	uint8_t i,j;

	for(i=0;i<16;i++)
	{
		ADC_Result=0;
		ADC_min=ADC_max=ADC.ADC_Measure();

		//8次取样，把极值去掉，取平均值
		for(j=0;j<8;j++)
		{
			ADC_temp=ADC.ADC_Measure();
			if(ADC_temp<ADC_min)
			{
				ADC_Result+=ADC_min;
				ADC_min=ADC_temp;
			}
			else if(ADC_temp>ADC_max)
			{
				ADC_Result+=ADC_max;
				ADC_max=ADC_temp;
			}
			else
			{
				ADC_Result+=ADC_temp;
			}
		}
		ADC_Result=ADC_Result/8;
		ADC_Return+=ADC_Result;
	}

	ADC_Return /= 16;
	
	return ADC_Return;
}

