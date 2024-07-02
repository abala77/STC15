#include "main.h"

//宏定义
#define WDT_FLAG	bit7
#define EN_WDT		bit5
#define CLR_WDT		bit4
#define IDLE_WDT	bit3
#define PS2				bit2
#define PS1				bit1
#define PS0				bit0

//函数声明
static void Init(void);											//初始化看门狗
static uint8_t Get_WatchdogStatus(void);		//获取看门狗当前状态=1开启，=0关闭
static void Start(void);										//启动看门狗
static void Close(void);										//关闭看门狗
static void Clear(void);										//重置看门狗

//看门狗实例化
Watchdog_t Watchdog=
{
	Init,
	Get_WatchdogStatus,
	Start,
	Close,
  Clear
};

//初始化看门狗
static void Init()
{
	WDT_CONTR=(~WDT_FLAG)&CLR_WDT|IDLE_WDT|(PS2|PS1|PS0);
	Watchdog.Start();
}

//获取看门狗当前状态=1开启，=0关闭
static uint8_t Get_WatchdogStatus()
{
	if(WDT_CONTR & EN_WDT)
		return 1;
	else
		return 0;
}

//启动看门狗
static void Start()
{
	WDT_CONTR |= EN_WDT;
}	

//关闭看门狗
static void Close()
{
	WDT_CONTR &= (~EN_WDT);
}	

//清零看门狗
static void Clear()
{
	WDT_CONTR |= CLR_WDT;
}	