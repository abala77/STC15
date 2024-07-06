#include "main.h"

//�궨��
#define WDT_FLAG	bit7
#define EN_WDT		bit5
#define CLR_WDT		bit4
#define IDLE_WDT	bit3
#define PS2				bit2
#define PS1				bit1
#define PS0				bit0

//��������
static void Init(void);											//��ʼ�����Ź�
static uint8_t Get_WatchdogStatus(void);		//��ȡ���Ź���ǰ״̬=1������=0�ر�
static void Start(void);										//�������Ź�
static void Close(void);										//�رտ��Ź�
static void Clear(void);										//���ÿ��Ź�

//���Ź�ʵ����
Watchdog_t Watchdog=
{
	Init,
	Get_WatchdogStatus,
	Start,
	Close,
  Clear
};

//��ʼ�����Ź�
static void Init()
{
	WDT_CONTR=(~WDT_FLAG)&CLR_WDT|IDLE_WDT|(PS2|PS1|PS0);
	Watchdog.Start();
}

//��ȡ���Ź���ǰ״̬=1������=0�ر�
static uint8_t Get_WatchdogStatus()
{
	if(WDT_CONTR & EN_WDT)
		return 1;
	else
		return 0;
}

//�������Ź�
static void Start()
{
	WDT_CONTR |= EN_WDT;
}	

//�رտ��Ź�
static void Close()
{
	WDT_CONTR &= (~EN_WDT);
}	

//���㿴�Ź�
static void Clear()
{
	WDT_CONTR |= CLR_WDT;
}	