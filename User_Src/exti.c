#include "AppLib.h"
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
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_12| \
									GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

  //中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级3,最低
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3, 最低
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//外部中断1
void EXTI1_IRQHandler(void) //voice busy
{
	static u8 i = 1;
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		// printf("%d,%d\n", i, VoiceTable[i]);
		// if(VoiceTable[i]!=0)
		// {
		// 	MusicStart(VoiceTable[i]);
		// 	i++;
		// }
		// else
		// {
		// 	i = 1;
		// 	VoiceTable[1] = 0;
		// }
		
		EXTI_ClearITPendingBit(EXTI_Line1); //清除线路挂起位
	}
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
		void MiddleMotorStop(void);
		EXTI_ClearITPendingBit(EXTI_Line13); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line14) != RESET) //right
	{
		// DEBUG_LED = ~DEBUG_LED;
		void MiddleMotorStop(void);
		EXTI_ClearITPendingBit(EXTI_Line14); //清除线路挂起位
	}

	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line15); //清除线路挂起位
	}
}
 
 
