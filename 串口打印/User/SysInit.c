#include "main.h"


static void GPIO_Init();
static void IE_Init();
static void Run_Indication();
static void System_Init();

//创建对象
Hardware_Init_t Hardware_Init=
{
	GPIO_Init,
	IE_Init,
	Run_Indication,
	System_Init
};


//引脚初始化
static void GPIO_Init()
{
	//P34推挽输出，P32高阻输入,P33准双向口
	P3M1=0x04;
	P3M0=0x10;
}

//中断初始化
static void IE_Init()
{
	IT0 = 1; // 设置INT0为下降沿触发  
	EX0 = 1; // 关闭外部中断0  
	ES  = 1; //使能串口1中断
	EA = 1;  // 打开全局中断
}

//运行指示
static void Run_Indication()
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		P34=~P34;
		Public.Delayms(50);
	}
}

//系统初始化
static void System_Init()
{
	Public.Delayms(50);
	Hardware_Init.Run_Indication();
	Hardware_Init.GPIO_Init();
	Hardware_Init.IE_Init();
	UART1.UART_Init();
}

