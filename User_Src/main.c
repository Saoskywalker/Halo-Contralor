/**********************************************************************
Name: Halo Contralor 
Version: v0.5 Beta
Editor: Aysi
Function: 
History: 
2019.2.26: create
**********************************************************************/
#define DISABLE_IWDG

#include "AppLib.h"
#include "stdlib.h"

/******************************
wifi model
*******************************/
/* const char CreateAP[] = "AT+CWSAP=\"DDD\",\"123456789\",3,2";
const char CreateAP_Return[] = "OK";
const char CheckIP[] = "AT+CWLIF";
const char SendData[] = "AT+CIPSEND=";
const char WifiRst[] = "AT+RST";
const char WifiRst_Return[] = "OK";
const char LinkServer[] = "AT+CIPSTART=\"TCP\",\"192.168.4.100\",8080";
const char LinkServer_Return[] = "OK";

char *WifiCmd = 0;
char ReturnStr[20];

void WifiUart(const char *str)
{
	WifiCmd = (char *)str;
	for(; *str!='\0'; str++)
	{
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, *str);
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	}
}
 */
int main()
{	
	u8 i = 0;
	u8 ear_cnt = 0, KeyTemp = 0;

	NVIC_Configuration(); 	//NVIC group: 2:2
	delay_init();	//Init Systick timer
	uart1_init(115200);	//To PC
	BitErrorBit.RealTimer = RTC_Init();	//Init RTC
	printf("wake up\n");
	WKUP_Init(); //stand by & wake up init
	IO_Init();
	LED_RED_PIN = 1;
	uart2_init(115200);	//To distinguish
	uart3_init(9600);	//To music
	// Adc_Init();	
	EXTIX_Init();

	//ear reset(up)
	HeadMotorUp(0);
	delay_ms(300);
	while(HALL_EAR_PIN)
	{
		delay_ms(100);
		if(++ear_cnt>20)
		{
			BitErrorBit.ERROR = 1;
			printf("HALL_EAR ERROR\n");
			break;
		}		
	}
	HeadMotorStop();
	
	if(DHT11_Init()) //dht11 init
	{
		printf("dht11 ERROR\n"); 
		BitErrorBit.ERROR = 1;
	}	 

	if(MPU_Init()) //MPU6050
	{
		printf("MPU6050 ERROR\n");
		BitErrorBit.ERROR = 1;
	}
	i = mpu_dmp_init(); //Usually error
	if(i)
	{
		printf("MPU6050 DMP ERROR%d\n", i);
		// BitErrorBit.MPU6050 = 1;
	}	 

	if(DistInitialization()) //Dist init
	{
		printf("Dist ERROR\n"); 
		BitErrorBit.ERROR = 1;
	}	

	if(BitErrorBit.RealTimer) 
	{
		printf("RTC ERROR\n");
		BitErrorBit.ERROR = 1;
	}

	if(MusicInitialization()) //Music init;
	{
		printf("Music ERROR\n"); 
		BitErrorBit.MUSIC = 1;
	}	

	TIM1_PWM_Init(7199, 0); //Double motor PWM 10kHz
	TIM2_Int_Init(99,70);	//100us
	
#ifndef DISABLE_IWDG
//prescaler 256,reload 625,over time 4s
	IWDG_Init(IWDG_Prescaler_256,625);  
#endif
	printf("Time:%d-%d-%d %d:%d:%d\n", calendar.w_year,
				calendar.w_month, calendar.w_date, calendar.hour,
				calendar.min, calendar.sec); //输出时间
	if(BitErrorBit.ERROR==0)
	{
		printf("Runing\n");
		// for(k = 0; k<=20; k++) //检查音频序号
		// {
		// 	MusicStart(k);
		// 	delay_ms(1000);
		// 	delay_ms(500);
		// 	printf("%d\n", k);
		// }
		// RGB_Renew(160, 0, 80);	//现在触摸模块自动变为无,可能前面电压跳动,暂时不需
		// while(!KeyWakeUpPress);
		// while(KeyWakeUpPress);
		MusicStart(V_HELLO);
		RGB_Renew(160, 160, 0);
		// if (calendar.min >= 55) //close next 5 min
		// 	RTC_Alarm_Set(calendar.w_year, calendar.w_month, calendar.w_date,
		// 				  calendar.hour + 1, calendar.min - 55, calendar.sec);
		// else
			// RTC_Alarm_Set(calendar.w_year, calendar.w_month, calendar.w_date,
			// 			  calendar.hour, calendar.min+5, calendar.sec);
		srand(SysRunTime);	
		RandTime[0] = rand()%100/2*10;
		srand(RandTime[0]);
		RandTime[1] = rand()%100/2*10;
		if(RandTime[0]>RandTime[1])
		{
			RandTime[0] = RandTime[0]^RandTime[1];
			RandTime[1] = RandTime[0]^RandTime[1];
			RandTime[0] = RandTime[0]^RandTime[1];
		}
		CloseTime = 0;
		printf("%d, %d\n", RandTime[0], RandTime[1]);
	}		
	else
	{
		printf("ERROR\n");
		Sys_Enter_Standby(); //进入待机模式
	}

	while(1)
	{
		if(KeyWakeUpPressLong)
		{
			Sys_Enter_Standby(); //进入待机模式
		}	
		else if(KeyWakeUpPress)
		{
			MOUTH_PIN = 1;
			KeyTemp = 1;
		}
		else
		{
			if(KeyTemp)
			{
				ActionType = ACTION_INTERACTION;
			}
			KeyTemp = 0;
		}

		if (UART2_RX_Cnt != 0) //get Dist result
		{
			if ((DistStartOnceRe[0] == UART2_RX_Cache[0]))
				// (DIST_NI_HAO == UART2_RX_Cache[1]))
			{
				printf("result:%d\n", UART2_RX_Cache[1]);
				ActionType = ACTION_REMIND;
			}		
			UART2_RX_Cnt = 0;			
		}

		if(RTCAlarm)
			ActionType = ACTION_TIME;
		if(RandActionRun)
			ActionType = ACTION_RADOM;
			
		switch (ActionType)
		{
			case ACTION_INTERACTION:
			{
				Interaction();
				break;
			}

			case ACTION_RADOM:
			{
				RandomAction();
				break;
			}

			case ACTION_TIME:
			{
				TimeAction();
				break;
			}

			case ACTION_REMIND:
			{
				RemindAction();
				break;
			}

			default:
			{
				break;
			}
		}
		ActionType = 0;
	}
}
