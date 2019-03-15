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
	NVIC_Configuration(); 	//NVIC group: 2:2
	delay_init();	//Init Systick timer
	//WKUP_Init(); //stand by & wake up init
	IO_Init();
	uart1_init(115200);	//To PC
	printf("wake up\n");
	uart2_init(115200);	//To distinguish
	uart3_init(9600);	//To music
	// Adc_Init();	
	EXTIX_Init();
	// RTC_Init(); //RTC init, osc have problem

	if(DHT11_Init()) //dht11 init
	{
		printf("dht11 ERROR\n"); 
	}	 

	MPU_Init(); //MPU6050
	if(mpu_dmp_init())
	{
		printf("MPU6050 ERROR\n");
	}	 

	TIM1_PWM_Init(7199, 0); //Double motor PWM 10kHz
	TIM2_Int_Init(999,70);	//1ms
	
#ifndef DISABLE_IWDG
//prescaler 256,reload 625,over time 4s
	IWDG_Init(IWDG_Prescaler_256,625);  
#endif
			
	while(1)
	{
		if (Check_WKUP())
			Sys_Enter_Standby(); //不是开机,进入待机模式
	}
}
