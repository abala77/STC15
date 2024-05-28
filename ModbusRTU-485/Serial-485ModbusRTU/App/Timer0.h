#ifndef __TIMER_H__
#define __TIMER_H__

//ö��һЩ���õ���ʱʱ��
typedef enum
{
	Timer0_10ms=(uint16_t)2,
	Timer0_50ms=(uint16_t)10,
	TIMER0_100mS	= (uint16_t)20,
	TIMER0_200mS	= (uint16_t)40,
	TIMER0_500mS	= (uint16_t)100,
	TIMER0_1S     = (uint16_t)200,
	TIMER0_2S     = (uint16_t)400,
	TIMER0_5S     = (uint16_t)400
}Delay_Time_t;

//��ʱ��0�ṹ��
typedef struct
{
	uint8_t volatile usMCU_RUN_Timer;//ϵͳ����ָʾ��ʱ��
	uint8_t volatile usDelay_Timer;//�趨����ʱʱ��
	
	void (*Timer0_Init)(void);//��ʱ��0��ʼ��
}Timer0_t;

extern Timer0_t Timer0;
#endif