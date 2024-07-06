#include "main.h"

#define Receive_Data_Length 100	//数据接收最大长度
#define Send_Data_Length    50	//数据发送最大长度
#define		S2TI	  bit1     //串口2发送中断标志S2CON.1
#define		S2RI	  bit0     //串口2接收中断标志S2CON.0
#define		S2TB8 	bit3     //校验位发送S2CON.3
#define		S2RB8 	bit2		 //接受数据校验位S2CON.2
#define 	S2REN   bit4     //允许串口2中断S2CON.4
#define		ET2			bit2			//T2中断允许位IE2.2

static uint8_t xdata UART2_pucRec_Buffer[Receive_Data_Length]={0};
static uint8_t xdata UART2_pucSend_Buffer[Send_Data_Length]={0};

/*--------------函数声明---------------------*/
static void UART2_Init();
static void SendByte(uint8_t dat);
static void SendString(uint8_t *string);
static void Send_Mode(void);	
static void Receive_Mode(void);
static void Protocol(void);	
static void SendArray(uint8_t *,uint8_t);

//创建串口1对象
UART_t xdata UART2=
{
	None,
	Baud_115200,
	false,
	false,
	0,

	UART2_pucSend_Buffer,
	UART2_pucRec_Buffer,
	
	UART2_Init,
	SendByte,
	SendArray,
	SendString,
	Protocol,
	
	TTL,
	Send_Mode,
	Receive_Mode,
};

//串口2初始化
static void UART2_Init()
{
	/*
	1.T2 固定位16位自动重载模式；
	2.串口2只能用T2作为波特率发生器；
	*/
	S2CON=0X50;			//8位数据，可变波特率
	AUXR |= 0x04;		//定时器时钟1T模式
	P_SW2 &=0xFE;		//选择P10和P11作为串口2引脚
	//设置波特率
	switch(UART1.BaudRate)
	{
		case Baud_4800: 	 T2L = 0xC0;T2H = 0xFD; break;
		case Baud_9600: 	 T2L = 0xE0;T2H = 0xFE; break;
		case Baud_19200:   T2L = 0x70; T2H = 0xFF; break;
		case Baud_115200:  T2L = 0xE8; T2H = 0xFF; break;
		default:           T2L = 0xC0; T2H = 0xFD; break;
	}
	
	IE2 &= ~ET2;		    //禁止定时器2中断
	AUXR |= 0x10;				//定时器2开始计时
	S2CON |= S2REN;     //允许串口2接收
	
	//清除缓冲
	Public.Memory_Clr(UART2.pucRec_Buffer,Receive_Data_Length);
	UART2.ucRec_Flag=false;
	UART2.ucRec_Cnt=0;
}

/*
  *  @name   SendByte
  *  @brief  单个字节数据发送
  *  @param  dat:一个字节数据
  *  @retval None
*/
static void SendByte(uint8_t dat)
{
	uint8_t temp_P=0;
	while(UART2.Tx_Busy_Flag==true);//等待前面数据发送完
	if(UART2.ParityBit!=None)
	{
		ACC=dat;//获取校验位P,P=1奇数个1，P=0，偶数个1
		temp_P=(temp_P+(uint8_t)P)<<3;
		switch(UART2.ParityBit)
		{
			case Odd: 
				if(temp_P==0)
				{
					S2CON |=S2TB8;
				}
				else
				{
					S2CON &=~S2TB8;
				}
				break;
			case Even: 
				if(temp_P==0)
				{
					
					S2CON &=~S2TB8;
				}
				else
				{
					S2CON |=S2TB8;
				}
				break;
		}
	}
	
	UART2.Tx_Busy_Flag=true;
	S2BUF=dat;
}

//发送字符串
static void SendString(uint8_t *str)
{
	uint8_t xdata str_ii=0;
	
	while(*(str+str_ii))
	{
		UART2.SendByte(*(str+str_ii));
		str_ii++;
	}
	while(UART2.Tx_Busy_Flag==true);
}

//发送数组
static void SendArray(uint8_t *array,uint8_t LEN)
{
	while(LEN--)
	{
		UART2.SendByte(*(array++));
	}
	while(UART2.Tx_Busy_Flag==true);
}



//串口协议
static void Protocol()
{
	bit Receive_8_Byte=1;//默认接收到了8个字节数据
	if(UART2.ucRec_Flag==true)
	{
		//过滤干扰数据0
		if(UART2.pucRec_Buffer!=0)
		{
			Timer0.usDelay_Timer=0;
			//等待接收到8个数据
			while(UART2.ucRec_Cnt<8)
			{
				if(Timer0.usDelay_Timer>=TIMER0_100ms)//100ms没接收完8个字节，跳出循环
				{
					Receive_8_Byte=0;
					break;
				}
			}
			if(Receive_8_Byte)
			{
				Modbus.Protocol_Analysis(&UART2);//解析协议
			}
			else
			{
				#if TestPrint
					UART1.RS485_Send_Mode();
					printf("Not have 8 Byte！！");
					UART1.RS485_Receive_Mode();
				#endif 
			}
		}
		
		Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
	}
}

//发送模式
static void Send_Mode()
{
	
}

//接收模式
static void Receive_Mode()
{
	
}



/*
  *  @name   uart1_isr
  *  @brief  串口2中断服务函数
  *  @param  None
  *  @retval None
*/
void uart2_isr() interrupt 8
{
	//3个缓存变量不能少
	uint8_t idata temp_SBUF;
	bit volatile idata temp_RB8;
	bit volatile idata temp_p;
	
	//发送标志
	if(S2CON & S2TI)
	{
		S2CON &= ~S2TI;//清标志
		UART2.Tx_Busy_Flag=false;
	}
	
	//接收标志
	if(S2CON & S2RI)
	{
		S2CON &= ~S2RI;		//清标志
		temp_SBUF=S2BUF;	//缓存数据
		temp_RB8=(bit)((S2CON & S2RB8)>>2);		//校验位
		
		//计算校验
		ACC=temp_SBUF;
		temp_p=P;
		
		if(UART2.ucRec_Cnt<Receive_Data_Length)//还有缓冲位置
		{
			switch(UART2.ParityBit)
			{
				case Odd:
					//如果P=1，前面8位的1个数是奇数，那么RB8应该=0
					if(temp_p==~temp_RB8)
					{
						UART2.pucRec_Buffer[UART2.ucRec_Cnt++]=temp_SBUF;
					}
					break;
				case Even:
					if(temp_p==temp_RB8)
					{
						UART2.pucRec_Buffer[UART2.ucRec_Cnt++]=temp_SBUF;
					}
					break;
				case None:
					UART2.pucRec_Buffer[UART2.ucRec_Cnt++]=temp_SBUF;
					break;
			}
		}
		UART2.ucRec_Flag=true;
	}
}

