#ifndef __UART_H__
#define __UART_H__

//波特率
typedef enum
{
	Baud_4800=0,
	Baud_9600=1,
	Baud_19200=2,
	Baud_115200=3
}BaudRate_t;

//串口结构体
typedef struct
{
	BaudRate_t BaudRate;
	uint8_t	volatile	 Tx_Busy_Flag;//发送忙碌标志位
	uint8_t	volatile	 Rx_Busy_Flag;//接收忙碌标志位
	uint8_t volatile 	 ucRec_Cnt;//接收计数位
	
	uint8_t *Receive_Data;
	
	void (*UART_Init)(void);
	void (*SendByte)(uint8_t);
	void (*SendString)(uint8_t*);
}UART_t;


#endif 