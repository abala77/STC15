#ifndef __CRC_16_H__
#define __CRC_16_H__

//CRC校验结构体
typedef struct
{
	uint16_t CRC;
	uint8_t CRC_H;
	uint8_t CRC_L;
	
	uint16_t (*CRC_Check)(uint8_t*,uint8_t);
}CRC_16_t;

extern CRC_16_t xdata CRC_16;

#endif