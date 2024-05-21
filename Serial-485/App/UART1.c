#include "main.h"

#define RS485_Mode_Switch P20
#define RS485_setSendMode 1
#define RS485_setReceiveMode 0
#define Receive_Data_Length 10

static uint8_t idata Receive_Data[Receive_Data_Length]={0};

static void UART1_Init();
static void SendByte(uint8_t dat);
static void SendString(uint8_t *string);
static void Send_Mode(void);	
static void Receive_Mode(void);
static void Protocol(void);	

//创建串口1对象
UART_t UART1=
{
	Baud_115200,
	false,
	false,
	0,
	
	Receive_Data,
	
	UART1_Init,
	SendByte,
	SendString,
	Protocol,
	Send_Mode,
	Receive_Mode,
};

//串口1初始化
static void UART1_Init()
{
	SCON=0X50;	//模式1，允许接收数据
	PCON=0X00;	//波特率不加倍
	TMOD&=0X0F;	//设定定时器1为16位自动重装方式
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	//AUXR1&=0x3F;	//串口1映射至P30,P31
	//串口1映射至P36,P37
	AUXR1&=~bit7;
	AUXR1|=bit6;
	
	//设置波特率
	switch(UART1.BaudRate)
	{
		case Baud_4800: TL1 = 0xC0;TH1 = 0xFD; break;
		case Baud_9600: TL1 = 0xE0;TH1 = 0xFE; break;
		case Baud_19200:   TL1 = 0x70; TH1 = 0xFF; break;
		case Baud_115200:  TL1 = 0xE8; TH1 = 0xFF; break;
		default:           TL1 = 0xC0; TH1 = 0xFD; break;
	}

	ET1 = 0;		    //禁止定时器1中断
	TR1 = 1;		    //启动定时器1
	REN  = 1;      //允许串口1接收
}

//单个字节数据发送
static void SendByte(uint8_t dat)
{
	while(UART1.Tx_Busy_Flag==true);//等待前面数据发送完
	UART1.Tx_Busy_Flag=true;
	SBUF=dat;
}

//发送字符串
static void SendString(uint8_t *str)
{
	UART1.RS485_Send_Mode();
	while(*str)
	{
		UART1.SendByte(*(str++));
	}
	while(UART1.Tx_Busy_Flag==true);
	Public.Delayms(1);
	UART1.RS485_Receive_Mode();
}

//putchar函数重定义
extern char putchar(char C)
{
  UART1.SendByte((uint8_t)C);
  return C;
}

//将485设置为发送模式
static void Send_Mode()
{
	RS485_Mode_Switch=RS485_setSendMode;
	Public.Delayms(2);//等待硬件电路反应过来
}

//将485设置为接收模式
static void Receive_Mode()
{
	Public.Delayms(2);//等待前面的数据发送完
	RS485_Mode_Switch=RS485_setReceiveMode;
}

//串口协议
static void Protocol()
{
	
}

//串口1中断
void uart1_isr() interrupt 4
{
	if(TI==1)
	{
		TI=(bit)0;
		UART1.Tx_Busy_Flag=false;
	}

	if(RI==1)
	{
		RI=0;
		if(UART1.ucRec_Cnt<Receive_Data_Length)
		{
			UART1.Receive_Data[UART1.ucRec_Cnt++]=SBUF;
		}
		UART1.Rx_Busy_Flag=false;
		flag11=flag11+1;
	}
}