#include "UserBaseLib.h"

u8 RestoreFlag(void)
{
	u8 RestFlag = 0;
	
	//��λ��־��6��
	//���Ź���λ��־
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		RestFlag = 1;
	}					
	//�����λ��־
	else if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		RestFlag =  2;  							 
	}
	//�ϵ縴λ��־
	else if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		RestFlag = 3;  
	}
	RCC_ClearFlag();//�����λ״̬��־λ
	return RestFlag;
}

/******************************************************
��Cortex-M3Ȩ��ָ������һ��Ҫע������⣺��SYSRESETREQ ����Ϊ��Ч��
����λ������ִ�и�λ�����������һ����ʱ���ڴ���ʱ�ڼ䣬
��������Ȼ������Ӧ�ж�����
�����ǵı���������Ҫ�ô˴�ִ�е���Ϊֹ����Ҫ�����κ����������ˡ�
���ԣ�����ڷ�����λ����ǰ��
�Ȱ�FAULTMASK ��λ��core_cm3.c �ļ���Ҳֱ���ṩ�˸ú���
*********************************************************/
 
/*****************
 * �����λ
 * ********************/
void SystemReset(void)
{
//Ŀ��:��ֹ�ж�ִ��   ʹ��FAULTMASK���ȼ�����ִ�е����ȼ�����Ϊ��1.
  __set_FAULTMASK(1); 
  //��ʼ��λ
  NVIC_SystemReset(); 
}

// //ϵͳ��λ   
// void Sys_Soft_Reset(void)
// {   
// 	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
// } 		 
