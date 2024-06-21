#ifndef __TIMER_H__
#define __TIMER_H__

//枚举一些常用的延时时间
typedef enum
{
	TIMER0_10ms=(uint16_t)2,
	TIMER0_50ms=(uint16_t)10,
	TIMER0_100ms	= (uint16_t)20,
	TIMER0_200ms	= (uint16_t)40,
	TIMER0_500ms	= (uint16_t)100,
	TIMER0_1s     = (uint16_t)200,
	TIMER0_2s     = (uint16_t)400,
	TIMER0_3s     = (uint16_t)600,
	TIMER0_4s			= (uint16_t)800,
	TIMER0_5s     = (uint16_t)1000,
	TIMER0_3m	=(uint16_t)36000
}Delay_Time_t;

//定时器0结构体
typedef struct
{
	uint8_t volatile usMCU_RUN_Timer;//系统运行指示灯时间
	uint16_t volatile usDelay_Timer;//设定的延时时间
	uint8_t volatile usAT_SettingTips;//WiFi正在配置指示灯
	
	void (*Timer0_Init)(void);//定时器0初始化
}Timer0_t;

extern Timer0_t idata Timer0;
#endif