#include "main.h"

#define AT24C04_Addr 0xA0							//�ӻ���ַ
#define AT24C04_SaveDataAddr 0x00			//���ݴ�ŵ�ַ

static uint8_t Sendstr(uint8_t *,uint8_t );			//���Ͷ���ֽ�
static uint8_t Receivestr(uint8_t *,uint8_t );		//���ն���ֽ�

AT24C02_t AT24C02=
{
	Sendstr,
	Receivestr
};


//д����ֽ�����
static uint8_t Sendstr(uint8_t *array,uint8_t LEN)
{
	uint8_t i=0;
	
	IIC.Start_I2c();					//����IIC
	
	IIC.SendByte(AT24C04_Addr);				//���ʹӻ���ַ
	if(IIC.slv_Ack==0)return 0;			//����ӻ�û�з�Ӧ�򷵻�0

	IIC.SendByte(AT24C04_SaveDataAddr);			//������Ҫ���������ݵ�ַ
	if(IIC.slv_Ack==0)return 0;
	
	for(i=0;i<LEN;i++)
	{
		IIC.SendByte(*array);			//��������
		if(IIC.slv_Ack==0)return 0;
		array++;
	}

	IIC.Stop_I2c();						//�ر�IIC

	Public.Delayms(1);				//�ȴ�1ms�ڲ������ź����
	
	return 1;
	
}


//��ȡ����ֽ�����
static uint8_t Receivestr(uint8_t *array,uint8_t LEN)
{
	uint8_t i=0;
	
	IIC.Start_I2c();											//����IIC
	
	IIC.SendByte(AT24C04_Addr);						//���ʹӻ���ַ
	if(IIC.slv_Ack==0)return 0;						//����ӻ�û�з�Ӧ�򷵻�0
	
	IIC.SendByte(AT24C04_SaveDataAddr);		//������Ҫ���������ݵ�ַ
	if(IIC.slv_Ack==0)return 0;
	
	IIC.Start_I2c();											//��������һ��
	
	IIC.SendByte(AT24C04_Addr+1);					//���ߴӻ���Ҫ��ȡ����
	if(IIC.slv_Ack==0)return 0;						//����ӻ�û�з�Ӧ�򷵻�0
	
	for(i=0;i<LEN-1;i++)
	{
		*array=IIC.RcvByte();
		IIC.Ack_I2c();
		array++;
	}

	//���һ���ֽ���Ҫ���ͷ�Ӧ��
	*array=IIC.RcvByte();
	IIC.NoAck_I2c();
	
	IIC.Stop_I2c();						//�ر�IIC

	return 1;
	
}