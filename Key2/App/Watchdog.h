#ifndef __WHATCHDOG_H__
#define __WHATCHDOG_H__

//����ṹ��
typedef struct
{
	void (*Init)(void);											//��ʼ�����Ź�
	
	uint8_t (*Get_WatchdogStatus)(void);		//��ȡ���Ź���ǰ״̬=1������=0�ر�
	void (*Start)(void);										//�������Ź�
	void (*Close)(void);										//�رտ��Ź�
	void (*Clear)(void);										//���ÿ��Ź�
}Watchdog_t;

extern Watchdog_t Watchdog;

#endif