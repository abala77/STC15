/*************************此部分为I2C总线的驱动程序*************************************/
#include "main.h"

#define  _Nop()  _nop_()   //定义空指令

#define SCL		P23	 	//I2C  时钟
#define SDA   P22		//I2C  数据


static void I2C_Init(void);				//IIC初始化
static void Start_I2c(void);			//启动IIC
static void Stop_I2c(void);				//停止IIC
static void SendByte(uint8_t);		//发送一个字节数据
static uint8_t RcvByte(void);			//接收一个字节数据
static void Ack_I2c(void);				//主机应答	
static void NoAck_I2c(void);			//主机不应答

static void delay(void);							//延时5us左右
			
//创建对象
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
  *  @brief  延时一个有效周期
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
  *  @brief  IIC初始化，释放SDA和SCL
  *  @param  None
  *  @retval None
*/
static void I2C_Init()
{
    SCL = 1; 	// 设置SCL为高电平
    SDA = 1; 	// 设置SDA为高电平
}

/*
  *  @name   Start_I2c
  *  @brief  启动总线
  *  @param  None
  *  @retval None
*/
static void Start_I2c()
{
  SDA = 1; // 设置SDA为高电平
	SCL = 1; // 设置SCL为高电平
	delay(); // 延时
	SDA = 0; // 产生起始信号
	delay(); // 延时
	SCL = 0; // 设置SCL为低电平
}


/*
  *  @name   Stop_I2c
  *  @brief  结束总线
  *  @param  None
  *  @retval None
*/
static void Stop_I2c()
{
	SDA = 0; // 设置SDA为低电平
	SCL = 1; // 设置SCL为高电平
	delay(); // 延时
	SDA = 1; // 产生停止信号
	delay(); // 延时
}


/*
  *  @name   SendByte
	*  @brief  发送一个字节数据。（发完后等待应答,并对此状态位进行操作.(不应答或非应答都使ack=0)）   
	*  @param  Data:需要发送的数据，可以是地址,也可以是数据
  *  @retval None
*/
static void SendByte(uint8_t Data)
{
	uint8_t BitCnt;
 
	for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
	{
		if((Data<<BitCnt)&0x80)
			SDA=1;   /*判断发送位*/
		else  
			SDA=0;                
		delay();
		SCL=1;               /*置时钟线为高，通知被控器开始接收数据位*/
		delay();      
		SCL=0; 
	}
	
	SDA=1;                /*8位发送完后释放数据线，准备接收应答位*/   
	delay();
	SCL=1;
	delay();
	if(SDA==1)IIC.slv_Ack=0;     
		 else IIC.slv_Ack=1;        /*判断是否接收到应答信号*/
	SCL=0;
	delay();
}
   

/*
  *  @name   RcvByte
	*  @brief  接收一个字节数据。（发完后请用应答函数应答从机）
	*  @param  None
	*  @retval 8位无符号数据
*/
static uint8_t RcvByte()
{
  uint8_t  retc;
  uint8_t  BitCnt;
  
  retc=0; 
  SDA=1;                       /*置数据线为输入方式*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
	{           
		SCL=0;                  /*置时钟线为低，准备接收数据位*/
		delay();
		SCL=1;                  /*置时钟线为高使数据线上数据有效*/
		delay();
		retc=retc<<1;
		if(SDA==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
	}
	SCL=0;    
	delay();
	
	return(retc);
}


/*
  *  @name   Ack_I2c
	*  @brief  主机应答信号
	*  @param  None
	*  @retval None
*/
static void Ack_I2c()
{             
  SDA=0;										/*应答信号 */
  delay();     
  SCL=1;
  delay();
  SCL=0;                     /*清时钟线，钳住I2C总线以便继续接收*/
  delay();  
}

/*
  *  @name   NoAck_I2c
	*  @brief  主机非应答信号
	*  @param  None
	*  @retval None
*/
static void NoAck_I2c()
{
  SDA=1;              /*非应答信号 */
  delay();    
  SCL=1;
  delay();
  SCL=0;                     /*清时钟线，钳住I2C总线以便继续接收*/
  delay();
}