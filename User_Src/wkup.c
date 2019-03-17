#include "wkup.h"
#include "delay.h"
#include "UserBaseLib.h"

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

//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
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
			if(t>=70)		//���³���3����
			{
				return 1; 	//����3s������
			}
		}
		else 
		{ 
			return 0; //���²���3��
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

//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	//ʹ��GPIOA�͸��ù���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  //��ʼ��IO

	//ʹ���ⲿ�жϷ�ʽ
	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //�ж���0����GPIOA.0

	// EXTI_InitStructure.EXTI_Line = EXTI_Line0;			   //���ð������е��ⲿ��·
	// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
	// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	// EXTI_Init(&EXTI_InitStructure); // ��ʼ���ⲿ�ж�

	// NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		  //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3��
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3��
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ʹ���ⲿ�ж�ͨ��
	// //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	// NVIC_Init(&NVIC_InitStructure);							  

	// if(RestoreFlag()==2) //������λ��Ĵ���
	// 	Sys_Standby();
	if (Check_WKUP() == 0)
		Sys_Standby(); //���ǿ���,�������ģʽ
}
