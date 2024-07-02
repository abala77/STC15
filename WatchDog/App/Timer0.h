#ifndef __TIMER_H__
#define __TIMER_H__

//ö��һЩ���õ���ʱʱ��
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

//��ʱ��0�ṹ��
typedef struct
{
	uint8_t volatile usMCU_RUN_Timer;//ϵͳ����ָʾ��ʱ��
	uint16_t volatile usDelay_Timer;//�趨����ʱʱ��
	uint8_t volatile usAT_SettingTips;//WiFi��������ָʾ��
	
	void (*Timer0_Init)(void);//��ʱ��0��ʼ��
}Timer0_t;

extern Timer0_t idata Timer0;
#endif