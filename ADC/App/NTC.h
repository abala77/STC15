#ifndef __NTC_H__
#define __NTC_H__


typedef struct 
{
	float Recent_Tempurature;		//��ǰ�¶�
	
	void (*Formula_CalcTemp)(void);		//��ʽ�����¶�
	void (*Form_CheckTemp)(void);			//�������¶�
}NTC_t;


extern NTC_t NTC;

#endif