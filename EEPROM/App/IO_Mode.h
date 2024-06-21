#ifndef __IO_MODE_H__
#define __IO_MODE_H__


//IO口模式选择
typedef struct
{
	void (*Push_Pull)(void);//推挽
	void (*Open_Drain)(void);//开漏
	void (*Floating_Input)(void);//高阻输入
	void (*Quasi_bidirectional)(void);//准双向口
}IO_Mode_t;

//声明IO口模式
extern IO_Mode_t IO_Mode;

#endif