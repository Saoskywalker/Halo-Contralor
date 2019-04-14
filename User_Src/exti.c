#include "AppLib.h"
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
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_12| \
									GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

  //�ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);

  	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�3,���
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3, ���
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//�ⲿ�ж�1
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
		
		EXTI_ClearITPendingBit(EXTI_Line1); //�����·����λ
	}
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
		void MiddleMotorStop(void);
		EXTI_ClearITPendingBit(EXTI_Line13); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line14) != RESET) //right
	{
		// DEBUG_LED = ~DEBUG_LED;
		void MiddleMotorStop(void);
		EXTI_ClearITPendingBit(EXTI_Line14); //�����·����λ
	}

	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		// DEBUG_LED = ~DEBUG_LED;
		EXTI_ClearITPendingBit(EXTI_Line15); //�����·����λ
	}
}
 
 
