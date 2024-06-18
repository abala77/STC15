#ifndef __MODBUSRTU_H__
#define __MODBUSRTU_H__

typedef struct 
{
	uint8_t Addr;//本机地址

	void (*Protocol_Analysis)(UART_t*);//协议分析，解析发送来的数据、发送数据给对方
}Modbus_t;

extern Modbus_t idata Modbus;

#endif 