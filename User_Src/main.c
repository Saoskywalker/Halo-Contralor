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

	NVIC_Configuration(); 	//NVIC group: 2:2
	delay_init();	//Init Systick timer
	WKUP_Init(); //stand by & wake up init
	IO_Init();
	uart1_init(115200);	//To PC
	printf("wake up\n");
	uart2_init(115200);	//To distinguish
	uart3_init(9600);	//To music
	// Adc_Init();	
	EXTIX_Init();
	delay_ms(500);

	if(DHT11_Init()) //dht11 init
	{
		printf("dht11 ERROR\n"); 
		BitErrorBit.DHT11 = 1;
	}	 

	if(MPU_Init()) //MPU6050
	{
		printf("MPU6050 ERROR\n");
		BitErrorBit.MPU6050 = 1;
	}
	i = mpu_dmp_init();
	if(i)
	{
		printf("MPU6050 DMP ERROR%d\n", i);
		BitErrorBit.MPU6050 = 1;
	}	 

	if(DistInitialization()) //Dist init
	{
		printf("Dist ERROR\n"); 
		BitErrorBit.DISTINGUISH = 1;
	}	

	if(MusicInitialization()) //Music init; Hardware bad
	{
		printf("Music ERROR\n"); 
		BitErrorBit.MUSIC = 1;
	}	

	if(RTC_Init()) //RTC init, osc have problem
	{
		printf("RTC ERROR\n");
		BitErrorBit.RealTimer = 1;
	}


	TIM1_PWM_Init(7199, 0); //Double motor PWM 10kHz
	TIM2_Int_Init(99,70);	//100us
	
#ifndef DISABLE_IWDG
//prescaler 256,reload 625,over time 4s
	IWDG_Init(IWDG_Prescaler_256,625);  
#endif

	if(BitErrorBit.MPU6050==0)
	{
		printf("Runing\n");
	}		
	else
	{
		printf("ERROR\n");
		//Sys_Enter_Standby(); //进入待机模式
	}
	while(KeyWakeUpPress);	//wait key free		
	while(1)
	{
		// if (KeyWakeUpPressLong) //press 2s
		// {
		// 	Sys_Enter_Standby();
		// }
		 if(WAKE_UP_PIN)
		{
			MusicInitialization();
		}
		delay_ms(200);
		LED_GREEN_PIN = 0;
		LED_RED_PIN = ~LED_RED_PIN;
		LED_BLUE_PIN = 1;
		MOUTH_PIN = ~MOUTH_PIN;
		MOTOR1_PIN = 1;
		// MOTOR2_PIN = 1;
		// TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
		// TIM_SetCompare1(TIM1,ScrubberPWMIntensity);	
		delay_ms(200);
		MOUTH_PIN = ~MOUTH_PIN;
		MOTOR1_PIN = 1;
		delay_ms(200);
		LED_RED_PIN = ~LED_RED_PIN;
		MOUTH_PIN = ~MOUTH_PIN;
		// MOTOR1_PIN = 0;
		// MOTOR2_PIN = 0;		
	}
}
