#ifndef __KEY_H__
#define __KEY_H__

typedef struct
{
	uint8_t volatile KEY_FLAG;		//按键标志位
	uint8_t Click;								//单击
	uint8_t Double_Click;					//双击
	uint8_t Press;								//长按
	void (*KEY_Detect)(void);			//按键检测
}Key_t;


#endif 