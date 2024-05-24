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

//��������1����
UART_t UART1=
{
	Even,
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

//����1��ʼ��
static void UART1_Init()
{
	if(UART1.ParityBit==None)
	{
		flag11=66;
		SCON=0X50;	//ģʽ1��8λ���ݣ������������
	}
	else
	{
		flag11=77;
		SCON=0XD0;	//ģʽ3��9λ���ݣ������������
	}
	
	PCON=0X00;	//�����ʲ��ӱ�
	TMOD&=0X0F;	//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	//AUXR1&=0x3F;	//����1ӳ����P30,P31
	//����1ӳ����P36,P37
	AUXR1&=~bit7;
	AUXR1|=bit6;
	
	//���ò�����
	switch(UART1.BaudRate)
	{
		case Baud_4800: TL1 = 0xC0;TH1 = 0xFD; break;
		case Baud_9600: TL1 = 0xE0;TH1 = 0xFE; break;
		case Baud_19200:   TL1 = 0x70; TH1 = 0xFF; break;
		case Baud_115200:  TL1 = 0xE8; TH1 = 0xFF; break;
		default:           TL1 = 0xC0; TH1 = 0xFD; break;
	}

	ET1 = 0;		    //��ֹ��ʱ��1�ж�
	TR1 = 1;		    //������ʱ��1
	REN  = 1;      //������1����
}

/*
  *  @name   SendByte
  *  @brief  �����ֽ����ݷ���
  *  @param  dat:һ���ֽ�����
  *  @retval None
*/
static void SendByte(uint8_t dat)
{
	bit temp_P;
	while(UART1.Tx_Busy_Flag==true);//�ȴ�ǰ�����ݷ�����
	if(UART1.ParityBit!=None)
	{
		ACC=dat;//��ȡУ��λP,P=1������1��P=0��ż����1
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

//�����ַ���
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

//putchar�����ض���
extern char putchar(char C)
{
  UART1.SendByte((uint8_t)C);
  return C;
}

//��485����Ϊ����ģʽ
static void Send_Mode()
{
	RS485_Mode_Switch=RS485_setSendMode;
	Public.Delayms(2);//�ȴ�Ӳ����·��Ӧ����
}

//��485����Ϊ����ģʽ
static void Receive_Mode()
{
	Public.Delayms(2);//�ȴ�ǰ������ݷ�����
	RS485_Mode_Switch=RS485_setReceiveMode;
}

//����Э��
static void Protocol()
{
	
}

/*
  *  @name   uart1_isr
  *  @brief  ����1�жϷ�����
  *  @param  None
  *  @retval None
*/
void uart1_isr() interrupt 4
{
	//3���������������
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
		if(UART1.ucRec_Cnt<Receive_Data_Length)
		{
			switch(UART1.ParityBit)
			{
				case Odd:
					//���P=1��ǰ��8λ��1��������������ôRB8Ӧ��=0
					if(temp_p==~temp_RB8)
					{
						UART1.Receive_Data[UART1.ucRec_Cnt++]=temp_SBUF;
					}
					break;
				case Even:
					if(temp_p==temp_RB8)
					{
						flag11=temp_RB8;
						UART1.Receive_Data[UART1.ucRec_Cnt++]=temp_SBUF;
					}
					break;
				case None:
					UART1.Receive_Data[UART1.ucRec_Cnt++]=temp_SBUF;
					break;
			}
		}
		UART1.Rx_Busy_Flag=false;
	}
}