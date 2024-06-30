#ifndef __IIC_H__
#define __IIC_H__

//�ṹ��
typedef struct
{
	volatile uint8_t slv_Ack;						//�ӻ�Ӧ���־=0��Ӧ��=1Ӧ��
	
	void (*I2C_Init)(void);
	void (*Start_I2c)(void);
	void (*Stop_I2c)(void);
	void (*SendByte)(uint8_t);
	uint8_t (*RcvByte)(void);
	void (*Ack_I2c)(void);
	void (*NoAck_I2c)(void);
}IIC_t;


extern IIC_t IIC;

#endif