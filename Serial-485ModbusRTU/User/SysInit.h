#ifndef __SYSINIT_H__
#define __SYSINIT_H__

//ϵͳ��ʼ���ṹ��ԭ��
typedef struct
{
	void (*GPIO_Init)(void);
	void (*IE_Init)(void);
	void (*Run_Indication)(void);
	void (*System_Init)(void);
}Hardware_Init_t;


extern Hardware_Init_t Hardware_Init;

#endif
