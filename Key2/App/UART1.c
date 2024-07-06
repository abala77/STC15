#include "main.h"

#define RS485_Mode_Switch P20
#define RS485_setSendMode    1
#define RS485_setReceiveMode 0
#define Receive_Data_Length 50	//数据接收最大长度
#define Send_Data_Length    50	//数据发送最大长度

static uint8_t xdata Receive_Data[Receive_Data_Length]={0};
static uint8_t xdata SendBuf_Data[Send_Data_Length]={0};

/*--------------函数声明---------------------*/
static void UART1_Init();
static void SendByte(uint8_t dat);
static void SendString(uint8_t *string);
static void Send_Mode(void);	
static void Receive_Mode(void);
static void Protocol(void);	
static void SendArray(uint8_t *,uint8_t);

//创建串口1对象
UART_t UART1=
{
	None,
	Baud_115200,
	false,
	false,
	0,

	SendBuf_Data,
	Receive_Data,
	
	UART1_Init,
	SendByte,
	SendArray,
	SendString,
	Protocol,
	
	RS485,
	Send_Mode,
	Receive_Mode,
};

//串口1初始化
static void UART1_Init()
{
	//是否校验
	switch(UART1.ParityBit)
	{
		case None:
			SCON=0X50;	//模式1，8位数据，允许接收数据
			break;
		default:
			SCON=0XD0;	//模式3，9位数据，允许接收数据
			break;
	}
	//接口类型
	switch(UART1.Interface_Type)
	{
		case TTL:
		case RS232:
			AUXR1&=0x3F;	//串口1映射至P30,P31
			break;
		case RS485:
			//串口1映射至P36,P37
			AUXR1&=~bit7;
			AUXR1|=bit6;
			break;
	}
	
	PCON=0X00;	//波特率不加倍
	TMOD&=0X0F;	//设定定时器1为16位自动重装方式
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	
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
	REN  = 1;       //允许串口1接收
	
	//清除缓冲
	Public.Memory_Clr(UART1.pucRec_Buffer,Receive_Data_Length);
	UART1.ucRec_Flag=false;
	UART1.ucRec_Cnt=0;
}

/*
  *  @name   SendByte
  *  @brief  单个字节数据发送
  *  @param  dat:一个字节数据
  *  @retval None
*/
static void SendByte(uint8_t dat)
{
	bit temp_P;
	while(UART1.Tx_Busy_Flag==true);//等待前面数据发送完
	//如果有校验需要设置第九位
	if(UART1.ParityBit!=None)
	{
		ACC=dat;//获取校验位P,P=1奇数个1，P=0，偶数个1
		temp_P=P;
		switch(UART1.ParityBit)
		{
			case Odd: TB8=~temp_P;break;
			case Even: TB8=temp_P;break;
		}
	}
	
	UART1.Tx_Busy_Flag=true;
	SBUF=dat;
}

//发送字符串
static void SendString(uint8_t *str1)
{
	uint16_t idata str_i=0;
	
	//485模式才需要设置
	if(UART1.Interface_Type==RS485)
	{
		UART1.RS485_Send_Mode();
	}

	while(*(str1+str_i))
	{
		UART1.SendByte(*(str1+str_i));
		str_i++;
	}
	while(UART1.Tx_Busy_Flag==true);
	
	if(UART1.Interface_Type==RS485)
	{
		Public.Delayms(1);
		UART1.RS485_Receive_Mode();
	}
}

//发送数组
static void SendArray(uint8_t *array,uint8_t LEN)
{
	
	if(UART1.Interface_Type==RS485)
	{
		UART1.RS485_Send_Mode();
	}
	
	while(LEN--)
	{
		UART1.SendByte(*(array++));
	}
	while(UART1.Tx_Busy_Flag==true);

	if(UART1.Interface_Type==RS485)
	{
		Public.Delayms(1);
		UART1.RS485_Receive_Mode();
	}
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
	bit Receive_8_Byte=1;//默认接收到了8个字节数据
	if(UART1.ucRec_Flag==true)
	{
		//过滤干扰数据0
		if(UART1.pucRec_Buffer!=0)
		{
			Timer0.usDelay_Timer=0;
			//等待接收到8个数据
			while(UART1.ucRec_Cnt<8)
			{
				if(Timer0.usDelay_Timer>=TIMER0_100ms)//100ms没接收完8个字节，跳出循环
				{
					Receive_8_Byte=0;
					break;
				}
			}
			if(Receive_8_Byte)
			{
				Modbus.Protocol_Analysis(&UART1);//解析协议
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
		
		Public.Memory_Clr(UART1.pucRec_Buffer,UART1.ucRec_Cnt);
		UART1.ucRec_Flag=false;
		UART1.ucRec_Cnt=0;
	}
}

/*
  *  @name   uart1_isr
  *  @brief  串口1中断服务函数
  *  @param  None
  *  @retval None
*/
void uart1_isr() interrupt 4 using 1
{
	//3个缓存变量不能少
	uint8_t temp_SBUF;
	bit temp_RB8;
	bit temp_p;
	if(TI==1)
	{
		TI=(bit)0;
		UART1.Tx_Busy_Flag=false;
	}

	if(RI==1)
	{
		RI=0;
		temp_SBUF=SBUF;
		temp_RB8=RB8;
		ACC=temp_SBUF;
		temp_p=P;
		if(UART1.ucRec_Cnt<Receive_Data_Length)//还有缓冲位置
		{
			switch(UART1.ParityBit)
			{
				case Odd:
					//如果P=1，前面8位的1个数是奇数，那么RB8应该=0
					if(temp_p==~temp_RB8)
					{
						UART1.pucRec_Buffer[UART1.ucRec_Cnt++]=temp_SBUF;
					}
					break;
				case Even:
					if(temp_p==temp_RB8)
					{
						UART1.pucRec_Buffer[UART1.ucRec_Cnt++]=temp_SBUF;
					}
					break;
				case None:
					UART1.pucRec_Buffer[UART1.ucRec_Cnt++]=temp_SBUF;
					break;
			}
		}
		UART1.ucRec_Flag=true;
	}
}