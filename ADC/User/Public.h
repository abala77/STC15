#ifndef __PUBLIC_H__
#define __PUBLIC_H__


#define Modbus_Regist_LEN 10		//�Ĵ������鳤��


//���������ض���
typedef signed char      sint8_t;
typedef signed short int sint16_t;
typedef signed long int  sint32_t;

typedef unsigned char      uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int  uint32_t;

//Bitλ����
typedef enum
{
	bit0 =(uint8_t)(0x01<<0),
	bit1 =(uint8_t)(0x01<<1),
	bit2 =(uint8_t)(0x01<<2),
	bit3 =(uint8_t)(0x01<<3),
	bit4 =(uint8_t)(0x01<<4),
	bit5 =(uint8_t)(0x01<<5),
	bit6 =(uint8_t)(0x01<<6),
	bit7 =(uint8_t)(0x01<<7),
}Bit_t;

//�������Ͷ���
typedef enum
{
	false=(bit)0,
	true=(bit)1
}Bool_t;

//��������
typedef struct
{
	void (*Delayms)(int);
	void (*Error_Handler)(void);
	void (*softReset)(void);
	void (*Led_Flip)(void);
	void (*Memory_Clr)(uint8_t*,uint8_t);
}Public_t;

//��������
extern Public_t Public;
extern uint16_t xdata Modbus_Regist[];//ModbusRTU�Ĵ���
#endif