#ifndef __ADC_H__
#define __ADC_H__

//�궨��
#define ADC_POWER	0x80	//ADC��Դ����λ
#define ADC_FLAG	0x10	//ADC��ɱ�־����Ҫ�������
#define	ADC_START	0x08	//ADC��������λ����'1'��ʼת����ת����������ADC_FLAG=1


//ADCת���ٶ�
typedef enum
{
	ADC_Speed_90=0x60,
	ADC_Speed_180=0x40,
	ADC_Speed_360=0x20,
	ADC_Speed_540=0x00,
}TranSpeed_t;




//�ṹ��
typedef struct 
{
	uint16_t ADC_SimuValue;		//ADCģ������ֵ

	void (*ADC_Init)(void);					//ADC��ʼ��
	uint16_t (*ADC_Measure)(void);			//ADC����
	uint16_t (*ADC_MeasureFilter)(void);	//ADC�˲�����
}ADC_t;


extern ADC_t ADC;

#endif