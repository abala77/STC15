#ifndef __KEY_H__
#define __KEY_H__

typedef struct
{
	uint8_t volatile KEY_FLAG;		//������־λ
	uint8_t Click;								//����
	uint8_t Double_Click;					//˫��
	uint8_t Press;								//����
	void (*KEY_Detect)(void);			//�������
}Key_t;


#endif 