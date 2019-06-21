#include "AppLib.h"

////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////
	 
void Sys_Standby(void)
{  
	//ʹ��PWR����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	
	PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 		 
}

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	Sys_Standby();
}

//����ͣ��ģʽ
void Sys_Enter_Stop(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��

	//���û�������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                       
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//PC1

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
	SystemInit(); //ͣ����������, ��ʼ��ʱ��
}

//���WKUP�ŵ��ź�
//����ֵ1:��������2s����
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
	u8 t=0;	//��¼���µ�ʱ��

	while(1)
	{
		if(WKUP_KD)
		{
			t++;			//�Ѿ������� 
			delay_ms(30);
			if(t>=50)		//���³���1.5����
			{
				return 1; 	//����2s������
			}
		}
		else 
		{ 
			return 0; //���²���2��
		}
	}
} 

//�ж�,��⵽PA0�ŵ�һ��������.	  
//�ж���0���ϵ��жϼ��
void EXTI0_IRQHandler(void)
{ 		    		    				     		    
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if (Check_WKUP()) //����
		{
			// SystemReset(); //�������رտ��Ź�, �ٴ���
			Sys_Enter_Standby();
		}
		EXTI_ClearITPendingBit(EXTI_Line0); //�����·����λ
	}
}

//�ⲿ�ж�1
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line1); //�����·����λ
	}
}

//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	//ʹ��GPIO�͸��ù���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                       
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//PC1

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		  //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);		

	if(RestoreFlag()==2) //��λ��ִ��, ������λ�����͹���ģʽ,Ϊ�رտ��Ź�
		Sys_Enter_Stop();
	// if(RTCAlarm==0) //�Ƿ����ӻ���
	// {
	// 	if (Check_WKUP() == 0)
	// 		Sys_Standby(); //���ǿ���,�������ģʽ
	// }
}
