#include "main.h"

#define FuntionCode_Read_03 0x03
#define FuntionCode_Write_06 0x06

static void Protocol_Analysis(UART_t*);//Э�����
static void Modbus_Read_Register(UART_t*);//���Ĵ���
static void Modbus_Write_Register(UART_t*);//д�Ĵ���

Modbus_t idata Modbus={1,Protocol_Analysis};


/*
  *  @name   Protocol_Analysis
  *  @brief  �������ģ������ݱ��Ľ�����Ӧ
  *  @param  UART:�ṹ��ָ��
  *  @retval None
*/
static void Protocol_Analysis(UART_t* UART)
{
	UART_t* const xdata COM=UART;
	
	//�˶Ե�ַ
	if(*(COM->pucRec_Buffer+0)==Modbus.Addr)
	{
		//����У����
		CRC_16.CRC=CRC_16.CRC_Check(COM->pucRec_Buffer,6);
		CRC_16.CRC_H=CRC_16.CRC>>8;
		CRC_16.CRC_L=(uint8_t)CRC_16.CRC;
		//�˶�У����
		if(((*(COM->pucRec_Buffer+6)==CRC_16.CRC_L) && (*(COM->pucRec_Buffer+7)==CRC_16.CRC_H))
												||
		((*(COM->pucRec_Buffer+6)==CRC_16.CRC_H) && (*(COM->pucRec_Buffer+7)==CRC_16.CRC_L)))
		{
			
			//�жϹ�����
			if(*(COM->pucRec_Buffer+1)==FuntionCode_Read_03)
			{
				Modbus_Read_Register(COM);
			}
			else if(*(COM->pucRec_Buffer+1)==FuntionCode_Write_06)
			{
				Modbus_Write_Register(COM);
			}
		}
		else
		{
			#if TestPrint
				UART1.RS485_Send_Mode();
				printf("CRC16 Check ERROR����");
				UART1.RS485_Receive_Mode();
			#endif 
		}
	}
}


/*
  *  @name   Modbus_Read_Register
  *  @brief  ��Ӧ�Է����Ĵ�������
  *  @param  UART:�ṹ��ָ��
  *  @retval None
*/
static void Modbus_Read_Register(UART_t* UART)
{
	UART_t* const xdata COM=UART;//��������
	uint16_t xdata Start_Regist_Addr=0;//������ʼ��ַ
	uint16_t xdata Read_Regist_Num=0;//������ȡ����
	uint8_t xdata Check_LEN=3;//�������ݳ���
	uint8_t Read_Reg_i;
	uint8_t Read_Reg_j;
	//����Ĵ�����ʼ��ַ
	Start_Regist_Addr=((Start_Regist_Addr+*(COM->pucRec_Buffer+2))<<8)+*(COM->pucRec_Buffer+3);
	//������Ҫ��ȡ�Ĵ�������
	Read_Regist_Num=((Read_Regist_Num+*(COM->pucRec_Buffer+4))<<8)+*(COM->pucRec_Buffer+5);
	//������ȫ�����鷶Χ�����Ҷ�ȡ��������0С��3
	if(((Start_Regist_Addr+Read_Regist_Num)<=Modbus_Regist_LEN) && Read_Regist_Num>0 && Read_Regist_Num<3)
	{
		//��ַ
		*(COM->pucSend_Buffer+0)=Modbus.Addr;
		//������
		*(COM->pucSend_Buffer+1)=FuntionCode_Read_03;
		//���ݳ��ȣ��ֽڣ�
		*(COM->pucSend_Buffer+2)=(uint8_t)Read_Regist_Num*2;
		//װ�ؼĴ�������
		for(Read_Reg_i=0;Read_Reg_i<Read_Regist_Num;Read_Reg_i++)
		{
			for(Read_Reg_j=Read_Reg_i;Read_Reg_j<=(Read_Reg_i+1);Read_Reg_j++)
			{
				if(Read_Reg_j==Read_Reg_i)
				{
					*(COM->pucSend_Buffer+3+Read_Reg_i+Read_Reg_j)=Modbus_Regist[Start_Regist_Addr+Read_Reg_i]>>8;//��λ
				}
				else
				{
					*(COM->pucSend_Buffer+3+Read_Reg_i+Read_Reg_j)=(uint8_t)Modbus_Regist[Start_Regist_Addr+Read_Reg_i];//��λ
				}
			}
			Check_LEN=Check_LEN+2;
		}

		//У����
		CRC_16.CRC=CRC_16.CRC_Check(COM->pucSend_Buffer,Check_LEN);
		*(COM->pucSend_Buffer+(Check_LEN++))=(uint8_t)CRC_16.CRC;
		*(COM->pucSend_Buffer+(Check_LEN++))=CRC_16.CRC>>8;
		
		//��������
		COM->SendArray(COM->pucSend_Buffer,Check_LEN);
	}
	else if((Start_Regist_Addr+Read_Regist_Num)>Modbus_Regist_LEN)
	{
		#if TestPrint
			UART1.RS485_Send_Mode();
			printf("Regist address out of range����0~9\n");
			UART1.RS485_Receive_Mode();
		#endif 
	}
	else
	{
		#if TestPrint
			UART1.RS485_Send_Mode();
			printf("Only read two regist once����\n");
			UART1.RS485_Receive_Mode();
		#endif 
	}
}


/*
  *  @name   Modbus_Write_Register
  *  @brief  ��Ӧ�Է�Ҫд�Ĵ�������
  *  @param  UART:�ṹ��ָ��
  *  @retval None
*/
static void Modbus_Write_Register(UART_t* UART)
{
	UART_t* const xdata COM=UART;
	uint16_t xdata Write_RegAddr=0;//��Ҫд�ļĴ�����ַ
	uint16_t xdata Write_RegData=0;//��Ҫд������
	uint8_t Send_i;

	for(Send_i=0;Send_i<8;Send_i++)
	{
		*(COM->pucSend_Buffer+Send_i)=*(COM->pucRec_Buffer+Send_i);
	}
	COM->SendArray(COM->pucSend_Buffer,8);

	Write_RegAddr=((Write_RegAddr+*(COM->pucRec_Buffer+2))<<8)+*(COM->pucRec_Buffer+3);
	Write_RegData=((Write_RegAddr+*(COM->pucRec_Buffer+4))<<8)+*(COM->pucRec_Buffer+5);
	
	//��0~9��Χ��
	if(Write_RegAddr<=9 && Write_RegAddr>=0)
	{
		Modbus_Regist[Write_RegAddr]=Write_RegData;//��ֵ������
	}
	else
	{
		#if TestPrint
			UART1.RS485_Send_Mode();
			printf("Regist address out of range2����0~9\n");
			UART1.RS485_Receive_Mode();
		#endif 
	}
}
