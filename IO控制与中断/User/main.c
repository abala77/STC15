#include "main.h"

static void INT0_Init();

int main(void)
{
	IO_Mode.Open_Drain();
	return 0;
}


//�жϳ�ʼ��
static void INT0_Init()
{  
    IT0 = 1; // ����INT0Ϊ�½��ش���  
    EX0 = 1; // ���ⲿ�ж�0  
    EA = 1;  // ��ȫ���ж�
}  

//KEY1(P32)�жϷ�����
void INT0_isr() interrupt 0
{
	P34=~P34;
}