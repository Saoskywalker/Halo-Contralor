//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�ⲿ�ж� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/01  
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  

#include "AppLib.h" 

//�ⲿ�ж�0�������
//��ʼ��PA0,PA13,PA15Ϊ�ж�����.
void EXTIX_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	//int pin input
	//ʹ��IO&���ù���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

  //�ж����Լ��жϳ�ʼ������   �½��ش���	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�3�� ���
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//�����ȼ�3, ���
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//�ⲿ�ж�3
void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line3); //�����·����λ
	}
}

//�ⲿ�ж�5~9
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line6); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line7); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line8); //�����·����λ
	}
}

//�ⲿ�ж�10~15������� 
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) //ear
	{
		// DEBUG_LED = ~DEBUG_LED;
		HeadMotorStop();
		EXTI_ClearITPendingBit(EXTI_Line12); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line13) != RESET) //left
	{
		// DEBUG_LED = ~DEBUG_LED;
		MiddleMotorStop();
		EXTI_ClearITPendingBit(EXTI_Line13); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line14) != RESET) //right
	{
		// DEBUG_LED = ~DEBUG_LED;
		MiddleMotorStop();
		EXTI_ClearITPendingBit(EXTI_Line14); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line15); //�����·����λ
	}
}
 
 
