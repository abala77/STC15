#include "main.h"

#define	WIFI_RST		P03
#define WIFI_EN			P02
#define SmartConfig_EN   0
#define Connect_WiFi	"AT+CWJAP_DEF=\"滴滴滴\",\"znhxkh309\"\r\n"
#define TCP_Server  "AT+CIPSTART=\"TCP\",\"192.168.101.104\",8888\r\n"  //电脑端模拟的TCP服务器

static void Init(void);																				//初始化
static void SmartConfig(void);																//WiFi模块配网
static void TCP_Connect_Server(void);													//通过TCP连接服务器
static void Send_data(void);																	//通过TCP发送数据
static void Receive_data(void);																//通过TCP接收数据
static uint8_t SendAT(uint8_t* ,uint8_t* ,Delay_Time_t);			//发送AT指令
static void Heart(void);


static void Computer_to_ESP12S(void);		//电脑串口->单片机->ESP12S
static void ESP12S_to_Computer(void);		//ESP12S->单片机->电脑串口

WiFi_t xdata esp12s=
{
	0,
	0,
	
	Init,
	SmartConfig,
	TCP_Connect_Server,
	Send_data,
	Receive_data,
	Heart
};

WiFi_Debug_t WiFi_Debug=
{
	Computer_to_ESP12S,		//电脑串口->单片机->ESP12S
	ESP12S_to_Computer		//ESP12S->单片机->电脑串口
};


/*
  *  @name   Init
  *  @brief  WiFi模块初始化
  *  @param  None
  *  @retval None
*/
static void Init()
{
	//复位
	WIFI_RST=0;
	Public.Delayms(100);
	WIFI_RST=1;
	//使能
	WIFI_EN=1;
	
	Timer0.usDelay_Timer=0;
	Timer0.usAT_SettingTips=true;	//开指示灯
	do
	{
		//清空缓存
		Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
		
		//发送测试指令
		UART2.SendString("AT\r\n");
		Public.Delayms(100);
		
		//超时则退出
		if(Timer0.usDelay_Timer>TIMER0_5s)
		{
			UART1.SendString("WiFi Init timeout!!\n");
			break;
		}
	}while(strstr(UART2.pucRec_Buffer,"OK")==NULL);
	
	
	Public.Delayms(1000);						//等待内部稳定
	Timer0.usAT_SettingTips=false;	//关指示灯
}	


/*
  *  @name   SendAT
  *  @brief  发送AT指令
  *  @param  SendCommand:要发送的指令
  			     RespondString:成功后返回来的指令
  	     		 OutTime:设置超时时间
  *  @retval None
*/
static uint8_t SendAT(uint8_t* SendCommand,uint8_t* RespondString,Delay_Time_t xdata OutTime)
{
	uint8_t xdata OK_Flag=1;
	uint8_t* const xdata temp_SendCommand=SendCommand;
	uint8_t* const xdata temp_RespondString=RespondString;
	
	Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
	UART2.ucRec_Cnt=0;
	UART2.ucRec_Flag=false;

	UART2.SendString(temp_SendCommand);		//发送指令
	
	Timer0.usDelay_Timer=0;
	//如果没有有收到\r\n或者对应的成功字符串就是没有成功
	while((strstr(UART2.pucRec_Buffer,"\r\n")==NULL) ||  (strstr(UART2.pucRec_Buffer,RespondString)==NULL))
	{
		if(Timer0.usDelay_Timer>OutTime)
		{
			//超时
			Public.Error_Handler();
			OK_Flag=0;
			break;
		}
	}

	//清除缓冲
	Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
	UART2.ucRec_Cnt=0;
	UART2.ucRec_Flag=false;
	
	return OK_Flag;		//返回是否成功
}


