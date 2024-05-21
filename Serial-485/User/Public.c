#include "main.h"

static void Delay1ms();
static void Delayms(int);

Public_t Public=
{
	Delayms
};


//1ms—” ±
static void Delay1ms()		//@11.0592MHz
{
	unsigned char data i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

//—” ±n∫¡√Î
static void Delayms(int ms)
{
	int k;
	for(k=0;k<ms;k++)
	{
		Delay1ms();
	}
}