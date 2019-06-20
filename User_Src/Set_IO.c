#include "Set_IO.h"

//Init not peripheral IO
void IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|
						RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	 
	
	//OUTPUT 
	//Disable JTAG, release the pin
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //RELEASE PB 3 4 15	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12| \
									GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|
					GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6| \
									GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
					GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);

	//INPUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                       
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                       
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|\
//									GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|G \
//									PIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15;		 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
//	GPIO_ResetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
//							GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15); 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_15;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_15);

	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
//	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后， PC14 PC15 才可以当普通IO用。
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
//	BKP_ITConfig(DISABLE);
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	// GPIO_Init(GPIOC, &GPIO_InitStructure);		
	// GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

