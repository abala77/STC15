/*-----------------------------
1.使用查表法检测当前的环境温度
2.使用公式法检测当前的环境温度
------------------------------*/
#include <math.h>
#include "main.h"

#define R0 10000.0		//参考温度下的电阻值
#define B  3950.0			//NTC热敏电阻的B值
#define T0 298.15     // 参考温度，25℃，转换为开尔文（273.15 + 25） 

#define PrintTemp_Debug	//调试开关

//温度表格
uint16_t code TempArray[101][2]=\
{
{	958	,	963	},
{	955	,	957	},
{	951	,	954	},
{	948	,	950	},
{	944	,	947	},
{	940	,	943	},
{	936	,	939	},
{	932	,	935	},
{	927	,	931	},
{	922	,	926	},
{	917	,	921	},
{	912	,	916	},
{	907	,	911	},
{	902	,	906	},
{	896	,	901	},
{	890	,	895	},
{	884	,	889	},
{	878	,	883	},
{	871	,	877	},
{	864	,	870	},
{	857	,	863	},
{	850	,	856	},
{	843	,	849	},
{	835	,	842	},
{	827	,	834	},
{	819	,	826	},
{	811	,	818	},
{	803	,	810	},
{	794	,	802	},
{	785	,	793	},
{	776	,	784	},
{	767	,	775	},
{	757	,	766	},
{	748	,	756	},
{	738	,	747	},
{	728	,	737	},
{	718	,	727	},
{	708	,	717	},
{	697	,	707	},
{	687	,	696	},
{	676	,	686	},
{	665	,	675	},
{	654	,	664	},
{	640	,	655	},
{	631	,	639	},
{	620	,	630	},
{	608	,	619	},
{	597	,	607	},
{	586	,	596	},
{	574	,	585	},
{	565	,	573	},
{	554	,	564	},
{	540	,	553	},
{	529	,	539	},
{	517	,	528	},
{	506	,	516	},
{	496	,	505	},
{	485	,	495	},
{	475	,	484	},
{	464	,	474	},
{	453	,	463	},
{	442	,	452	},
{	432	,	441	},
{	421	,	431	},
{	411	,	420	},
{	400	,	410	},
{	390	,	399	},
{	380	,	389	},
{	370	,	379	},
{	360	,	369	},
{	351	,	359	},
{	342	,	350	},
{	332	,	341	},
{	323	,	331	},
{	315	,	322	},
{	306	,	314	},
{	297	,	305	},
{	289	,	296	},
{	281	,	288	},
{	273	,	280	},
{	266	,	272	},
{	258	,	265	},
{	250	,	257	},
{	243	,	249	},
{	236	,	242	},
{	229	,	235	},
{	223	,	228	},
{	216	,	222	},
{	210	,	215	},
{	204	,	209	},
{	200	,	203	},
{	192	,	199	},
{	186	,	191	},
{	181	,	185	},
{	176	,	180	},
{	170	,	175	},
{	165	,	169	},
{	161	,	164	},
{	156	,	160	},
{	151	,	155	},
{	147	,	150	}
};




static void Formula_CalcTemp(void);		//计算法得到当前温度
static void Form_CheckTemp(void);				//表格计算温度

NTC_t NTC=
{
	0,
	
	Formula_CalcTemp,
	Form_CheckTemp
};


//公式法测量当前温度
static void Formula_CalcTemp()
{
	float Recent_Resistance;		//当前阻值
	uint16_t ADC_Value=0;
	float Current_Voltage=0;
	
	ADC_Value=ADC.ADC_MeasureFilter();																							//采集到ADC的值
	Current_Voltage=ADC_Value/1023.00*3.30;																					//换算出来的电压
	Recent_Resistance=Current_Voltage/((3.30-Current_Voltage)/10000);						//使用欧姆定律求出电阻R=U/I
	NTC.Recent_Tempurature=(1/((1/T0)+(1/B)*log(Recent_Resistance/R0)))-273.15+0.5;	//使用B值计算当前温度

	#ifdef PrintTemp_Debug
		UART1.RS485_Send_Mode();
		printf("Calc_Recent_Tempurature=%.2f\n",NTC.Recent_Tempurature);
		UART1.RS485_Receive_Mode();
	#endif
}

//查表法测量温度
static void Form_CheckTemp()
{
	float ADC_Value=0;
	uint8_t left=0,right=100,mid=50;
	
	ADC_Value=ADC.ADC_MeasureFilter();	//检测当前AD电压
	
	//使用二分法查找数据
	while(left<right)
	{
		if(ADC_Value>=TempArray[mid][0] && ADC_Value<=TempArray[mid][1])
		{
			break;
		}
		else if(ADC_Value<TempArray[mid][0])
		{
			left=mid+1;
		}
		else if(ADC_Value>TempArray[mid][1])
		{
			right=mid-1;
		}
		mid=left+(right-left)/2;
	}
	
	//根据查表计算温度
	NTC.Recent_Tempurature=(mid+(ADC_Value-TempArray[mid][0])/(TempArray[mid][1]-TempArray[mid][0]))-30;
	
	#ifdef PrintTemp_Debug
		UART1.RS485_Send_Mode();
		printf("ADC_Value=%.2f\n",ADC_Value);
		printf("mid=%bu\n",mid);
		printf("TempArray[%bu][0]=%hu\n",mid,TempArray[mid][0]);
		printf("TempArray[%bu][1]=%hu\n",mid,TempArray[mid][1]);
		printf("Check_Recent_Tempurature=%.2f\n",NTC.Recent_Tempurature);
		UART1.RS485_Receive_Mode();
	#endif
}

