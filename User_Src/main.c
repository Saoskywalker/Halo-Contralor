/**********************************************************************
Name: Halo Contralor 
Version: v0.5 Beta
Editor: Aysi
Function: 
History: 
2019.2.26: create
**********************************************************************/
//#define DEBUG
#include "stm32f10x.h"
#include "sys.h"
#include "timer.h"
#include "exti.h"
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
	IO_Init();
	uart1_init(115200);	//To PC
	uart2_init(115200);	//To distinguish
	uart3_init(9600);	//To music
	Adc_Init();	
	EXTIX_Init();		 
	TIM4_PWM_Init(36000,0);	//BIO 2Khz.
	TIM_SetCompare4(TIM4,36000);	//BIO voltage
	TIM_SetCompare3(TIM4,0);	//BIO output
	TIM3_PWM_Init(102,0);	//RF 700Khz.
	TIM1_PWM_Init((u16)(72000/26.3), 0);	//Scrubber Init 26.3kHz
	TIM2_Int_Init(999,70);	//1ms
	
#ifndef DEBUG
//prescaler 256,reload 625,over time 4s
	IWDG_Init(IWDG_Prescaler_256,625);  
#endif
			
	while(1)
	{

	}
