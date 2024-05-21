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
	SCON=0X50;	//ģʽ1�������������
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

//����1�ж�
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