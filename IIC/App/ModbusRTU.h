#ifndef __MODBUSRTU_H__
#define __MODBUSRTU_H__

typedef struct 
{
	uint8_t Addr;//������ַ

	void (*Protocol_Analysis)(UART_t*);//Э����������������������ݡ��������ݸ��Է�
}Modbus_t;

extern Modbus_t idata Modbus;

#endif 