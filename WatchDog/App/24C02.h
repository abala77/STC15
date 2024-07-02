#ifndef __AT24C02_H__
#define __AT24C02_H__

//结构体
typedef struct
{
	uint8_t (*Sendstr)(uint8_t *,uint8_t );			//发送多个字节
	uint8_t (*Receivestr)(uint8_t *,uint8_t);			//接收多个字节
}AT24C02_t;


extern AT24C02_t AT24C02;

#endif