#include "main.h"

#define NTC_CHAN 0x04


void ADC_Init(void); 				//ADC��ʼ��
uint16_t ADC_Measure(void);			//ADC����
uint16_t ADC_MeasureFilter(void);	//ADC�˲�����

ADC_t ADC= 
{
	0,					

	ADC_Init,			
	ADC_Measure,		
	ADC_MeasureFilter	
};

//ADC��ʼ��
void ADC_Init()
{
	P1ASF=bit4;		//����P1��ΪAD��
	CLK_DIV |=bit5;	//����ADC_RES�Ÿ�2λ��ADC_RESL�ŵ�8λ
	EADC=0;			//��ֹADC�ж�
	ADC_CONTR=NTC_CHAN|ADC_POWER|ADC_Speed_180;	//ѡ��ͨ�������硢�����ٶ�
	Public.Delayms(2);	//�ȴ���Դ�ȶ�
}


//ADC����
uint16_t ADC_Measure()
{
	uint16_t result=0;

	//���㻻����
	ADC_RES=0;
	ADC_RESL=0;

	//����ɼ���ɱ�־
	ADC_CONTR&=(~ADC_FLAG);

	ADC_CONTR|=ADC_START;
	_nop_();							//�ȴ��ɼ����
	_nop_();
	_nop_();
	_nop_();

	while(!(ADC_CONTR & ADC_FLAG));		//�ȴ�ADCת�����
	ADC_CONTR &= ~ADC_FLAG;				//����ɼ���ɱ�־

	//��ȡת����� ->ADC_RES��2λ + ADC_RESL��8λ
	result=ADC_RES;
	result=(result<<8)+ADC_RESL;

	return result;
}



//ADC�˲�����
uint16_t ADC_MeasureFilter()
{
	uint16_t ADC_Result=0,ADC_Return=0,ADC_min,ADC_max,ADC_temp;
	uint8_t i,j;

	for(i=0;i<16;i++)
	{
		ADC_Result=0;
		ADC_min=ADC_max=ADC.ADC_Measure();

		//8��ȡ�����Ѽ�ֵȥ����ȡƽ��ֵ
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

