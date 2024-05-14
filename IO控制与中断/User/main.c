#include "main.h"

static void INT0_Init();

int main(void)
{
	IO_Mode.Open_Drain();
	return 0;
}


//中断初始化
static void INT0_Init()
{  
    IT0 = 1; // 设置INT0为下降沿触发  
    EX0 = 1; // 打开外部中断0  
    EA = 1;  // 打开全局中断
}  

//KEY1(P32)中断服务函数
void INT0_isr() interrupt 0
{
	P34=~P34;
}