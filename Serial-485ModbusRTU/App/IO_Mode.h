#ifndef __IO_MODE_H__
#define __IO_MODE_H__


//IO��ģʽѡ��
typedef struct
{
	void (*Push_Pull)(void);//����
	void (*Open_Drain)(void);//��©
	void (*Floating_Input)(void);//��������
	void (*Quasi_bidirectional)(void);//׼˫���
}IO_Mode_t;

//����IO��ģʽ
extern IO_Mode_t IO_Mode;

#endif