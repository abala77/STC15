#ifndef __EEPROM_H__
#define __EEPROM_H__

#define EEPROM_Default_Addr 	0x0000
#define EEPROM_Cnt 						10

//����ṹ��
typedef struct
{
	uint8_t ucEEPROM_Flag;		//EEPROM������־λ
	uint8_t ucEEPROM_Cnt;		//EEPEOM��������
	
	uint8_t (*EEPROM_ReadByte)(uint16_t);		//��һ���ֽ�
	void (*EEPROM_WriteByte)(uint16_t,uint8_t);		//дһ���ֽ�
	void (*EEPROM_EraseSector)(uint16_t);			//��������
}EEPROM_t;


extern EEPROM_t EEPROM;


#endif