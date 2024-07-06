#include "main.h"

#define RS485_Mode_Switch P20
#define RS485_setSendMode    1
#define RS485_setReceiveMode 0
#define Receive_Data_Length 50	//���ݽ�����󳤶�
#define Send_Data_Length    50	//���ݷ�����󳤶�

static uint8_t xdata Receive_Data[Receive_Data_Length]={0};
static uint8_t xdata SendBuf_Data[Send_Data_Length]={0};

/*--------------��������---------------------*/
static void UART1_Init();
static void SendByte(uint8_t dat);
static void SendString(uint8_t *string);
static void Send_Mode(void);	
static void Receive_Mode(void);
static void Protocol(void);	
static void SendArray(uint8_t *,uint8_t);

//��������1����
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

//����1��ʼ��
static void UART1_Init()
{
	//�Ƿ�У��
	switch(UART1.ParityBit)
	{
		case None:
			SCON=0X50;	//ģʽ1��8λ���ݣ������������
			break;
		default:
			SCON=0XD0;	//ģʽ3��9λ���ݣ������������
			break;
	}
	//�ӿ�����
	switch(UART1.Interface_Type)
	{
		case TTL:
		case RS232:
			AUXR1&=0x3F;	//����1ӳ����P30,P31
			break;
		case RS485:
			//����1ӳ����P36,P37
			AUXR1&=~bit7;
			AUXR1|=bit6;
			break;
	}
	
	PCON=0X00;	//�����ʲ��ӱ�
	TMOD&=0X0F;	//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	
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
	REN  = 1;       //������1����
	
	//�������
	Public.Memory_Clr(UART1.pucRec_Buffer,Receive_Data_Length);
	UART1.ucRec_Flag=false;
	UART1.ucRec_Cnt=0;
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
	//�����У����Ҫ���õھ�λ
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
static void SendString(uint8_t *str1)
{
	uint16_t idata str_i=0;
	
	//485ģʽ����Ҫ����
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

//��������
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
	bit Receive_8_Byte=1;//Ĭ�Ͻ��յ���8���ֽ�����
	if(UART1.ucRec_Flag==true)
	{
		//���˸�������0
		if(UART1.pucRec_Buffer!=0)
		{
			Timer0.usDelay_Timer=0;
			//�ȴ����յ�8������
			while(UART1.ucRec_Cnt<8)
			{
				if(Timer0.usDelay_Timer>=TIMER0_100ms)//100msû������8���ֽڣ�����ѭ��
				{
					Receive_8_Byte=0;
					break;
				}
			}
			if(Receive_8_Byte)
			{
				Modbus.Protocol_Analysis(&UART1);//����Э��
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
		
		Public.Memory_Clr(UART1.pucRec_Buffer,UART1.ucRec_Cnt);
		UART1.ucRec_Flag=false;
		UART1.ucRec_Cnt=0;
	}
}

/*
  *  @name   uart1_isr
  *  @brief  ����1�жϷ�����
  *  @param  None
  *  @retval None
*/
void uart1_isr() interrupt 4 using 1
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
		if(UART1.ucRec_Cnt<Receive_Data_Length)//���л���λ��
		{
			switch(UART1.ParityBit)
			{
				case Odd:
					//���P=1��ǰ��8λ��1��������������ôRB8Ӧ��=0
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