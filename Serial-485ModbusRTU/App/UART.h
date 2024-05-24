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

//校验位
typedef enum
{
	None=0,
	Odd=1,
	Even=2
}Parity_t;

//通信方式
typedef enum
{
	TTL,
	RS485,
	RS232
}Interface_Type_t;

//串口结构体
typedef struct
{
	Parity_t   ParityBit;//校验位
	BaudRate_t BaudRate;
	uint8_t	volatile	 Tx_Busy_Flag;	//发送忙碌标志位
	uint8_t	volatile	 Rx_Busy_Flag;	//接收忙碌标志位
	uint8_t volatile 	 ucRec_Cnt;		  //接收计数位
	
	uint8_t *Receive_Data;						//接收缓存指针
	
	void (*UART_Init)(void);					//串口初始化
	void (*SendByte)(uint8_t);				//发送一个字节数据（一个字符）
	void (*SendString)(uint8_t*);			//发送字符串
	void (*Protocol)(void);	
	
	//Interface_Type_t 
	void (*RS485_Send_Mode)(void);		//485发送模式
	void (*RS485_Receive_Mode)(void);		//485接收模式
}UART_t;


#endif 