#include "main.h"

#define	WIFI_RST		P03
#define WIFI_EN			P02
#define SmartConfig_EN   0
#define Connect_WiFi	"AT+CWJAP_DEF=\"�εε�\",\"znhxkh309\"\r\n"
#define TCP_Server  "AT+CIPSTART=\"TCP\",\"192.168.101.104\",8888\r\n"  //���Զ�ģ���TCP������

static void Init(void);																				//��ʼ��
static void SmartConfig(void);																//WiFiģ������
static void TCP_Connect_Server(void);													//ͨ��TCP���ӷ�����
static void Send_data(void);																	//ͨ��TCP��������
static void Receive_data(void);																//ͨ��TCP��������
static uint8_t SendAT(uint8_t* ,uint8_t* ,Delay_Time_t);			//����ATָ��
static void Heart(void);


static void Computer_to_ESP12S(void);		//���Դ���->��Ƭ��->ESP12S
static void ESP12S_to_Computer(void);		//ESP12S->��Ƭ��->���Դ���

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
	Computer_to_ESP12S,		//���Դ���->��Ƭ��->ESP12S
	ESP12S_to_Computer		//ESP12S->��Ƭ��->���Դ���
};


/*
  *  @name   Init
  *  @brief  WiFiģ���ʼ��
  *  @param  None
  *  @retval None
*/
static void Init()
{
	//��λ
	WIFI_RST=0;
	Public.Delayms(100);
	WIFI_RST=1;
	//ʹ��
	WIFI_EN=1;
	
	Timer0.usDelay_Timer=0;
	Timer0.usAT_SettingTips=true;	//��ָʾ��
	do
	{
		//��ջ���
		Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
		
		//���Ͳ���ָ��
		UART2.SendString("AT\r\n");
		Public.Delayms(100);
		
		//��ʱ���˳�
		if(Timer0.usDelay_Timer>TIMER0_5s)
		{
			UART1.SendString("WiFi Init timeout!!\n");
			break;
		}
	}while(strstr(UART2.pucRec_Buffer,"OK")==NULL);
	
	
	Public.Delayms(1000);						//�ȴ��ڲ��ȶ�
	Timer0.usAT_SettingTips=false;	//��ָʾ��
}	


/*
  *  @name   SendAT
  *  @brief  ����ATָ��
  *  @param  SendCommand:Ҫ���͵�ָ��
  			     RespondString:�ɹ��󷵻�����ָ��
  	     		 OutTime:���ó�ʱʱ��
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

	UART2.SendString(temp_SendCommand);		//����ָ��
	
	Timer0.usDelay_Timer=0;
	//���û�����յ�\r\n���߶�Ӧ�ĳɹ��ַ�������û�гɹ�
	while((strstr(UART2.pucRec_Buffer,"\r\n")==NULL) ||  (strstr(UART2.pucRec_Buffer,RespondString)==NULL))
	{
		if(Timer0.usDelay_Timer>OutTime)
		{
			//��ʱ
			Public.Error_Handler();
			OK_Flag=0;
			break;
		}
	}

	//�������
	Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
	UART2.ucRec_Cnt=0;
	UART2.ucRec_Flag=false;
	
	return OK_Flag;		//�����Ƿ�ɹ�
}


/*
  *  @name   SmartConfig
  *  @brief  ��������
  *  @param  None
  *  @retval None
*/
static void SmartConfig()
{
	//��⵽����
	if(Key1.Press==true)
	{
		Timer0.usAT_SettingTips=true;	//��ָʾ��
		//���֮ǰ���������˳�͸��ģʽ
		if(esp12s.TCP_Connect_Status==true)
		{
			UART1.SendString("Exit TouChuan----\n");
			UART2.SendString("+");
			UART2.SendString("+");
			UART2.SendString("+");
			//�˳�͸��ģʽ��ȴ�һ�����ATģʽ
			Public.Delayms(1000);
		}
		
		SendAT("AT\r\n","OK",TIMER0_100ms);								//����AT
		SendAT("ATE0\r\n","OK",TIMER0_100ms);							//�رջ��ԣ���ֹ���ź����ж�
		SendAT("AT+CWMODE_DEF=1\r\n","OK",TIMER0_100ms);	//����ΪSTAģʽ
		SendAT("AT+CWAUTOCONN\r\n","OK",TIMER0_100ms);		//�ϵ���Զ�����AP
		#if SmartConfig_EN
			SendAT("AT+CWSTARTSMART\r\n","OK",TIMER0_1s);		//������������
		#else
			SendAT(Connect_WiFi,"OK",TIMER0_1s);						//���ӹ̶�WiFi
		#endif

		//�ȴ�������3���ӳ�ʱ���˳�
		Timer0.usDelay_Timer=0;
		while(Timer0.usDelay_Timer<TIMER0_3m)
		{
			if(strstr(UART2.pucRec_Buffer,"WIFI GOT IP")!=NULL)
			{
				UART1.SendString("WIFI GOT IP");
				//����wifi����ʱ3S�ȴ�����ڲ�����
				Public.Delayms(1000);
				Public.Delayms(1000);
				Public.Delayms(1000);
				break;
			}
		}
		
		Timer0.usAT_SettingTips=false;
		//�������ϵͳ��λ
		Public.softReset();
	}
}	


