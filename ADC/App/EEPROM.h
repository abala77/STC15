#ifndef __EEPROM_H__
#define __EEPROM_H__

#define EEPROM_Default_Addr 	0x0000
#define EEPROM_Cnt 						10

//定义结构体
typedef struct
{
	uint8_t ucEEPROM_Flag;		//EEPROM操作标志位
	uint8_t ucEEPROM_Cnt;		//EEPEOM操作次数
	
	uint8_t (*EEPROM_ReadByte)(uint16_t);		//读一个字节
	void (*EEPROM_WriteByte)(uint16_t,uint8_t);		//写一个字节
	void (*EEPROM_EraseSector)(uint16_t);			//擦除扇区
}EEPROM_t;


extern EEPROM_t EEPROM;


#endif