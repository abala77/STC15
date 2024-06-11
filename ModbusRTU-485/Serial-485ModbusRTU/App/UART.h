#ifndef __UART_H__
#define __UART_H__

//������
typedef enum
{
	Baud_4800=0,
	Baud_9600=1,
	Baud_19200=2,
	Baud_115200=3
}BaudRate_t;

//У��λ
typedef enum
{
	None=0,
	Odd=1,
	Even=2
}Parity_t;

//ͨ�ŷ�ʽ
typedef enum
{
	TTL,
	RS485,
	RS232
}Interface_Type_t;

//���ڽṹ��
typedef struct
{
	Parity_t   ParityBit;//У��λ
	BaudRate_t BaudRate;
	uint8_t	volatile	 Tx_Busy_Flag;	//����æµ��־λ
	uint8_t	volatile	 ucRec_Flag;	//���ձ�־λ
	uint8_t volatile 	 ucRec_Cnt;		  //���ռ���λ

	uint8_t *pucSend_Buffer;				//���ͻ���ָ��
	uint8_t *pucRec_Buffer;						//���ջ���ָ��
	
	void (*UART_Init)(void);					//���ڳ�ʼ��
	void (*SendByte)(uint8_t);				//����һ���ֽ����ݣ�һ���ַ���
	void (*SendArray)(uint8_t*,uint8_t);	//��������
	void (*SendString)(uint8_t*);			//�����ַ���
	void (*Protocol)(void);					//ModbusRTUЭ��
	
	Interface_Type_t Interface_Type;	//�ӿ�����
	void (*RS485_Send_Mode)(void);		//485����ģʽ
	void (*RS485_Receive_Mode)(void);		//485����ģʽ
}UART_t;


#endif 