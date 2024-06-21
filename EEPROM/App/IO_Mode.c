#include "main.h"

static void Push_Pull();
static void Open_Drain();
static void Floating_Input();
static void Quasi_bidirectional();


IO_Mode_t IO_Mode=
{
	Push_Pull,//推挽
	Open_Drain,//开漏
	Floating_Input,//高阻输入
	Quasi_bidirectional//准双向口
};

//P34设置成推挽输出
static void Push_Pull()
{
	//P34推挽输出，P32高阻输入,P33准双向口
	P3M1=0x04;
	P3M0=0x10;
	while(true)
	{
		if(P33==0)
		{
			P34=1;
		}
		else if(P33==1)
		{
			P34=0;
		}
	}
}

//P32设置成开漏输出
static void Open_Drain()
{
	//P34推挽输出，P32开漏输入,P33准双向口
	P3M1=0x04;
	P3M0=0x14;
	while(true)
	{
		if(P32==0)
		{
			P34=1;
		}
		else if(P32==1)
		{
			P34=0;
		}
	}
}

//P32设置成高阻输入
static void Floating_Input()
{
	//P34推挽输出，P32高阻输入,P33准双向口
	P3M1=0x04;
	P3M0=0x10;
	while(true)
	{
		if(P32==0)
		{
			P34=1;
		}
		else if(P32==1)
		{
			P34=0;
		}
	}
}


//P33设置成准双向口输入（输出只能使用灌电流，电路不支持。拉电流太小）
static void Quasi_bidirectional()
{
	//P34推挽输出，P32高阻输入,P33准双向口
	P3M1=0x04;
	P3M0=0x10;
	P33=1;//输出高电平关闭下面的MOS
	while(true)
	{
		if(P33==0)
		{
			P34=1;
		}
		else if(P33==1)
		{
			P34=0;
		}
	}
}