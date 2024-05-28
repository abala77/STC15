#include "main.h"

#define FuntionCode_Read_03 0x03
#define FuntionCode_Write_06 0x06

static void Protocol_Analysis(UART_t*);//协议解析
static void Modbus_Read_Register(UART_t*);//读寄存器
static void Modbus_Write_Register(UART_t*);//写寄存器

Modbus_t idata Modbus={1,Protocol_Analysis};


/*
  *  @name   Protocol_Analysis
  *  @brief  解析报文，并根据报文进行响应
  *  @param  UART:结构体指针
  *  @retval None
*/
static void Protocol_Analysis(UART_t* UART)
{
	UART_t* const xdata COM=UART;
	
	//核对地址
	if(*(COM->pucRec_Buffer+0)==Modbus.Addr)
	{
		//计算校验码
		CRC_16.CRC=CRC_16.CRC_Check(COM->pucRec_Buffer,6);
		CRC_16.CRC_H=CRC_16.CRC>>8;
		CRC_16.CRC_L=(uint8_t)CRC_16.CRC;
		//核对校验码
		if(((*(COM->pucRec_Buffer+6)==CRC_16.CRC_L) && (*(COM->pucRec_Buffer+7)==CRC_16.CRC_H))
												||
		((*(COM->pucRec_Buffer+6)==CRC_16.CRC_H) && (*(COM->pucRec_Buffer+7)==CRC_16.CRC_L)))
		{
			
			//判断功能码
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
				printf("CRC16 Check ERROR！！");
				UART1.RS485_Receive_Mode();
			#endif 
		}
	}
}


/*
  *  @name   Modbus_Read_Register
  *  @brief  响应对方读寄存器操作
  *  @param  UART:结构体指针
  *  @retval None
*/
static void Modbus_Read_Register(UART_t* UART)
{
	UART_t* const xdata COM=UART;//保护数据
	uint16_t xdata Start_Regist_Addr=0;//读出起始地址
	uint16_t xdata Read_Regist_Num=0;//读出读取数量
	uint8_t xdata Check_LEN=3;//跟踪数据长度
	uint8_t Read_Reg_i;
	uint8_t Read_Reg_j;
	//计算寄存器开始地址
	Start_Regist_Addr=((Start_Regist_Addr+*(COM->pucRec_Buffer+2))<<8)+*(COM->pucRec_Buffer+3);
	//计算需要读取寄存器数量
	Read_Regist_Num=((Read_Regist_Num+*(COM->pucRec_Buffer+4))<<8)+*(COM->pucRec_Buffer+5);
	//不超出全局数组范围，并且读取数量大于0小于3
	if(((Start_Regist_Addr+Read_Regist_Num)<=Modbus_Regist_LEN) && Read_Regist_Num>0 && Read_Regist_Num<3)
	{
		//地址
		*(COM->pucSend_Buffer+0)=Modbus.Addr;
		//功能码
		*(COM->pucSend_Buffer+1)=FuntionCode_Read_03;
		//数据长度（字节）
		*(COM->pucSend_Buffer+2)=(uint8_t)Read_Regist_Num*2;
		//装载寄存器数据
		for(Read_Reg_i=0;Read_Reg_i<Read_Regist_Num;Read_Reg_i++)
		{
			for(Read_Reg_j=Read_Reg_i;Read_Reg_j<=(Read_Reg_i+1);Read_Reg_j++)
			{
				if(Read_Reg_j==Read_Reg_i)
				{
					*(COM->pucSend_Buffer+3+Read_Reg_i+Read_Reg_j)=Modbus_Regist[Start_Regist_Addr+Read_Reg_i]>>8;//高位
				}
				else
				{
					*(COM->pucSend_Buffer+3+Read_Reg_i+Read_Reg_j)=(uint8_t)Modbus_Regist[Start_Regist_Addr+Read_Reg_i];//低位
				}
			}
			Check_LEN=Check_LEN+2;
		}

		//校验码
		CRC_16.CRC=CRC_16.CRC_Check(COM->pucSend_Buffer,Check_LEN);
		*(COM->pucSend_Buffer+(Check_LEN++))=(uint8_t)CRC_16.CRC;
		*(COM->pucSend_Buffer+(Check_LEN++))=CRC_16.CRC>>8;
		
		//发送数据
		COM->SendArray(COM->pucSend_Buffer,Check_LEN);
	}
	else if((Start_Regist_Addr+Read_Regist_Num)>Modbus_Regist_LEN)
	{
		#if TestPrint
			UART1.RS485_Send_Mode();
			printf("Regist address out of range！！0~9\n");
			UART1.RS485_Receive_Mode();
		#endif 
	}
	else
	{
		#if TestPrint
			UART1.RS485_Send_Mode();
			printf("Only read two regist once！！\n");
			UART1.RS485_Receive_Mode();
		#endif 
	}
}


/*
  *  @name   Modbus_Write_Register
  *  @brief  响应对方要写寄存器操作
  *  @param  UART:结构体指针
  *  @retval None
*/
static void Modbus_Write_Register(UART_t* UART)
{
	UART_t* const xdata COM=UART;
	uint16_t xdata Write_RegAddr=0;//需要写的寄存器地址
	uint16_t xdata Write_RegData=0;//需要写的数据
	uint8_t Send_i;

	for(Send_i=0;Send_i<8;Send_i++)
	{
		*(COM->pucSend_Buffer+Send_i)=*(COM->pucRec_Buffer+Send_i);
	}
	COM->SendArray(COM->pucSend_Buffer,8);

	Write_RegAddr=((Write_RegAddr+*(COM->pucRec_Buffer+2))<<8)+*(COM->pucRec_Buffer+3);
	Write_RegData=((Write_RegAddr+*(COM->pucRec_Buffer+4))<<8)+*(COM->pucRec_Buffer+5);
	
	//在0~9范围内
	if(Write_RegAddr<=9 && Write_RegAddr>=0)
	{
		Modbus_Regist[Write_RegAddr]=Write_RegData;//赋值给数组
	}
	else
	{
		#if TestPrint
			UART1.RS485_Send_Mode();
			printf("Regist address out of range2！！0~9\n");
			UART1.RS485_Receive_Mode();
		#endif 
	}
}
