#include "main.h"


uint16_t xdata Modbus_Regist[Modbus_Regist_LEN]={0};


static void Delay1ms();
static void Delayms(int);

static void Error_Handler(void);
static void softReset(void);
static void Led_Flip(void);
static void Memory_Clr(uint8_t* ,uint8_t );

//结构体
Public_t Public=
{
	Delayms,
	Error_Handler,
	softReset,
	Led_Flip,
	Memory_Clr
};


//错误处理
static void Error_Handler()
{
	UART1.SendString("WiFi Error\n");
}

//软件复位
static void softReset()
{
	IAP_CONTR |= 0x20;
}

//LED翻转
static void Led_Flip()
{
	P34=~P34;
}

//清除缓存
static void Memory_Clr(uint8_t* pucString,uint8_t LEN)
{
	memset(pucString,0,LEN);
	Public.Delayms(10);
}


//延时n毫秒
static void Delayms(int ms)
{
	int k;
	for(k=0;k<ms;k++)
	{
		Delay1ms();
	}
}

//1ms延时
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

