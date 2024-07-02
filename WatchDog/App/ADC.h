#ifndef __ADC_H__
#define __ADC_H__

//宏定义
#define ADC_POWER	0x80	//ADC电源控制位
#define ADC_FLAG	0x10	//ADC完成标志，需要软件清零
#define	ADC_START	0x08	//ADC启动控制位，置'1'开始转换，转换结束后置ADC_FLAG=1


//ADC转换速度
typedef enum
{
	ADC_Speed_90=0x60,
	ADC_Speed_180=0x40,
	ADC_Speed_360=0x20,
	ADC_Speed_540=0x00,
}TranSpeed_t;




//结构体
typedef struct 
{
	uint16_t ADC_SimuValue;		//ADC模拟输入值

	void (*ADC_Init)(void);					//ADC初始化
	uint16_t (*ADC_Measure)(void);			//ADC测量
	uint16_t (*ADC_MeasureFilter)(void);	//ADC滤波处理
}ADC_t;


extern ADC_t ADC;

#endif