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
	//M1  M0
	//0   0	     准双向口
	//0   1	     推挽输出
	//1   0	     高阻输入
	//1   1	     开漏
	
	//P34推挽输出，P32高阻输入,P33准双向口
	//P30、P31准双向口
	//P36高阻输入，P37推挽输出
	P3M1=0x44;//0100 0100
	P3M0=0x90;//1001 0000
	P35=0;
	
	//P20设置为推挽模式
	P2M1=0x00;
	P2M0=0x01;
	
	P2 &= (~bit0);
	
	//P5
	P5M1=0x00;
	P5M0=0xFF;
	P54=1;//打开外设电源
	P55=0;
	
	//Public.Delayms(2);//等待电平平稳
}

//中断初始化
static void IE_Init()
{
	EX0 = 1; // 打开外部中断0 
	IT0 = 1; // 设置INT0为下降沿触发  
	ET0 = 1;	//打开定时器0中断
	ES  = 1; //使能串口1中断
	EA = 1;  // 打开全局中断
}

//运行指示
static void Run_Indication()
{
	uint8_t i;
	for(i=0;i<10;i++)
	{
		P34=~P34;
		Public.Delayms(100);
	}
}

//系统初始化
static void System_Init()
{
	Public.Delayms(100);
	Hardware_Init.GPIO_Init();
	Hardware_Init.Run_Indication();
	Hardware_Init.IE_Init();
	UART1.UART_Init();
}

