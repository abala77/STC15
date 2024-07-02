/*************************�˲���ΪI2C���ߵ���������*************************************/
#include "main.h"

#define  _Nop()  _nop_()   //�����ָ��

#define SCL		P23	 	//I2C  ʱ��
#define SDA   P22		//I2C  ����


static void I2C_Init(void);				//IIC��ʼ��
static void Start_I2c(void);			//����IIC
static void Stop_I2c(void);				//ֹͣIIC
static void SendByte(uint8_t);		//����һ���ֽ�����
static uint8_t RcvByte(void);			//����һ���ֽ�����
static void Ack_I2c(void);				//����Ӧ��	
static void NoAck_I2c(void);			//������Ӧ��

static void delay(void);							//��ʱ5us����
			
//��������
IIC_t IIC=
{
	0,
	
	I2C_Init,
	Start_I2c,
	Stop_I2c,
	SendByte,
	RcvByte,
	Ack_I2c,
	NoAck_I2c
};

/*
  *  @name   Delay
  *  @brief  ��ʱһ����Ч����
  *  @param  None
  *  @retval None
*/
static void delay()
{
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	_Nop();
}

/*
  *  @name   I2C_Init
  *  @brief  IIC��ʼ�����ͷ�SDA��SCL
  *  @param  None
  *  @retval None
*/
static void I2C_Init()
{
    SCL = 1; 	// ����SCLΪ�ߵ�ƽ
    SDA = 1; 	// ����SDAΪ�ߵ�ƽ
}

/*
  *  @name   Start_I2c
  *  @brief  ��������
  *  @param  None
  *  @retval None
*/
static void Start_I2c()
{
  SDA = 1; // ����SDAΪ�ߵ�ƽ
	SCL = 1; // ����SCLΪ�ߵ�ƽ
	delay(); // ��ʱ
	SDA = 0; // ������ʼ�ź�
	delay(); // ��ʱ
	SCL = 0; // ����SCLΪ�͵�ƽ
}


/*
  *  @name   Stop_I2c
  *  @brief  ��������
  *  @param  None
  *  @retval None
*/
static void Stop_I2c()
{
	SDA = 0; // ����SDAΪ�͵�ƽ
	SCL = 1; // ����SCLΪ�ߵ�ƽ
	delay(); // ��ʱ
	SDA = 1; // ����ֹͣ�ź�
	delay(); // ��ʱ
}


/*
  *  @name   SendByte
	*  @brief  ����һ���ֽ����ݡ��������ȴ�Ӧ��,���Դ�״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)��   
	*  @param  Data:��Ҫ���͵����ݣ������ǵ�ַ,Ҳ����������
  *  @retval None
*/
static void SendByte(uint8_t Data)
{
	uint8_t BitCnt;
 
	for(BitCnt=0;BitCnt<8;BitCnt++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
	{
		if((Data<<BitCnt)&0x80)
			SDA=1;   /*�жϷ���λ*/
		else  
			SDA=0;                
		delay();
		SCL=1;               /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
		delay();      
		SCL=0; 
	}
	
	SDA=1;                /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/   
	delay();
	SCL=1;
	delay();
	if(SDA==1)IIC.slv_Ack=0;     
		 else IIC.slv_Ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/
	SCL=0;
	delay();
}
   

/*
  *  @name   RcvByte
	*  @brief  ����һ���ֽ����ݡ������������Ӧ����Ӧ��ӻ���
	*  @param  None
	*  @retval 8λ�޷�������
*/
static uint8_t RcvByte()
{
  uint8_t  retc;
  uint8_t  BitCnt;
  
  retc=0; 
  SDA=1;                       /*��������Ϊ���뷽ʽ*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
	{           
		SCL=0;                  /*��ʱ����Ϊ�ͣ�׼����������λ*/
		delay();
		SCL=1;                  /*��ʱ����Ϊ��ʹ��������������Ч*/
		delay();
		retc=retc<<1;
		if(SDA==1)retc=retc+1;  /*������λ,���յ�����λ����retc�� */
	}
	SCL=0;    
	delay();
	
	return(retc);
}


/*
  *  @name   Ack_I2c
	*  @brief  ����Ӧ���ź�
	*  @param  None
	*  @retval None
*/
static void Ack_I2c()
{             
  SDA=0;										/*Ӧ���ź� */
  delay();     
  SCL=1;
  delay();
  SCL=0;                     /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  delay();  
}

/*
  *  @name   NoAck_I2c
	*  @brief  ������Ӧ���ź�
	*  @param  None
	*  @retval None
*/
static void NoAck_I2c()
{
  SDA=1;              /*��Ӧ���ź� */
  delay();    
  SCL=1;
  delay();
  SCL=0;                     /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  delay();
}