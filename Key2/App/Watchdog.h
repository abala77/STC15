#ifndef __WHATCHDOG_H__
#define __WHATCHDOG_H__

//定义结构体
typedef struct
{
	void (*Init)(void);											//初始化看门狗
	
	uint8_t (*Get_WatchdogStatus)(void);		//获取看门狗当前状态=1开启，=0关闭
	void (*Start)(void);										//启动看门狗
	void (*Close)(void);										//关闭看门狗
	void (*Clear)(void);										//重置看门狗
}Watchdog_t;

extern Watchdog_t Watchdog;

#endif