#ifndef __AT24C02_H__
#define __AT24C02_H__

//�ṹ��
typedef struct
{
	uint8_t (*Sendstr)(uint8_t *,uint8_t );			//���Ͷ���ֽ�
	uint8_t (*Receivestr)(uint8_t *,uint8_t);			//���ն���ֽ�
}AT24C02_t;


extern AT24C02_t AT24C02;

#endif