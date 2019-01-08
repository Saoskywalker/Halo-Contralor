/**********************************************************************
Name: Space
Version: v0.5 Beta
Editor: Aysi
Function: Scrubber, Ultrasound, Two BIO, Cold&Heat, use DW L Display, O2 Mask, Pen, clean pen
History:2018.1.13: Debug completed
2018.1.17: Repair some bug, and add the sound when scrubber adjust function works
2018.1.19: Cancel the sound of adjust working, change ScrubberPWM(), Scrubber PWM switch becomes TIM1OUTPUT
2018.3.9: add fan2, change scrubber frequency range to 20.1~30.0, Voltage Adj change, Scrubber bug repaired
flash way changed, intensity changed, screenCheck(); advance work
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
	NVIC_Configuration(); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	//Init Systick timer
	IO_Init();
	uart1_init(115200);	//To PC
	uart2_init(115200);	//To Dw display
	Adc_Init();	
//	EXTIX_Init();		 
	TIM4_PWM_Init(36000,0);	//BIO 2Khz.
	TIM_SetCompare4(TIM4,36000);	//BIO voltage
	TIM_SetCompare3(TIM4,0);	//BIO output
	TIM3_PWM_Init(102,0);	//RF 700Khz.
	TIM1_PWM_Init((u16)(72000/26.3), 0);	//Scrubber Init 26.3kHz
//	screenCheck();	//DW power on check
	dwSetBL(0x20);	//change display light
	TIM2_Int_Init(999,70);	//1ms

	/*scrubber frequency init*/
	STMFLASH_Read(FLASH_SAVE_ADDR, &ScrubberFrequency, 1);	
	if(ScrubberFrequency<201||ScrubberFrequency>300)
	{
		ScrubberFrequency = 250;
		STMFLASH_Write(FLASH_SAVE_ADDR, &ScrubberFrequency, 1);
		INLINE_MUSIC_START();
	}
	
	uasrt1SendByte(0x01);
	nextPage = FUNCTION_RESTART;
	language = LANGUAGE_ENGLISH;
	dwSetColor(DW_COL_BLACK, DW_COL_WHITE);
//	dwDisPicNoL(PIC_LANGUAGE);
	
#ifndef DEBUG
//与分频数为256,重载值为625,溢出时间为4s
	IWDG_Init(IWDG_Prescaler_256,625);  
#endif
			
	while(1)
	{
		switch(nextPage)
		{
			case FUNCTION_RESTART:
			{
				PageRestartAnimation();
				break;
			}
			case FUNCTION_MAIN:
			{
				PageMain();
				break;
			}
			case FUNCTION_SCRUBBER:
			{
				PageScrubber();
				break;
			}
			case FUNCTION_SCRUBBER_ADJ:
			{
				PageScrubberAdj();
				break;
			}
			case FUNCTION_ULTRASOUND:
			{
				PageUltrasound();
				break;
			}
			case FUNCTION_O2:
			{
				PageO2();
				break;
			}
			case FUNCTION_ICE:
			{
				PageIce();
				break;
			}
			case FUNCTION_BIO1:
			{
				PageBIO1();
				break;
			}
			case FUNCTION_BIO2:
			{
				PageBIO2();
				break;
			}
			case FUNCTION_CLEAN:
			{
				PageClean();
				break;
			}
			case FUNCTION_RF:
			{
				PageRF();
				break;
			}
			case TEST:
			{
				test();
				break;
			}
			default:
			{
				break;
			}
		}
	}
}
