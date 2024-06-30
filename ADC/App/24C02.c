#include "main.h"

#define AT24C04_Addr 0xA0							//从机地址
#define AT24C04_SaveDataAddr 0x00			//数据存放地址

static uint8_t Sendstr(uint8_t *,uint8_t );			//发送多个字节
static uint8_t Receivestr(uint8_t *,uint8_t );		//接收多个字节

AT24C02_t AT24C02=
{
	Sendstr,
	Receivestr
};


//写多个字节数据
static uint8_t Sendstr(uint8_t *array,uint8_t LEN)
{
	uint8_t i=0;
	
	IIC.Start_I2c();					//开启IIC
	
	IIC.SendByte(AT24C04_Addr);				//发送从机地址
	if(IIC.slv_Ack==0)return 0;			//如果从机没有反应则返回0

	IIC.SendByte(AT24C04_SaveDataAddr);			//发送需要操作的数据地址
	if(IIC.slv_Ack==0)return 0;
	
	for(i=0;i<LEN;i++)
	{
		IIC.SendByte(*array);			//发送数据
		if(IIC.slv_Ack==0)return 0;
		array++;
	}

	IIC.Stop_I2c();						//关闭IIC

	Public.Delayms(1);				//等待1ms内部处理信号完成
	
	return 1;
	
}


//读取多个字节数据
static uint8_t Receivestr(uint8_t *array,uint8_t LEN)
{
	uint8_t i=0;
	
	IIC.Start_I2c();											//开启IIC
	
	IIC.SendByte(AT24C04_Addr);						//发送从机地址
	if(IIC.slv_Ack==0)return 0;						//如果从机没有反应则返回0
	
	IIC.SendByte(AT24C04_SaveDataAddr);		//发送需要操作的数据地址
	if(IIC.slv_Ack==0)return 0;
	
	IIC.Start_I2c();											//重新启动一次
	
	IIC.SendByte(AT24C04_Addr+1);					//告诉从机需要读取数据
	if(IIC.slv_Ack==0)return 0;						//如果从机没有反应则返回0
	
	for(i=0;i<LEN-1;i++)
	{
		*array=IIC.RcvByte();
		IIC.Ack_I2c();
		array++;
	}

	//最后一个字节需要发送非应答
	*array=IIC.RcvByte();
	IIC.NoAck_I2c();
	
	IIC.Stop_I2c();						//关闭IIC

	return 1;
	
}