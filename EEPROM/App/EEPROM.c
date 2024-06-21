#include "main.h"

#define CMD_IDLE 				0	 //����ģʽ
#define CMD_READ 				1  //IAP�ֽڶ�����
#define CMD_PROGRAM 		2  //IAP�ֽڱ������
#define CMD_ERASE 			3  //IAP������������
#define ENABLE_IAP	 0x83  //ʹ��IAP�����õȴ�ʱ��

static uint8_t ReadByte(uint16_t);					//��һ���ֽ�
static void WriteByte(uint16_t,uint8_t);		//дһ���ֽ�
static void EraseSector(uint16_t);					//��������

static void IapIdle(void);									//�ر�IAP

//�ṹ�����
EEPROM_t EEPROM=
{
	false,
	EEPROM_Cnt,
	
	ReadByte,
	WriteByte,
	EraseSector
};

//�ر�IAP
static void IapIdle()
{
	IAP_CONTR= 0;			//�ر�IAP����
	IAP_CMD= 0;				//�������Ĵ���
	IAP_TRIG= 0;			//��������Ĵ���
	IAP_ADDRH= 0xFF;	//����ַ���õ���IAP����
	IAP_ADDRL= 0xFF;
}

//��һ���ֽ�
static uint8_t ReadByte(uint16_t addr)
{
	uint8_t dat=0;
	
	IAP_CONTR	= ENABLE_IAP;	 //ʹ��IAP
	IAP_CMD		= CMD_READ;		 //����IAP����
	IAP_ADDRL	= addr;			   //����IAP�͵�ַ
  IAP_ADDRH	= addr>>8;	   //����IAP�ߵ�ַ
	IAP_TRIG	= 0x5a;			   //��������
  IAP_TRIG	= 0xa5;
	_nop_();					       //�ȴ��������
	dat = IAP_DATA;				   //������
	IapIdle();		           //�ر�IAP����

	return dat;
}

//дһ���ֽ�
static void WriteByte(uint16_t addr,uint8_t dat)
{
	IAP_CONTR	= ENABLE_IAP;	 //ʹ��IAP
	IAP_CMD		= CMD_PROGRAM; //����IAP����
	IAP_ADDRL	= addr;			   //����IAP�͵�ַ
  IAP_ADDRH	= addr>>8;	   //����IAP�ߵ�ַ
	IAP_DATA=dat;				  	 //д����
	IAP_TRIG	= 0x5a;			   //��������
  IAP_TRIG	= 0xa5;
	_nop_();					       //�ȴ��������
	IapIdle();		           //�ر�IAP����
}

//��������
static void EraseSector(uint16_t addr)
{
	IAP_CONTR	= ENABLE_IAP;	 //ʹ��IAP
	IAP_CMD		= CMD_ERASE;   //����IAP����
	IAP_ADDRL	= addr;			   //����IAP�͵�ַ
  IAP_ADDRH	= addr>>8;	   //����IAP�ߵ�ַ
	IAP_TRIG	= 0x5a;			   //��������
  IAP_TRIG	= 0xa5;
	_nop_();					       //�ȴ��������
	IapIdle();		           //�ر�IAP����
}