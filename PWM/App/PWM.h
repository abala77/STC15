#ifndef __PWM_H__
#define __PWM_H__

//定义占空比
typedef enum
{
	Duty_0		=(uint8_t)0,
	Duty_20		=(uint8_t)20,
	Duty_40		=(uint8_t)40,
	Duty_60		=(uint8_t)60,
	Duty_80		=(uint8_t)80,
	Duty_100		=(uint8_t)100,
}PWM_Value_t;

//定义结构体类型
typedef struct
{
	PWM_Value_t Duty;
	void (*PWM_Init)(void);
	void (*PWM_LED_Adjust_Brightness)(void);
}PWM_t;


extern PWM_t PWM;

#endif