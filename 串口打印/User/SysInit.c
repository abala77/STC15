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
	//P34���������P32��������,P33׼˫���
	P3M1=0x04;
	P3M0=0x10;
}

//�жϳ�ʼ��
static void IE_Init()
{
	IT0 = 1; // ����INT0Ϊ�½��ش���  
	EX0 = 1; // �ر��ⲿ�ж�0  
	ES  = 1; //ʹ�ܴ���1�ж�
	EA = 1;  // ��ȫ���ж�
}

//����ָʾ
static void Run_Indication()
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		P34=~P34;
		Public.Delayms(50);
	}
}

//ϵͳ��ʼ��
static void System_Init()
{
	Public.Delayms(50);
	Hardware_Init.Run_Indication();
	Hardware_Init.GPIO_Init();
	Hardware_Init.IE_Init();
	UART1.UART_Init();
}

