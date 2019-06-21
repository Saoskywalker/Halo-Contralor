#include "AppLib.h"

////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//待机唤醒 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////
	 
void Sys_Standby(void)
{  
	//使能PWR外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	
	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能
	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 		 
}

//系统进入待机模式
void Sys_Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	Sys_Standby();
}

//进入停机模式
void Sys_Enter_Stop(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口

	//设置唤醒引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                       
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//PC1

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
	SystemInit(); //停机继续工作, 初始化时钟
}

//检测WKUP脚的信号
//返回值1:连续按下2s以上
//      0:错误的触发	
u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间

	while(1)
	{
		if(WKUP_KD)
		{
			t++;			//已经按下了 
			delay_ms(30);
			if(t>=50)		//按下超过1.5秒钟
			{
				return 1; 	//按下2s以上了
			}
		}
		else 
		{ 
			return 0; //按下不足2秒
		}
	}
} 

//中断,检测到PA0脚的一个上升沿.	  
//中断线0线上的中断检测
void EXTI0_IRQHandler(void)
{ 		    		    				     		    
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if (Check_WKUP()) //检测键
		{
			// SystemReset(); //先重启关闭看门狗, 再待机
			Sys_Enter_Standby();
		}
		EXTI_ClearITPendingBit(EXTI_Line0); //清除线路挂起位
	}
}

//外部中断1
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line1); //清除线路挂起位
	}
}

//PA0 WKUP唤醒初始化
void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	//使能GPIO和复用功能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                       
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//PC1

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		  //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);		

	if(RestoreFlag()==2) //软复位后执行, 继续软复位后进入低功耗模式,为关闭看门狗
		Sys_Enter_Stop();
	// if(RTCAlarm==0) //是否闹钟唤醒
	// {
	// 	if (Check_WKUP() == 0)
	// 		Sys_Standby(); //不是开机,进入待机模式
	// }
}
