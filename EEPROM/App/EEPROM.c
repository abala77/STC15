#include "main.h"

#define CMD_IDLE 				0	 //空闲模式
#define CMD_READ 				1  //IAP字节读命令
#define CMD_PROGRAM 		2  //IAP字节编程命令
#define CMD_ERASE 			3  //IAP扇区擦除命令
#define ENABLE_IAP	 0x83  //使能IAP，设置等待时间

static uint8_t ReadByte(uint16_t);					//读一个字节
static void WriteByte(uint16_t,uint8_t);		//写一个字节
static void EraseSector(uint16_t);					//擦除扇区

static void IapIdle(void);									//关闭IAP

//结构体变量
EEPROM_t EEPROM=
{
	false,
	EEPROM_Cnt,
	
	ReadByte,
	WriteByte,
	EraseSector
};

//关闭IAP
static void IapIdle()
{
	IAP_CONTR= 0;			//关闭IAP功能
	IAP_CMD= 0;				//清除命令寄存器
	IAP_TRIG= 0;			//清除触发寄存器
	IAP_ADDRH= 0xFF;	//将地址设置到非IAP区域
	IAP_ADDRL= 0xFF;
}

//读一个字节
static uint8_t ReadByte(uint16_t addr)
{
	uint8_t dat=0;
	
	IAP_CONTR	= ENABLE_IAP;	 //使能IAP
	IAP_CMD		= CMD_READ;		 //设置IAP命令
	IAP_ADDRL	= addr;			   //设置IAP低地址
  IAP_ADDRH	= addr>>8;	   //设置IAP高地址
	IAP_TRIG	= 0x5a;			   //触发命令
  IAP_TRIG	= 0xa5;
	_nop_();					       //等待操作完成
	dat = IAP_DATA;				   //读数据
	IapIdle();		           //关闭IAP功能

	return dat;
}

//写一个字节
static void WriteByte(uint16_t addr,uint8_t dat)
{
	IAP_CONTR	= ENABLE_IAP;	 //使能IAP
	IAP_CMD		= CMD_PROGRAM; //设置IAP命令
	IAP_ADDRL	= addr;			   //设置IAP低地址
  IAP_ADDRH	= addr>>8;	   //设置IAP高地址
	IAP_DATA=dat;				  	 //写数据
	IAP_TRIG	= 0x5a;			   //触发命令
  IAP_TRIG	= 0xa5;
	_nop_();					       //等待操作完成
	IapIdle();		           //关闭IAP功能
}

//擦除扇区
static void EraseSector(uint16_t addr)
{
	IAP_CONTR	= ENABLE_IAP;	 //使能IAP
	IAP_CMD		= CMD_ERASE;   //设置IAP命令
	IAP_ADDRL	= addr;			   //设置IAP低地址
  IAP_ADDRH	= addr>>8;	   //设置IAP高地址
	IAP_TRIG	= 0x5a;			   //触发命令
  IAP_TRIG	= 0xa5;
	_nop_();					       //等待操作完成
	IapIdle();		           //关闭IAP功能
}