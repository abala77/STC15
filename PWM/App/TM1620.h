#ifndef __TM1620_H__
#define __TM1620_H__


//显示模式
typedef enum
{
	Disp_Mode_GRID4_SEG10 =0x00,
	Disp_Mode_GRID5_SEG9 =0x01,
	Disp_Mode_GRID6_SEG8 =0x02
}Disp_Mode_t;

//写数据地址
typedef enum
{
	Write_Data_Addr_Fix			=0x44,	//地址固定
	Write_Data_Addr_Auto_Add=0x40,	//自动加一
}Write_Data_Addr_Mode_t;

//辉度等级
typedef enum
{
	Brightness_level_0		= 0x80,
	Brightness_level_1  	= 0x88,
	Brightness_level_2  	= 0x89,
	Brightness_level_3  	= 0x8A,
	Brightness_level_4  	= 0x8B,
	Brightness_level_5  	= 0x8C,
	Brightness_level_6  	= 0x8D,
	Brightness_level_7  	= 0x8E,
	Brightness_level_8  	= 0x8F,
}Brightness_level_t;

//显示寄存器地址
typedef enum
{
	Disp_SFR_Addr_Num		=(uint8_t)12,
	
	Disp_SFR_Addr_00H		= 0xC0,
	Disp_SFR_Addr_01H  	= 0xC1,
	Disp_SFR_Addr_02H  	= 0xC2,
	Disp_SFR_Addr_03H  	= 0xC3,
	Disp_SFR_Addr_04H  	= 0xC4,
	Disp_SFR_Addr_05H  	= 0xC5,
	Disp_SFR_Addr_06H  	= 0xC6,
	Disp_SFR_Addr_07H  	= 0xC7,
	Disp_SFR_Addr_08H  	= 0xC8,
	Disp_SFR_Addr_09H  	= 0xC9,
	Disp_SFR_Addr_0AH  	= 0xCA,
	Disp_SFR_Addr_0BH  	= 0xCB,
}Disp_SFR_Addr_t;

//定义结构体类型
typedef struct
{
	Brightness_level_t Brightness;		//辉度等级
	
	void (*Init)(void);																			//初始化
	void (*Disp_Number)(uint8_t GPIR,uint8_t Number);				//设置数字
	void (*Delay_Bright_ADJ)(uint8_t);											//亮度调节
}TM1620_t;

extern TM1620_t xdata TM1620;

#endif