/*
  *  @name   SmartConfig
  *  @brief  智能配网
  *  @param  None
  *  @retval None
*/
static void SmartConfig()
{
	//检测到长按
	if(Key1.Press==true)
	{
		Timer0.usAT_SettingTips=true;	//开指示灯
		//如果之前有链接则退出透传模式
		if(esp12s.TCP_Connect_Status==true)
		{
			UART1.SendString("Exit TouChuan----\n");
			UART2.SendString("+");
			UART2.SendString("+");
			UART2.SendString("+");
			//退出透传模式后等待一秒进入AT模式
			Public.Delayms(1000);
		}
		
		SendAT("AT\r\n","OK",TIMER0_100ms);								//测试AT
		SendAT("ATE0\r\n","OK",TIMER0_100ms);							//关闭回显，防止干扰后续判断
		SendAT("AT+CWMODE_DEF=1\r\n","OK",TIMER0_100ms);	//设置为STA模式
		SendAT("AT+CWAUTOCONN\r\n","OK",TIMER0_100ms);		//上电后自动连接AP
		#if SmartConfig_EN
			SendAT("AT+CWSTARTSMART\r\n","OK",TIMER0_1s);		//开启智能配网
		#else
			SendAT(Connect_WiFi,"OK",TIMER0_1s);						//连接固定WiFi
		#endif

		//等待配网，3分钟超时则退出
		Timer0.usDelay_Timer=0;
		while(Timer0.usDelay_Timer<TIMER0_3m)
		{
			if(strstr(UART2.pucRec_Buffer,"WIFI GOT IP")!=NULL)
			{
				UART1.SendString("WIFI GOT IP");
				//连接wifi后延时3S等待完成内部设置
				Public.Delayms(1000);
				Public.Delayms(1000);
				Public.Delayms(1000);
				break;
			}
		}
		
		Timer0.usAT_SettingTips=false;
		//配置完成系统复位
		Public.softReset();
	}
}	


/*
  *  @name   TCP_Connect_Server
  *  @brief  TCP连接服务器
  *  @param  None
  *  @retval None
*/
static void TCP_Connect_Server()
{
	uint8_t xdata OK_Flag=0;	//成功返回标志
	sint8_t xdata State=0;		//状态机状态
	
	UART2.SendString("+++");
	//退出透传模式后等待一秒进入AT模式
	Public.Delayms(1000);
	
	Timer0.usDelay_Timer=0;
	Timer0.usAT_SettingTips=true;	//开指示灯
	while(State!=-1)
	{
		UART1.RS485_Send_Mode();
		printf("State=%bd,OK_Flag=%bd\n",State,OK_Flag);
		UART1.RS485_Receive_Mode();
		
		switch(State)
		{
			case 0:
				OK_Flag=SendAT("AT\r\n","OK",TIMER0_100ms);				//测试AT
				if(OK_Flag==1)
					State=1;
				else
					State=-1;
				break;
			case 1:
				SendAT("ATE0\r\n","OK",TIMER0_100ms);			//关闭回显
				if(OK_Flag==1)
					State=2;
				else
					State=-1;
				break;
			case 2:
				SendAT("AT+CWMODE_DEF=1\r\n","OK",TIMER0_100ms);	//设置为STA模式，保存到flash
				if(OK_Flag==1)
					State=3;
				else
					State=-1;
				break;
			case 3:
				OK_Flag=SendAT("AT+CIFSR\r\n","192.168",TIMER0_5s);		//查询有没有连接到WiFi
				if(OK_Flag==1)
					State=4;
				else
					State=-1;
				break;
			case 4:
				//如果连接了WiFi可以开始连接服务器
				OK_Flag=SendAT(TCP_Server,"CONNECT",TIMER0_5s);		//连接服务器
				if(OK_Flag==1)
				{
					esp12s.TCP_Connect_Status=true;
					State=5;
				}
				else
				{
					esp12s.TCP_Connect_Status=false;
					State=-1;
				}
				break;
			case 5:
				OK_Flag=SendAT("AT+CIPMODE=1\r\n","OK",TIMER0_100ms);		//设置为透传模式
				if(OK_Flag==1)
					State=6;
				else
					State=-1;
				break;
			case 6:
				OK_Flag=SendAT("AT+CIPSEND\r\n","OK",TIMER0_100ms);		//开始传输
				if(OK_Flag==1)
					State=-1;
				break;
		}
	}
	//关指示灯
	Timer0.usAT_SettingTips=false;	
	//重连时间清零
	esp12s.TCP_Reconnect_Timer=0;
}	

