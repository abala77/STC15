#include "main.h"


uint16_t xdata Modbus_Regist[Modbus_Regist_LEN]={0};


static void Delay1ms();
static void Delayms(int);
void Memory_Clr(uint8_t*,uint8_t);

Public_t Public=
{
	Delayms,
	Memory_Clr
};


void Memory_Clr(uint8_t* pucCLR,uint8_t LEN)
{
	memset(pucCLR,0,LEN);
}



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