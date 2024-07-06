#ifndef __KEY2_H__
#define __KEY2_H__


//定义枚举类型
typedef enum
{
	STA1_KEY_Up			=(uint8_t)0x01,//按键弹起
	STA2_KEY_DownShake	=(uint8_t)0x02,//按键抖动
	STA3_KEY_Down		=(uint8_t)0x03,//按键按下
	STA4_KEY_UpShake	=(uint8_t)0x04 //弹起抖动
}STA_Machine_Status_t;

//定义结构体类型
typedef struct
{
	STA_Machine_Status_t ucSTA_Machine_Status;		//状态机状态
	uint16_t volatile 	 ucSTA_Machine_Scan_Timer;	//状态机扫描定时器
	uint16_t volatile	 usKEY2_Doule_Click_Timer;	//KEY2双击定时器
	uint16_t volatile 	 usKEY2_Press_Timer;		//KEY2长按定时器
}STA_Machine_t;


//结构体声明
extern Key_t		KEY2;
extern STA_Machine_t  STA_Machine;


#endif 