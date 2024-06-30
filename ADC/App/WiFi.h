#ifndef __WIFI_H__
#define __WIFI_H__

//TCP���ݷ��������
typedef struct
{
	uint8_t volatile TCP_Connect_Status;		//TCP����״̬
	uint16_t TCP_Reconnect_Timer;						//TCP������ʱ��
	
	void (*Init)(void);											//��ʼ��
	void (*SmartConfig)(void);							//WiFiģ������
	void (*TCP_Connect_Server)(void);				//ͨ��TCP���ӷ�����
	void (*Send_data)(void);								//ͨ��TCP��������
	void (*Receive_data)(void);							//ͨ��TCP��������
	void (*Heart)(void);							//����
}WiFi_t;

//���Ժ���
typedef struct
{
	void (*Computer_to_ESP12S)(void);				//���Դ���->��Ƭ��->ESP12S
	void (*ESP12S_to_Computer)(void);				//ESP12S->��Ƭ��->���Դ���
}WiFi_Debug_t;


extern WiFi_t xdata esp12s;
extern WiFi_Debug_t xdata WiFi_Debug;

#endif