#include "UserBaseLib.h"

u8 RestoreFlag(void)
{
	u8 RestFlag = 0;
	
	//复位标志共6种
	//看门狗复位标志
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		RestFlag = 1;
	}					
	//软件复位标志
	else if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		RestFlag =  2;  							 
	}
	//上电复位标志
	else if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		RestFlag = 3;  
	}
	RCC_ClearFlag();//清楚复位状态标志位
	return RestFlag;
}

/******************************************************
在Cortex-M3权威指南中有一个要注意的问题：从SYSRESETREQ 被置为有效，
到复位发生器执行复位命令，往往会有一个延时。在此延时期间，
处理器仍然可以响应中断请求。
但我们的本意往往是要让此次执行到此为止，不要再做任何其它事情了。
所以，最好在发出复位请求前，
先把FAULTMASK 置位。core_cm3.c 文件里也直接提供了该函数
*********************************************************/
 
/*****************
 * 软件复位
 * ********************/
void SystemReset(void)
{
//目的:阻止中断执行   使能FAULTMASK优先级：把执行的优先级提升为－1.
  __set_FAULTMASK(1); 
  //开始复位
  NVIC_SystemReset(); 
}

// //系统软复位   
// void Sys_Soft_Reset(void)
// {   
// 	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
// } 		 
