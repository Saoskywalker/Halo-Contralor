//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//外部中断 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/01  
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  

#include "AppLib.h" 

//外部中断0服务程序
//初始化PA0,PA13,PA15为中断输入.
void EXTIX_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	//int pin input
	//使能IO&复用功能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

  //中断线以及中断初始化配置   下降沿触发	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级3， 最低
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级3, 最低
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//外部中断3
void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line3); //清除线路挂起位
	}
}

//外部中断5~9
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line6); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line7); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line8); //清除线路挂起位
	}
}

//外部中断10~15服务程序 
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) //ear
	{
		// DEBUG_LED = ~DEBUG_LED;
		HeadMotorStop();
		EXTI_ClearITPendingBit(EXTI_Line12); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line13) != RESET) //left
	{
		// DEBUG_LED = ~DEBUG_LED;
		MiddleMotorStop();
		EXTI_ClearITPendingBit(EXTI_Line13); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line14) != RESET) //right
	{
		// DEBUG_LED = ~DEBUG_LED;
		MiddleMotorStop();
		EXTI_ClearITPendingBit(EXTI_Line14); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line15); //清除线路挂起位
	}
}
 
 
