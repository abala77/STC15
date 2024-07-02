#ifndef __NTC_H__
#define __NTC_H__


typedef struct 
{
	float Recent_Tempurature;		//当前温度
	
	void (*Formula_CalcTemp)(void);		//公式计算温度
	void (*Form_CheckTemp)(void);			//表格计算温度
}NTC_t;


extern NTC_t NTC;

#endif