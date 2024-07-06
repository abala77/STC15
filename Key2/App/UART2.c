#include "main.h"

#define Receive_Data_Length 100	//���ݽ�����󳤶�
#define Send_Data_Length    50	//���ݷ�����󳤶�
#define		S2TI	  bit1     //����2�����жϱ�־S2CON.1
#define		S2RI	  bit0     //����2�����жϱ�־S2CON.0
#define		S2TB8 	bit3     //У��λ����S2CON.3
#define		S2RB8 	bit2		 //��������У��λS2CON.2
#define 	S2REN   bit4     //������2�ж�S2CON.4
#define		ET2			bit2			//T2�ж�����λIE2.2

static uint8_t xdata UART2_pucRec_Buffer[Receive_Data_Length]={0};
static uint8_t xdata UART2_pucSend_Buffer[Send_Data_Length]={0};

/*--------------��������---------------------*/
static void UART2_Init();
static void SendByte(uint8_t dat);
static void SendString(uint8_t *string);
static void Send_Mode(void);	
static void Receive_Mode(void);
static void Protocol(void);	
static void SendArray(uint8_t *,uint8_t);

//��������1����
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

//����2��ʼ��
static void UART2_Init()
{
	/*
	1.T2 �̶�λ16λ�Զ�����ģʽ��
	2.����2ֻ����T2��Ϊ�����ʷ�������
	*/
	S2CON=0X50;			//8λ���ݣ��ɱ䲨����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	P_SW2 &=0xFE;		//ѡ��P10��P11��Ϊ����2����
	//���ò�����
	switch(UART1.BaudRate)
	{
		case Baud_4800: 	 T2L = 0xC0;T2H = 0xFD; break;
		case Baud_9600: 	 T2L = 0xE0;T2H = 0xFE; break;
		case Baud_19200:   T2L = 0x70; T2H = 0xFF; break;
		case Baud_115200:  T2L = 0xE8; T2H = 0xFF; break;
		default:           T2L = 0xC0; T2H = 0xFD; break;
	}
	
	IE2 &= ~ET2;		    //��ֹ��ʱ��2�ж�
	AUXR |= 0x10;				//��ʱ��2��ʼ��ʱ
	S2CON |= S2REN;     //������2����
	
	//�������
	Public.Memory_Clr(UART2.pucRec_Buffer,Receive_Data_Length);
	UART2.ucRec_Flag=false;
	UART2.ucRec_Cnt=0;
}

/*
  *  @name   SendByte
  *  @brief  �����ֽ����ݷ���
  *  @param  dat:һ���ֽ�����
  *  @retval None
*/
static void SendByte(uint8_t dat)
{
	uint8_t temp_P=0;
	while(UART2.Tx_Busy_Flag==true);//�ȴ�ǰ�����ݷ�����
	if(UART2.ParityBit!=None)
	{
		ACC=dat;//��ȡУ��λP,P=1������1��P=0��ż����1
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

//�����ַ���
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

//��������
static void SendArray(uint8_t *array,uint8_t LEN)
{
	while(LEN--)
	{
		UART2.SendByte(*(array++));
	}
	while(UART2.Tx_Busy_Flag==true);
}



//����Э��
static void Protocol()
{
	bit Receive_8_Byte=1;//Ĭ�Ͻ��յ���8���ֽ�����
	if(UART2.ucRec_Flag==true)
	{
		//���˸�������0
		if(UART2.pucRec_Buffer!=0)
		{
			Timer0.usDelay_Timer=0;
			//�ȴ����յ�8������
			while(UART2.ucRec_Cnt<8)
			{
				if(Timer0.usDelay_Timer>=TIMER0_100ms)//100msû������8���ֽڣ�����ѭ��
				{
					Receive_8_Byte=0;
					break;
				}
			}
			if(Receive_8_Byte)
			{
				Modbus.Protocol_Analysis(&UART2);//����Э��
			}
			else
			{
				#if TestPrint
					UART1.RS485_Send_Mode();
					printf("Not have 8 Byte����");
					UART1.RS485_Receive_Mode();
				#endif 
			}
		}
		
		Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
	}
}

//����ģʽ
static void Send_Mode()
{
	
}

//����ģʽ
static void Receive_Mode()
{
	
}



/*
  *  @name   uart1_isr
  *  @brief  ����2�жϷ�����
  *  @param  None
  *  @retval None
*/
void uart2_isr() interrupt 8
{
	//3���������������
	uint8_t idata temp_SBUF;
	bit volatile idata temp_RB8;
	bit volatile idata temp_p;
	
	//���ͱ�־
	if(S2CON & S2TI)
	{
		S2CON &= ~S2TI;//���־
		UART2.Tx_Busy_Flag=false;
	}
	
	//���ձ�־
	if(S2CON & S2RI)
	{
		S2CON &= ~S2RI;		//���־
		temp_SBUF=S2BUF;	//��������
		temp_RB8=(bit)((S2CON & S2RB8)>>2);		//У��λ
		
		//����У��
		ACC=temp_SBUF;
		temp_p=P;
		
		if(UART2.ucRec_Cnt<Receive_Data_Length)//���л���λ��
		{
			switch(UART2.ParityBit)
			{
				case Odd:
					//���P=1��ǰ��8λ��1��������������ôRB8Ӧ��=0
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

