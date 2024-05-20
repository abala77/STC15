#include "main.h"

#define Receive_Data_Length 10

static uint8_t idata Receive_Data[Receive_Data_Length]={0};

static void UART1_Init();
static void SendByte(uint8_t dat);
static void SendString(uint8_t *string);

//��������1����
UART_t UART1=
{
	Baud_115200,
	false,
	false,
	0,
	
	Receive_Data,
	
	UART1_Init,
	SendByte,
	SendString
};

//����1��ʼ��
static void UART1_Init()
{
	SCON=0X50;	//ģʽ1�������������
	PCON=0X00;	//�����ʲ��ӱ�
	TMOD&=0X0F;	//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	AUXR1&=0x3F;	////����1ӳ����P30,P31
	
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
	REN   = 1;      //������1����
}

//�����ֽ����ݷ���
static void SendByte(uint8_t dat)
{
	while(UART1.Tx_Busy_Flag==true);//�ȴ�ǰ�����ݷ�����
	UART1.Tx_Busy_Flag=true;
	SBUF=dat;
}

//�����ַ���
static void SendString(uint8_t *str)
{
	//uint8_t *temp=str;
	while(*str)
	{
		UART1.SendByte(*(str++));
	}
	while(UART1.Tx_Busy_Flag==true);
}

//putchar�����ض���
char putchar(char C)
{
  UART1.SendByte((uint8_t)C);
  return C;
}

//�����ж�
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
	}
}