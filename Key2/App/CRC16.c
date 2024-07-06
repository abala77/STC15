#include "main.h"

static uint16_t CRC_Check(uint8_t*,uint8_t);

CRC_16_t xdata CRC_16={0,0,0,CRC_Check};



/*
	* @name   CRC_Check
	* @brief  CRC校验
	* @param  CRC_Ptr->数组指针，LEN->长度
	* @retval CRC校验值      
*/
static uint16_t CRC_Check(uint8_t *CRC_Ptr,uint8_t LEN)
{
	uint16_t CRC = 0;
	uint8_t CRC_i   = 0;
	uint8_t CRC_j   = 0;

	CRC = 0xffff;
	for(CRC_i=0;CRC_i<LEN;CRC_i++)
	{
		CRC ^= (uint16_t)(*(CRC_Ptr+CRC_i));
		for(CRC_j=0;CRC_j<8;CRC_j++)
		{
			if(CRC & 0x0001)
				CRC = (CRC >> 1) ^ 0xA001;
			else
				CRC = (CRC >> 1);
		}
	}
	
	return CRC;
}
