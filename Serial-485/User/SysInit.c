#include "main.h"


static void GPIO_Init();
static void IE_Init();
static void Run_Indication();
static void System_Init();

//��������
Hardware_Init_t Hardware_Init=
{
	GPIO_Init,
	IE_Init,
	Run_Indication,
	System_Init
};


//���ų�ʼ��
static void GPIO_Init()
{
	//M1  M0
	//0   0	     ׼˫���
	//0   1	     �������
	//1   0	     ��������
	//1   1	     ��©
	
	//P34���������P32��������,P33׼˫���
	//P30��P31׼˫���
	//P36�������룬P37�������
	P3M1=0x44;//0100 0100
	P3M0=0x90;//1001 0000
	P35=0;
	
	//P20����Ϊ����ģʽ
	P2M1=0x00;
	P2M0=0x01;
	
	P2 &= (~bit0);
	
	//P5
	P5M1=0x00;
	P5M0=0xFF;
	P54=1;//�������Դ
	P55=0;
	
	//Public.Delayms(2);//�ȴ���ƽƽ��
}

//�жϳ�ʼ��
static void IE_Init()
{
	EX0 = 1; // ���ⲿ�ж�0 
	IT0 = 1; // ����INT0Ϊ�½��ش���  
	ET0 = 1;	//�򿪶�ʱ��0�ж�
	ES  = 1; //ʹ�ܴ���1�ж�
	EA = 1;  // ��ȫ���ж�
}

//����ָʾ
static void Run_Indication()
{
	uint8_t i;
	for(i=0;i<10;i++)
	{
		P34=~P34;
		Public.Delayms(100);
	}
}

//ϵͳ��ʼ��
static void System_Init()
{
	Public.Delayms(100);
	Hardware_Init.GPIO_Init();
	Hardware_Init.Run_Indication();
	Hardware_Init.IE_Init();
	UART1.UART_Init();
}