/*
  *  @name   TCP_Connect_Server
  *  @brief  TCP���ӷ�����
  *  @param  None
  *  @retval None
*/
static void TCP_Connect_Server()
{
	uint8_t xdata OK_Flag=0;	//�ɹ����ر�־
	sint8_t xdata State=0;		//״̬��״̬
	
	UART2.SendString("+++");
	//�˳�͸��ģʽ��ȴ�һ�����ATģʽ
	Public.Delayms(1000);
	
	Timer0.usDelay_Timer=0;
	Timer0.usAT_SettingTips=true;	//��ָʾ��
	while(State!=-1)
	{
		UART1.RS485_Send_Mode();
		printf("State=%bd,OK_Flag=%bd\n",State,OK_Flag);
		UART1.RS485_Receive_Mode();
		
		switch(State)
		{
			case 0:
				OK_Flag=SendAT("AT\r\n","OK",TIMER0_100ms);				//����AT
				if(OK_Flag==1)
					State=1;
				else
					State=-1;
				break;
			case 1:
				SendAT("ATE0\r\n","OK",TIMER0_100ms);			//�رջ���
				if(OK_Flag==1)
					State=2;
				else
					State=-1;
				break;
			case 2:
				SendAT("AT+CWMODE_DEF=1\r\n","OK",TIMER0_100ms);	//����ΪSTAģʽ�����浽flash
				if(OK_Flag==1)
					State=3;
				else
					State=-1;
				break;
			case 3:
				OK_Flag=SendAT("AT+CIFSR\r\n","192.168",TIMER0_5s);		//��ѯ��û�����ӵ�WiFi
				if(OK_Flag==1)
					State=4;
				else
					State=-1;
				break;
			case 4:
				//���������WiFi���Կ�ʼ���ӷ�����
				OK_Flag=SendAT(TCP_Server,"CONNECT",TIMER0_5s);		//���ӷ�����
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
				OK_Flag=SendAT("AT+CIPMODE=1\r\n","OK",TIMER0_100ms);		//����Ϊ͸��ģʽ
				if(OK_Flag==1)
					State=6;
				else
					State=-1;
				break;
			case 6:
				OK_Flag=SendAT("AT+CIPSEND\r\n","OK",TIMER0_100ms);		//��ʼ����
				if(OK_Flag==1)
					State=-1;
				break;
		}
	}
	//��ָʾ��
	Timer0.usAT_SettingTips=false;	
	//����ʱ������
	esp12s.TCP_Reconnect_Timer=0;
}	

//ͨ��TCP��������
static void Send_data()
{
	
}	

//ͨ��TCP��������
static void Receive_data()
{
	
}	

/*
  *  @name   Heart
  *  @brief  ��������
  *  @param  None
  *  @retval None
*/
static void Heart()
{
	static uint8_t times=0;//����Ϊ��̬�������������ڼ���Ч

	//3����һ��
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

		//�������4�ζ��ղ��������ź��ж�Ϊ�Ͽ�����
		if(times>=4)
		{
			esp12s.TCP_Connect_Status=false;
			times=0;
		}
		
		//�������
		Public.Memory_Clr(UART2.pucRec_Buffer,4);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
		//��ʱ������
		Timer0.usDelay_Timer=0;			
	}
}



/*
  *  @name   Computer_to_ESP12S
  *  @brief  ���Դ��ڷ������ݣ�ģ�����
  *  @param  None
  *  @retval None
*/
static void Computer_to_ESP12S()
{
	uint8_t xdata TimeOutFlag=1;

	//��������ݹ���
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
		
		//�����ATָ��ͷ���
		if(TimeOutFlag==1 && (*(UART1.pucRec_Buffer)=='A') && (*(UART1.pucRec_Buffer+1)=='T'))
		{
			//����ATָ���WiFi
			UART1.RS485_Send_Mode();
			printf("Send data=%s",UART1.pucRec_Buffer);
			UART1.RS485_Receive_Mode();

			UART2.SendString(UART1.pucRec_Buffer);
		}
		else
		{
			//�������ATָ��ش����ַ���
			UART1.RS485_Send_Mode();
			printf("Not AT Command!!=%s\n",UART1.pucRec_Buffer);
			UART1.RS485_Receive_Mode();

			UART2.SendString(UART1.pucRec_Buffer);
		}

		//����
		Public.Memory_Clr(UART1.pucRec_Buffer,UART1.ucRec_Cnt);
		UART1.ucRec_Cnt=0;
		UART1.ucRec_Flag=false;
	}
}


/*
  *  @name   ESP12S_to_Computer
  *  @brief  ģ�鷢���ݣ�������
  *  @param  None
  *  @retval None
*/
static void ESP12S_to_Computer()
{
	if(UART2.ucRec_Flag==true)
	{
		//�ȴ��������
		Timer0.usDelay_Timer=0;
		while(strstr(UART2.pucRec_Buffer,"\r\n")==NULL)
		{
			if(Timer0.usDelay_Timer>TIMER0_100ms)
			{
				UART1.SendString("Receive TimeOut!\n");
				break;
			}
		}

		//����յ�����ֵ������û��\r\n�����ͨ�����ַ���Ϣ
		UART1.SendString(UART2.pucRec_Buffer);

		//�建��
		Public.Memory_Clr(UART2.pucRec_Buffer,UART2.ucRec_Cnt);
		UART2.ucRec_Cnt=0;
		UART2.ucRec_Flag=false;
	}
}
