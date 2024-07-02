#include "main.h"

static void Push_Pull();
static void Open_Drain();
static void Floating_Input();
static void Quasi_bidirectional();


IO_Mode_t IO_Mode=
{
	Push_Pull,//����
	Open_Drain,//��©
	Floating_Input,//��������
	Quasi_bidirectional//׼˫���
};

//P34���ó��������
static void Push_Pull()
{
	//P34���������P32��������,P33׼˫���
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

//P32���óɿ�©���
static void Open_Drain()
{
	//P34���������P32��©����,P33׼˫���
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

//P32���óɸ�������
static void Floating_Input()
{
	//P34���������P32��������,P33׼˫���
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


//P33���ó�׼˫������루���ֻ��ʹ�ù��������·��֧�֡�������̫С��
static void Quasi_bidirectional()
{
	//P34���������P32��������,P33׼˫���
	P3M1=0x04;
	P3M0=0x10;
	P33=1;//����ߵ�ƽ�ر������MOS
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