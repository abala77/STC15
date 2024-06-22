#include "main.h"

#define STB P26
#define CLK P25
#define DIN P24

#define GPIR1 Disp_SFR_Addr_00H
#define GPIR2 Disp_SFR_Addr_02H

uint8_t code num[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};  //��������� 0 - 9


static void Init(void);														//��ʼ��
static void Disp_Number(uint8_t ,uint8_t );				//��������
static void Delay_Bright_ADJ(uint8_t );		//���ȵ��ں���ʱ

static void TM1620_Write_Byte(uint8_t);    //TM1620д���ֽ�

//�ṹ��
TM1620_t xdata TM1620=
{
	Brightness_level_3,
	Init,
	Disp_Number,
	Delay_Bright_ADJ
};



/*
  *  @name   TM1620_Write_Byte
  *  @brief  дһ���ֽ�
  *  @param  dat:һ���ֽ�����
  *  @retval None
*/
static void TM1620_Write_Byte(uint8_t dat)
{
	uint8_t TM1620_i=0;
	STB=0;		//STB�½��ؽ������ݽ���ģʽ
	Public.Delayms(1);
	for(TM1620_i=0;TM1620_i<8;TM1620_i++)
	{
		//CLK�½���ʱ׼������
		CLK=0;
		if((dat&bit0)==bit0)
			DIN=1;
		else
			DIN=0;
		dat=dat>>1;//���Ƶ���һλ
		_nop_();

		//CLK�����ض�ȡ����
		CLK=1;
		_nop_();
	}
}

//��ʼ��
static void Init()
{
	uint8_t init_i=0;
	//������ʾģʽ
	TM1620_Write_Byte(Disp_Mode_GRID6_SEG8);
	STB=1;
	
	//����д�Դ����������Զ���һ
	TM1620_Write_Byte(Write_Data_Addr_Auto_Add);
	STB=1;
	
	//��������
	TM1620_Write_Byte(Disp_SFR_Addr_00H);
	for(init_i=0;init_i<Disp_SFR_Addr_Num;init_i++)
		TM1620_Write_Byte(0x00);
	STB=1;
	
	//���ûԶ�
	TM1620_Write_Byte(TM1620.Brightness);
	STB=1;
	
	
	//������ʾģʽ
	TM1620_Write_Byte(Disp_Mode_GRID6_SEG8);
	STB=1;
	
	//����д�Դ����������̶���ַ
	TM1620_Write_Byte(Write_Data_Addr_Fix);
	STB=1;
	
	//д��ʾ�Ĵ���
	TM1620_Write_Byte(GPIR1);
	TM1620_Write_Byte(0xFF);
	STB=1;
	
	TM1620_Write_Byte(GPIR2);
	TM1620_Write_Byte(0xFF);
	STB=1;
	
	//���ûԶ�
	TM1620_Write_Byte(TM1620.Brightness);
	STB=1;
}

//��ʾ����
static void Disp_Number(uint8_t GPIR,uint8_t Num)
{
	//ָ���Ĵ���
	switch(GPIR)
	{
		case 1:
			TM1620_Write_Byte(GPIR1);
			break;
		case 2:
			TM1620_Write_Byte(GPIR2);
			break;	
	}
	//��������
	TM1620_Write_Byte(num[Num]);
	STB=1;
	
	//���ûԶ�
	TM1620_Write_Byte(TM1620.Brightness);
	STB=1;
}

//��ʱ�͵�������
static void Delay_Bright_ADJ(uint8_t ms)
{
	uint8_t k=0;
	
	for(k=0;k<ms;k++)
	{
		Key1.KEY_Detect();
		if(Key1.Press==true)
		{
			TM1620.Brightness=Brightness_level_0;
			break;
		}
		else if(Key1.Click==true && Key1.Press==false)
		{
			switch(TM1620.Brightness)
			{
				case Brightness_level_0: TM1620.Brightness=Brightness_level_1;break;
				case Brightness_level_1: TM1620.Brightness=Brightness_level_2;break;
				case Brightness_level_2: TM1620.Brightness=Brightness_level_3;break;
				case Brightness_level_3: TM1620.Brightness=Brightness_level_4;break;
				case Brightness_level_4: TM1620.Brightness=Brightness_level_5;break;
				case Brightness_level_5: TM1620.Brightness=Brightness_level_6;break;
				case Brightness_level_6: TM1620.Brightness=Brightness_level_7;break;
				case Brightness_level_7: TM1620.Brightness=Brightness_level_8;break;
				case Brightness_level_8: TM1620.Brightness=Brightness_level_1;break;
			}
			break;
		}
		Public.Delayms(1);
	}
}
