#ifndef __KEY2_H__
#define __KEY2_H__


//����ö������
typedef enum
{
	STA1_KEY_Up			=(uint8_t)0x01,//��������
	STA2_KEY_DownShake	=(uint8_t)0x02,//��������
	STA3_KEY_Down		=(uint8_t)0x03,//��������
	STA4_KEY_UpShake	=(uint8_t)0x04 //���𶶶�
}STA_Machine_Status_t;

//����ṹ������
typedef struct
{
	STA_Machine_Status_t ucSTA_Machine_Status;		//״̬��״̬
	uint16_t volatile 	 ucSTA_Machine_Scan_Timer;	//״̬��ɨ�趨ʱ��
	uint16_t volatile	 usKEY2_Doule_Click_Timer;	//KEY2˫����ʱ��
	uint16_t volatile 	 usKEY2_Press_Timer;		//KEY2������ʱ��
}STA_Machine_t;


//�ṹ������
extern Key_t		KEY2;
extern STA_Machine_t  STA_Machine;


#endif 