//通过TCP发送数据
static void Send_data()
{
	
}	

//通过TCP接收数据
static void Receive_data()
{
	
}	

/*
  *  @name   Heart
  *  @brief  心跳测试
  *  @param  None
  *  @retval None
*/
static void Heart()
{
	static uint8_t times=0;//声明为静态整个程序运行期间有效

	//3秒跳一次
	if(esp12s.TCP_Connect_Status==true && Timer0.usDelay_Timer>TIMER0_3s)
	{
		UART2.SendString("Running!\n");
		while(true)
		{
			if(UART2.ucRec_Flag==true && strstr(UART2.pucRec_Buffer,"OK")!=NULL)
			{
				times=0;
				break;
			}
			
			if(Timer0.usDelay_Timer>TIMER0_4s)
			{
				times++;
				break;
			}
		}

		//如果连续4次都收不到心跳信号判定为断开连接
		if(times>=4)
		{
			esp12s.TCP_Connect_Status=false;
			times=0;
		}
		
		//清除缓存
		Public.Memory_Clr(UART2.pucRec_Buffer,4);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
		//定时器清零
		Timer0.usDelay_Timer=0;			
	}
}



/*
  *  @name   Computer_to_ESP12S
  *  @brief  电脑串口发送数据，模块接收
  *  @param  None
  *  @retval None
*/
static void Computer_to_ESP12S()
{
	uint8_t xdata TimeOutFlag=1;

	//如果有数据过来
	if(UART1.ucRec_Flag==true)
	{
		Timer0.usDelay_Timer=0;
		while(strstr(UART1.pucRec_Buffer,"\r\n")==NULL)
		{
			if(Timer0.usDelay_Timer>TIMER0_100ms)
			{
				TimeOutFlag=0;
				UART1.SendString("TimeOut!\n");
				break;
			}
		}
		
		//如果是AT指令就发送
		if(TimeOutFlag==1 && (*(UART1.pucRec_Buffer)=='A') && (*(UART1.pucRec_Buffer+1)=='T'))
		{
			//发送AT指令给WiFi
			UART1.RS485_Send_Mode();
			printf("Send data=%s",UART1.pucRec_Buffer);
			UART1.RS485_Receive_Mode();

			UART2.SendString(UART1.pucRec_Buffer);
		}
		else
		{
			//如果不是AT指令返回错误字符串
			UART1.RS485_Send_Mode();
			printf("Not AT Command!!=%s\n",UART1.pucRec_Buffer);
			UART1.RS485_Receive_Mode();

			UART2.SendString(UART1.pucRec_Buffer);
		}

		//重置
		Public.Memory_Clr(UART1.pucRec_Buffer,UART1.ucRec_Cnt);
		UART1.ucRec_Cnt=0;
		UART1.ucRec_Flag=false;
	}
}


/*
  *  @name   ESP12S_to_Computer
  *  @brief  模块发数据，电脑收
  *  @param  None
  *  @retval None
*/
static void ESP12S_to_Computer()
{
	if(UART2.ucRec_Flag==true)
	{
		//等待接收完成
		Timer0.usDelay_Timer=0;
		while(strstr(UART2.pucRec_Buffer,"\r\n")==NULL)
		{
			if(Timer0.usDelay_Timer>TIMER0_100ms)
			{
				UART1.SendString("Receive TimeOut!\n");
				break;
			}
		}

		//如果收到返回值无论有没有\r\n都会给通信助手发消息
		UART1.SendString(UART2.pucRec_Buffer);

		//清缓存
		Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
	}
}
