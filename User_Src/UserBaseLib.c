#include "UserBaseLib.h"

u8 RestoreFlag(void)
{
	u8 RestFlag = 0;
	
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
