#include "AppLib.h"

//#define DEBUG_WDG
#define SCR_T_ON 0.2	//Scrubber PWM(Timer1) compare value
#define FRAME_HEAD 0xAA
#define FRAME_END 0x55

//const u16 CoolIntensity[] = {0,1700,1800,1900,2000,2175,2275,2375,2475,2550,2625};	//2.2 2.4 2.6 2.8 3.0 3.2 3.4 3.6 3.8 4.0 
u8 Flag1ms = 0;
u8 nextPage = 0;
u8 menuExit = 0;
u16 WorkTime = 1800;
u8 Flag1s = 0, Flag50ms = 0;
u16 ScrubberFrequency = 250;
long int HallCount = 0;

u8 ScrubberMode = 0, ScrubberIntensity = 0;
u8 O2Mode = 0, O2Intensity = 0;
u8 RFMode = 0, RFIntensity = 0;
u8 ColdMode = 0, ColdIntensity = 0;
u8 BIO1Mode = 0, BIO1Intensity = 0;
u8 BIO2Mode = 0, BIO2Intensity = 0;
u8 CleanMode = 0, CleanIntensity = 0;
u8 UltrasoundMode = 0, UltrasoundIntensity = 0;
u8 ScrubberWorkFlag = 0, O2WorkFlag = 0, RFWorkFlag = 0, BIO1WorkFlag = 0, BIO2WorkFlag = 0, CleanWorkFlag = 0, UltrasoundWorkFlag = 0, ColdWorkFlag = 0;
const float ScrubberPWMTable[] = {0, 0.1, 0.2, 0.3, 0.4};
u16 ScrubberPWMIntensity = 0;
const u16 BIO1IntensityTable[] = {36000, 20000, 10000, 5000, 0, 0};//{36000, 30000, 29000, 28000, 27000, 26000};	//{36000, 20000, 15000, 10000, 5000, 0};
const u16 BIO2IntensityTable[] = {36000, 29500, 28500, 27500, 26500, 25000};	//{36000, 20000, 15000, 10000, 5000, 0};

//Button press effect
const Button btnClean = {PIC_EFFECT, 154,387,278,463};
const Button btnO2 = {PIC_EFFECT, 10,387,134,463};
const Button btnScrubber = {PIC_EFFECT, 10,294,134,370};
const Button btnUltrasound = {PIC_EFFECT, 10,200,134,277};
const Button btnRF = {PIC_EFFECT, 154,200,278,277};
const Button btnIce = {PIC_EFFECT, 154,294,278,370};
const Button btnBIO1 = {PIC_EFFECT, 10,106,134,183};
const Button btnBIO2 = {PIC_EFFECT, 154,106,278,183};
const Button btnTimeUp = {PIC_EFFECT, 29,32,77,80};
const Button btnTimeDown = {PIC_EFFECT, 211,32,260,80};
const Button btnIntensityUp = {PIC_EFFECT, 459,145,509,192};
const Button btnIntensityDown = {PIC_EFFECT, 644,145,694,192};
const Button btnModeUp = {PIC_EFFECT, 459,253,509,300};
const Button btnModeDown = {PIC_EFFECT, 644,250,694,297};
const Button btnStart = {PIC_EFFECT, 592,338,750,390};
const Button btnScrubberAdj = {PIC_EFFECT,758,441,799,479};
const Button btnMin = {PIC_EFFECT,720,9,783,64};

//Botton free effect
const Button btnTimeDown_O = {PIC_EFFECT, 211,32,260,80};
const Button btnTimeUp_O = {PIC_EFFECT, 29,32,77,80};
const Button btnIntensityUp_O = {PIC_ORIG, 80,302,146,365};
const Button btnIntensityDown_O = {PIC_ORIG, 77,500,144,567};
const Button btnModeA_O = {PIC_ORIG, 330,274,408,343};
const Button btnModeB_O = {PIC_ORIG, 330,360,408,426};
const Button btnModeC_O = {PIC_ORIG, 330,450,408,520};
const Button btnStart_O = {PIC_ORIG, 150,520,329,622};


void ScrubberPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_SCRUBBER;
}

void O2Pres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_O2;
}

void RFPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_RF;
}

void CleanPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_CLEAN;
}

void UltrasoundPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_ULTRASOUND;
}

void IcePres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_ICE;
}

void BIO1Pres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_BIO1;	
}

void BIO2Pres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_BIO2;
}

void IntensityUpPres(void)
{
	INLINE_MUSIC_BUTTON();
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberIntensity<4)
				ScrubberIntensity++;
			dwDisNum(DW_SIZE_48, 550, 151, ScrubberIntensity);
			ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
			TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			if(ScrubberFrequency<300)
			{
				ScrubberFrequency++;
				dwDisNum(DW_SIZE_48, 525,    151,ScrubberFrequency/100);
				dwDisNum(DW_SIZE_48, 525+24, 151,ScrubberFrequency/10%10);
				dwDisChar(DW_SIZE_48,525+48, 151,'.');
				dwDisNum(DW_SIZE_48, 525+72, 151,ScrubberFrequency%10);
				TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
				ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
				TIM_SetCompare1(TIM1, ScrubberPWMIntensity);
			}
			break;
		}
		case FUNCTION_BIO1:
		{
			if(BIO1Intensity<4)
				BIO1Intensity++;
			dwDisNum(DW_SIZE_48, 550, 151, BIO1Intensity);
			TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
			break;
		}
		case FUNCTION_BIO2:
		{
			if(BIO2Intensity<4)
				BIO2Intensity++;
			dwDisNum(DW_SIZE_48, 550, 151, BIO2Intensity);
			TIM_SetCompare4(TIM4, BIO2IntensityTable[BIO2Intensity]);
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			if(UltrasoundIntensity<4)
				UltrasoundIntensity++;
			dwDisNum(DW_SIZE_48, 550, 151, UltrasoundIntensity);
			break;
		}
		case FUNCTION_RF:
		{
			if(RFIntensity<4)
				RFIntensity++;
			dwDisNum(DW_SIZE_48, 550, 151, RFIntensity);
			break;
		}
		default: {break;}
	}	
}

void ScrubberAdjPres(void)
{
	INLINE_MUSIC_STOP();
	nextPage = FUNCTION_SCRUBBER_ADJ;
}

void IntensityUpFree(void)
{
	dwDisButton(&btnIntensityUp_O, 1, btnIntensityUp_O.xs, btnIntensityUp_O.ys);
}

void IntensityDownPres(void)
{
	INLINE_MUSIC_BUTTON();	
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberIntensity>0)
				ScrubberIntensity--;
			dwDisNum(DW_SIZE_48, 550, 151, ScrubberIntensity);
			ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
			TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
			break;
		}
		case FUNCTION_SCRUBBER_ADJ:
		{
			if(ScrubberFrequency>201)
			{
				ScrubberFrequency--;
				dwDisNum(DW_SIZE_48, 525,    151,ScrubberFrequency/100);
				dwDisNum(DW_SIZE_48, 525+24, 151,ScrubberFrequency/10%10);
				dwDisChar(DW_SIZE_48,525+48, 151,'.');
				dwDisNum(DW_SIZE_48, 525+72, 151,ScrubberFrequency%10);
				TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
				ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
				TIM_SetCompare1(TIM1, ScrubberPWMIntensity);
			}
			break;
		}
		case FUNCTION_BIO1:
		{
			if(BIO1Intensity>0)
				BIO1Intensity--;
			dwDisNum(DW_SIZE_48, 550, 151, BIO1Intensity);
			TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
			break;
		}
		case FUNCTION_BIO2:
		{
			if(BIO2Intensity>0)
				BIO2Intensity--;
			dwDisNum(DW_SIZE_48, 550, 151, BIO2Intensity);
			TIM_SetCompare4(TIM4, BIO2IntensityTable[BIO2Intensity]);
			break;
		}
		case FUNCTION_ULTRASOUND:
		{
			if(UltrasoundIntensity>0)
				UltrasoundIntensity--;
			dwDisNum(DW_SIZE_48, 550, 151, UltrasoundIntensity);
			break;
		}
		case FUNCTION_RF:
		{
			if(RFIntensity>0)
				RFIntensity--;
			dwDisNum(DW_SIZE_48, 550, 151, RFIntensity);
			break;
		}
		default: {break;}
	}	
//	dwDisButton(&btnIntensityDown, 1, btnIntensityDown.xs, btnIntensityDown.ys);
}

void IntensityDownFree(void)
{
	dwDisButton(&btnIntensityDown_O, 1, btnIntensityDown_O.xs, btnIntensityDown_O.ys);
}

void TimeUpPres(void)
{
	INLINE_MUSIC_BUTTON();
//	dwDisButton(&btnTimeUp, 1, btnTimeUp.xs, btnTimeUp.ys);
	if(WorkTime<3600)
	{
		WorkTime += 60;
		funDisTime(WorkTime);
	}	
}

void TimeUpFree(void)
{
	dwDisButton(&btnTimeUp_O, 1, btnTimeUp_O.xs, btnTimeUp_O.ys);
}

void TimeDownPres(void)
{
	INLINE_MUSIC_BUTTON();
//	dwDisButton(&btnTimeDown, 1, btnTimeDown.xs, btnTimeDown.ys);
	if(WorkTime>60)
	{
		WorkTime -= 60;
		funDisTime(WorkTime);
	}
}

void TimeDownFree(void)
{
	dwDisButton(&btnTimeDown_O, 1, btnTimeDown_O.xs, btnTimeDown_O.ys);
}

void ModeUpPres(void)
{
	INLINE_MUSIC_BUTTON();
//	dwDisButton(&btnModeUp, 1, btnModeUp.xs, btnModeUp.ys);
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberMode<2)
				ScrubberMode++;
			dwDisChar(DW_SIZE_48, 550, 253, ScrubberMode+'A');
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1Mode<2)
				BIO1Mode++;
			dwDisChar(DW_SIZE_48, 550, 253, BIO1Mode+'A');
			break;
		}
		case FUNCTION_BIO2: 
		{
			if(BIO2Mode<2)
				BIO2Mode++;
			dwDisChar(DW_SIZE_48, 550, 253, BIO2Mode+'A');
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundMode<2)
				UltrasoundMode++;
			dwDisChar(DW_SIZE_48, 550, 253, UltrasoundMode+'A');
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFMode<2)
				RFMode++;
			dwDisChar(DW_SIZE_48, 550, 253, RFMode+'A');
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdMode<1)
				ColdMode++;
			dwDisChar(DW_SIZE_48, 550, 253, ColdMode+'A');
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2Mode<2)
				O2Mode++;
			dwDisChar(DW_SIZE_48, 550, 253, O2Mode+'A');
			break;
		}
		case FUNCTION_CLEAN: 
		{
			if(CleanMode<2)
				CleanMode++;
			dwDisChar(DW_SIZE_48, 550, 253, CleanMode+'A');
			break;
		}
		default: {break;}
	}
}

void ModeDownPres(void)
{
	INLINE_MUSIC_BUTTON();
//	dwDisButton(&btnModeDown, 1, btnModeDown.xs, btnModeDown.ys);
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberMode>0)
				ScrubberMode--;
			dwDisChar(DW_SIZE_48, 550, 253, ScrubberMode+'A');
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1Mode>0)
				BIO1Mode--;
			dwDisChar(DW_SIZE_48, 550, 253, BIO1Mode+'A');
			break;
		}
		case FUNCTION_BIO2: 
		{
			if(BIO2Mode>0)
				BIO2Mode--;
			dwDisChar(DW_SIZE_48, 550, 253, BIO2Mode+'A');
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundMode>0)
				UltrasoundMode--;
			dwDisChar(DW_SIZE_48, 550, 253, UltrasoundMode+'A');
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFMode>0)
				RFMode--;
			dwDisChar(DW_SIZE_48, 550, 253, RFMode+'A');
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdMode>0)
				ColdMode--;
			dwDisChar(DW_SIZE_48, 550, 253, ColdMode+'A');
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2Mode>0)
				O2Mode--;
			dwDisChar(DW_SIZE_48, 550, 253, O2Mode+'A');
			break;
		}
		case FUNCTION_CLEAN: 
		{
			if(CleanMode>0)
				CleanMode--;
			dwDisChar(DW_SIZE_48, 550, 253, CleanMode+'A');
			break;
		}
		default: {break;}
	}
}

void StartPres(void)
{
	INLINE_MUSIC_START();
	switch(nextPage)
	{
		case FUNCTION_SCRUBBER: 
		{
			if(ScrubberWorkFlag)
			{				
				ScrubberWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				ScrubberWorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_BIO1: 
		{
			if(BIO1WorkFlag)
			{				
				BIO1WorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				BIO1WorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_BIO2: 
		{
			if(BIO2WorkFlag)
			{				
				BIO2WorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				BIO2WorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_ULTRASOUND: 
		{
			if(UltrasoundWorkFlag)
			{				
				UltrasoundWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				UltrasoundWorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_RF: 
		{
			if(RFWorkFlag)
			{				
				RFWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				RFWorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_ICE: 
		{
			if(ColdWorkFlag)
			{				
				ColdWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				ColdWorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_O2: 
		{
			if(O2WorkFlag)
			{				
				O2WorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				O2WorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		case FUNCTION_CLEAN: 
		{
			if(CleanWorkFlag)
			{				
				CleanWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			else
			{
				CleanWorkFlag = 1;
				dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			break;
		}
		default: {break;}
	}
}

void EnterPres(void)
{
	menuExit = 1;
	nextPage = FUNCTION_MAIN;
	INLINE_MUSIC_BUTTON();
//	dwSetLanguage(LANGUAGE_ENGLISH);
}

void MinPres(void)
{
	INLINE_MUSIC_BUTTON();
	nextPage = FUNCTION_MAIN;
}

void WorkTimeDeal(void)
{
	if(ScrubberWorkFlag||O2WorkFlag||RFWorkFlag||BIO1WorkFlag||BIO2WorkFlag||CleanWorkFlag||UltrasoundWorkFlag||ColdWorkFlag)
	{
		if(Flag1s)
		{
			Flag1s = 0;
			if(WorkTime>0)
			{
				WorkTime--;
			}
			else
			{
				INLINE_MUSIC_STOP();
				WorkTime = 1800;
				ScrubberWorkFlag = 0;
				O2WorkFlag = 0;
				RFWorkFlag = 0;
				BIO1WorkFlag = 0;
		    BIO2WorkFlag = 0;
				CleanWorkFlag = 0;
				UltrasoundWorkFlag = 0; 
				ColdWorkFlag = 0;
				dwCutPic(PIC_SCRUBBER, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
			}
			funDisTime(WorkTime);
		}
	}
}

//Init not peripheral IO
void IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //Disable JTAG, release the pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|\
								  GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|\
								  GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6|GPIO_Pin_15);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15;	//SOUND		 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
//	GPIO_ResetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15); 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_15;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_15);

//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);		
//	//GPIO_ResetBits(GPIOC, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
//	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后， PC14 PC15 才可以当普通IO用。
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
//	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
//	BKP_ITConfig(DISABLE);
}

//icon dispaly
void IconDisplay(void)
{
	if(ScrubberWorkFlag)
		dwCutPicL(PIC_SCRUBBER, btnScrubber.xs, btnScrubber.ys, btnScrubber.xe, btnScrubber.ye, btnScrubber.xs, btnScrubber.ys);
	if(O2WorkFlag)
		dwCutPicL(PIC_O2, btnO2.xs, btnO2.ys, btnO2.xe, btnO2.ye, btnO2.xs, btnO2.ys);
	if(RFWorkFlag)
		dwCutPicL(PIC_RF, btnRF.xs, btnRF.ys, btnRF.xe, btnRF.ye, btnRF.xs, btnRF.ys);
	if(BIO1WorkFlag)
		dwCutPicL(PIC_BIO1, btnBIO1.xs, btnBIO1.ys, btnBIO1.xe, btnBIO1.ye, btnBIO1.xs, btnBIO1.ys);
	if(BIO2WorkFlag)
		dwCutPicL(PIC_BIO2, btnBIO2.xs, btnBIO2.ys, btnBIO2.xe, btnBIO2.ye, btnBIO2.xs, btnBIO2.ys);
	if(CleanWorkFlag)
		dwCutPicL(PIC_CLEAN, btnClean.xs, btnClean.ys, btnClean.xe, btnClean.ye, btnClean.xs, btnClean.ys);
	if(UltrasoundWorkFlag)
		dwCutPicL(PIC_ULTRASOUND, btnUltrasound.xs, btnUltrasound.ys, btnUltrasound.xe, btnUltrasound.ye, btnUltrasound.xs, btnUltrasound.ys);
	if(ColdWorkFlag)
		dwCutPicL(PIC_ICE, btnIce.xs, btnIce.ys, btnIce.xe, btnIce.ye, btnIce.xs, btnIce.ys);
}

void test(void)
{	
	long int T = 0, RunT = 0;
	u8 Cnt = 0;
	DisCharSet.FrontColor = DW_COL_BLACK;
	DisCharSet.BackColor = DW_COL_WHITE;
	DisCharSet.Size = DW_SIZE_48;
	dwDisPicNoL(3);
	while(1)
	{
		if(Flag50ms)
		{
			Flag50ms = 0;
			if(++Cnt>=20)
			{
				Cnt = 0;
				RunT++;
			
			printf("RunTime:%d ", RunT);
			//Display count
//			DisCharSet.CursorX = 37;
//			DisCharSet.CursorY = 87;
//			DisNumber(HallCount, 10, 0, Dis_Dec);
			printf("Count:%d ", HallCount);
			
			//Display speed
//			DisCharSet.CursorX = 37;
//			DisCharSet.CursorY = 220;			
//			DisNumber((HallCount-T)/20, 4, 0, Dis_Dec);
			printf("Speed:%d\n", HallCount-T);
			T = HallCount;
		
			//Display AD
//			DisCharSet.CursorX = 37;
//			DisCharSet.CursorY = 361;
//			DisNumber(Get_Adc_Average(ADC_Channel_1,6), 4, 0, Dis_Dec);		
			}
		}
	}
}

//restart animation
#define RESTART_ANIMATION_SPEED 70

const Button btnEnter  = {PIC_MAIN, 0, 0, 800, 480};

void PageRestartAnimation(void)
{
	u8 i = 0;
	u16 x = 0;
	
	if(RestoreFlag()==1)	//Check restore reason
	{
		BeeMod = 1;
		BeeTime = 1;
	}
	
	/*Animation program*/
	dwDisPicNoL(PIC_START_ANIMATION);
	delay_ms(500);
	for(i = 0; i<32; i++, x += 25)
	{		
		dwCutPic(PIC_START_ANIMATION+1, x, 230, 24+x, 250, x, 230);
		delay_ms(RESTART_ANIMATION_SPEED);
	}
	delay_ms(100);
	dwCutPic(PIC_START_ANIMATION+1, 218, 178, 602, 228, 218, 178);
	
	menuExit = 0;
	dwCancelKey();
	dwListenKey(EnterPres, 0, &btnEnter);
//	dwWaitRelease();
	while(!menuExit)
	{
		dwHandler();
	}
	
	dwWaitRelease();
	dwHandler();
}
////////////////////////////////

//Main page
void PageMain(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_MAIN);
	IconDisplay();
	funDisTime(WorkTime);
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_MAIN)
		{
			menuExit = 1;
		}
		WorkTimeDeal();
	}
	
	dwWaitRelease();
	dwHandler();
}
///////////////////////////////

//Scrubber function
void PageScrubber(void)
{
	u8 i = 0;
	menuExit = 0;
	dwDisPicWithL(PIC_SCRUBBER);
	dwCutPicL(PIC_SCRUBBER, btnScrubber.xs, btnScrubber.ys, btnScrubber.xe, btnScrubber.ye, btnScrubber.xs, btnScrubber.ys);
	IconDisplay();
	dwDisNum(DW_SIZE_48, 550, 151, ScrubberIntensity);
	dwDisChar(DW_SIZE_48, 550, 253, ScrubberMode+'A');
	funDisTime(WorkTime);
	if(ScrubberWorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
	ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
	TIM_SetCompare1(TIM1,ScrubberPWMIntensity);
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(ScrubberAdjPres, 0, &btnScrubberAdj);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_SCRUBBER)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				ScrubberIntensity = 0;
				ScrubberMode = 0;
				ScrubberWorkFlag = 0;
			}
		}
		WorkTimeDeal();		
		if(Get_Adc_Average(ADC_Channel_1,6)>=50)
		{
			if(++i>=10)
			{
				i = 10;
				INLINE_MUSIC_BUTTON();
			}
		}
		else
		{
			if(i>0)
			if(--i==0)
			{
				i = 0;
			}
		}
	}

	dwWaitRelease();
	dwHandler();
}

//ScrubberPWM
const u16 ScrubberModPeriod[] = {7000, 200, 12};	//{100, 200, 500, 3500};	//{12, 200, 7000, 3500}
const u16 ScrubberModCompare[] = {6000, 40, 3};	//{100, 100, 250, 1200};	//{3, 40, 6000, 1200} 
void ScrubberPWM(u8 i, u8 Work)
{
	static u16 ScrubberTimeCnt = 0; //ScrubberTimeCnt2 = 0;
	static u8 ScrubberModRenew = 0;
	
	if(Work)
	{
		if(++ScrubberTimeCnt>=ScrubberModPeriod[ScrubberModRenew])
		{
			ScrubberTimeCnt = 0;
			ScrubberModRenew = i;
		}
		if(ScrubberTimeCnt<ScrubberModCompare[ScrubberModRenew])
		{
			TIM_CtrlPWMOutputs(TIM1,ENABLE);
//			if(++ScrubberTimeCnt2>=12)
//			{
//				ScrubberTimeCnt2 = 0;
////			POWER_ON_PIN = 1;
//			}
//			if(ScrubberTimeCnt2<3)
//			{
//				TIM_CtrlPWMOutputs(TIM1,ENABLE);
////			POWER_ON_PIN = 1;
//			}
//			else
//			{
//				TIM_CtrlPWMOutputs(TIM1,DISABLE);
//			}
		}
		else
		{
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	
//			POWER_ON_PIN = 0;		
//			ScrubberTimeCnt2 = 0;
		}			
	}
	else
	{
//		ScrubberTimeCnt2 = 0;
		ScrubberTimeCnt = 0;
//		POWER_ON_PIN = 0;	
		TIM_CtrlPWMOutputs(TIM1,DISABLE);	
	}
}

//ScrubberAdj
void PageScrubberAdj(void)
{
	ScrubberWorkFlag = 1;
	ScrubberMode = 0;
	ScrubberIntensity = 4;
	menuExit = 0;
	
	STMFLASH_Read(FLASH_SAVE_ADDR, &ScrubberFrequency, 1);	
	if(ScrubberFrequency<201||ScrubberFrequency>300)
	{
		ScrubberFrequency = 250;
		STMFLASH_Write(FLASH_SAVE_ADDR, &ScrubberFrequency, 1);
	}	
	dwDisPicWithL(PIC_SCRUBBER_ADJ);	
	dwDisNum(DW_SIZE_48, 525,    151,ScrubberFrequency/100);
	dwDisNum(DW_SIZE_48, 525+24, 151,ScrubberFrequency/10%10);
	dwDisChar(DW_SIZE_48,525+48, 151,'.');
	dwDisNum(DW_SIZE_48, 525+72, 151,ScrubberFrequency%10);
	
	TIM_SetAutoreload(TIM1, (u16)(720000/ScrubberFrequency));
	ScrubberPWMIntensity = 720000/ScrubberFrequency*ScrubberPWMTable[ScrubberIntensity];
	TIM_SetCompare1(TIM1,ScrubberPWMIntensity);	

	dwCancelKey();
	dwListenKey(MinPres, 0, &btnMin);
	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_SCRUBBER_ADJ)
		{
			menuExit = 1;
			ScrubberWorkFlag = 0;
			ScrubberMode = 0;
			ScrubberIntensity = 0;
			STMFLASH_Write(FLASH_SAVE_ADDR, &ScrubberFrequency, 1);
		}
	}
	
	dwWaitRelease();
	dwHandler();
}
/////////////////////////////////

//Ultrasound function
void PageUltrasound(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_ULTRASOUND);
	dwCutPicL(PIC_ULTRASOUND, btnUltrasound.xs, btnUltrasound.ys, btnUltrasound.xe, btnUltrasound.ye, btnUltrasound.xs, btnUltrasound.ys);
	IconDisplay();
	dwDisNum(DW_SIZE_48, 550, 151, UltrasoundIntensity);
	dwDisChar(DW_SIZE_48, 550, 253, UltrasoundMode+'A');
	funDisTime(WorkTime);
	if(UltrasoundWorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_ULTRASOUND)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				UltrasoundIntensity = 0;
				UltrasoundMode = 0;
				UltrasoundWorkFlag = 0;
			}
		}
		WorkTimeDeal();		
	}
	
	dwWaitRelease();
	dwHandler();
}

//UltrasoundPWM
const u8 UltrasoundIntensityTable[] = {0, 2, 3, 4, 5, 6};
const u16 UltrasoundModPeriod[] = {1000, 510, 1020};
const u16 UltrasoundModCompare[] = {1000, 252, 504};
void UltrasoundPWM(u8 i, u8 Work)
{
	static u16 UltrasoundTimeCnt = 0; 
	static u16 UltrasoundTimeCnt2 = 0;
	static u8 UltrasoundModRenew = 0, UltrasoundModRenew2 = 0;
				
	if(Work)
	{
		//PWM1
		if(++UltrasoundTimeCnt>=UltrasoundModPeriod[UltrasoundModRenew])
		{
			UltrasoundTimeCnt = 0;
			UltrasoundModRenew = i;
		}
		if(UltrasoundTimeCnt<UltrasoundModCompare[UltrasoundModRenew])				
		{
			//PWM2
			if(++UltrasoundTimeCnt2>=6)
			{
				UltrasoundTimeCnt2 = 0;
				UltrasoundModRenew2 = UltrasoundIntensity;
			}
			if(UltrasoundTimeCnt2<UltrasoundIntensityTable[UltrasoundModRenew2])
				ULT_PIN = 1;	//Ultrasound open
			else
				ULT_PIN = 0;	//Ultrasound close
		}
		else
		{
			ULT_PIN = 0;	//Ultrasound close
			UltrasoundTimeCnt2 = 0;
		}	
	}
	else
	{
		UltrasoundTimeCnt = 0;
		UltrasoundTimeCnt2 = 0;
		ULT_PIN = 0;	//Ultrasound close
	}
}
//////////////////////////////////////////////////

//RF function
void PageRF(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_RF);
	dwCutPicL(PIC_RF, btnRF.xs, btnRF.ys, btnRF.xe, btnRF.ye, btnRF.xs, btnRF.ys);
	IconDisplay();
	dwDisNum(DW_SIZE_48, 550, 151, RFIntensity);
	dwDisChar(DW_SIZE_48, 550, 253, RFMode+'A');
	funDisTime(WorkTime);
	if(RFWorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_RF)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				RFIntensity = 0;
				RFMode = 0;
				RFWorkFlag = 0;
			}
		}
		WorkTimeDeal();		
	}
	
	dwWaitRelease();
	dwHandler();
}

//RF PWM contral
const u16 RFIntensityTable[] = {0, 12, 23, 28, 35, 43};
const u16 RFModPeriod[] = {1000, 510, 1020};
const u16 RFModCompare[] = {1000, 252, 504};
void RFPWM(u8 i, u8 Work)
{
	static u16 RFTimeCnt = 0; 
	static u8 RFModRenew = 0;
				
	if(Work)
	{
		//PWM1
		if(++RFTimeCnt>=RFModPeriod[RFModRenew])
		{
			RFTimeCnt = 0;
			RFModRenew = i;
		}
		if(RFTimeCnt<RFModCompare[RFModRenew])				
		{
			TIM_SetCompare4(TIM3, RFIntensityTable[RFIntensity]);
		}
		else
		{
			TIM_SetCompare4(TIM3, 0);
		}	
	}
	else
	{
		RFTimeCnt = 0;
		TIM_SetCompare4(TIM3, 0);
	}
}
/////////////////////////////////

//Ice function
void PageIce(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_ICE);
	dwCutPicL(PIC_ICE, btnIce.xs, btnIce.ys, btnIce.xe, btnIce.ye, btnIce.xs, btnIce.ys);
	IconDisplay();
	dwDisChar(DW_SIZE_48, 550, 253, ColdMode+'A');
	funDisTime(WorkTime);
	if(ColdWorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
//	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
//	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_ICE)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				ColdMode = 0;
				ColdWorkFlag = 0;
			}
		}
		WorkTimeDeal();	
		
		if(ColdWorkFlag)
		{
			POWER_ON_PIN = 1;
			if(ColdMode)
				COLD_HOT_PIN = 1;
			else
				COLD_HOT_PIN = 0;
		}
		else		
		{
			COLD_HOT_PIN = 0;
			POWER_ON_PIN = 0;
		}
	} 
	
	dwWaitRelease();
	dwHandler();
}
///////////////////////////////////

//O2 function
void PageO2(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_O2);
	dwCutPicL(PIC_O2, btnO2.xs, btnO2.ys, btnO2.xe, btnO2.ye, btnO2.xs, btnO2.ys);
	IconDisplay();
	dwDisChar(DW_SIZE_48, 550, 253, O2Mode+'A');
	funDisTime(WorkTime);
	if(O2WorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
//	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
//	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_O2)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				O2Mode = 0;
				O2WorkFlag = 0;
			}
		}
		WorkTimeDeal();
		if(O2WorkFlag)
		{
			PUMP220_PIN = 1;
			if(CleanMode==0)
			{
				LED_RED_PIN = 1;
				LED_GREEN_PIN = 0;
				LED_BLUE_PIN = 0;
			}
			else if(CleanMode==1)
			{
				LED_RED_PIN = 0;
				LED_GREEN_PIN = 1;
				LED_BLUE_PIN = 0;
			}
			else if(CleanMode==2)
			{
				LED_RED_PIN = 0;
				LED_GREEN_PIN = 0;
				LED_BLUE_PIN = 1;
			}			
		}
		else
		{
			PUMP220_PIN = 0;
			LED_RED_PIN = 0;
			LED_GREEN_PIN = 0;
			LED_BLUE_PIN = 0;
		}
	}
	
	dwWaitRelease();
	dwHandler();
}
////////////////////////////////////////////////////

//Clean function
void PageClean(void)
{
	menuExit = 0;
	dwDisPicWithL(PIC_CLEAN);
	dwCutPicL(PIC_CLEAN, btnClean.xs, btnClean.ys, btnClean.xe, btnClean.ye, btnClean.xs, btnClean.ys);
	IconDisplay();
	dwDisChar(DW_SIZE_48, 550, 253, CleanMode+'A');
	funDisTime(WorkTime);
	if(CleanWorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
//	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
//	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_CLEAN)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				CleanMode = 0;
				CleanWorkFlag = 0;
			}
		}
		WorkTimeDeal();
		if(CleanWorkFlag)
		{
			PUMP12_PIN = 1;
			if(CleanMode==0)
			{
				BOTTLE1_PIN = 1;
				BOTTLE2_PIN = 0;
				BOTTLE3_PIN = 0;
			}
			else if(CleanMode==1)
			{
				BOTTLE1_PIN = 0;
				BOTTLE2_PIN = 1;
				BOTTLE3_PIN = 0;
			}
			else if(CleanMode==2)
			{
				BOTTLE1_PIN = 0;
				BOTTLE2_PIN = 0;
				BOTTLE3_PIN = 1;
			}			
		}
		else
		{
			PUMP12_PIN = 0;
			BOTTLE1_PIN = 0;
			BOTTLE2_PIN = 0;
			BOTTLE3_PIN = 0;
		}
	}
	
	dwWaitRelease();
	dwHandler();
}
//////////////////////////////////////////////

//BIO1 function
void PageBIO1(void)
{
	BIO1or2_PIN = 0;
	menuExit = 0;
	dwDisPicWithL(PIC_BIO1);
	dwCutPicL(PIC_BIO1, btnBIO1.xs, btnBIO1.ys, btnBIO1.xe, btnBIO1.ye, btnBIO1.xs, btnBIO1.ys);
	IconDisplay();
	dwDisNum(DW_SIZE_48, 550, 151, BIO1Intensity);
	dwDisChar(DW_SIZE_48, 550, 253, BIO1Mode+'A');
	funDisTime(WorkTime);
	TIM_SetCompare4(TIM4, BIO1IntensityTable[BIO1Intensity]);
	if(BIO1WorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_BIO1)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				BIO1Intensity = 0;
				BIO1Mode = 0;
				BIO1WorkFlag = 0;
			}
		}
		WorkTimeDeal();		
		if(!BIO1WorkFlag)
		{	
			TIM_SetCompare3(TIM4, 0);
		}
	}
	TIM_SetCompare4(TIM4, BIO1IntensityTable[0]);
	dwWaitRelease();
	dwHandler();
}

//BIO1 PWM
const u16 BIO1ModPeriod[] = {167, 250, 400};
const u16 BIO1ModCompare[] = {84, 140, 240};
	
void BIO1PWM(u8 i, u8 Work)
{
	static u16 BIO1TimeCnt = 0;
	static u8 BIO1ModRenew = 0;
	
	if(Work)
	{
		if(++BIO1TimeCnt>=BIO1ModPeriod[BIO1ModRenew])
		{
			BIO1TimeCnt = 0;
			BIO1ModRenew = i;
		}
		if((BIO1TimeCnt<BIO1ModCompare[BIO1ModRenew]))
			TIM_SetCompare3(TIM4, 18000);	//PWM_BIO
		else
			TIM_SetCompare3(TIM4, 0);	//PWM_BIO
	}
	else
	{
		BIO1TimeCnt = 0;
		TIM_SetCompare3(TIM4, 0);	//PWM_BIO
	}
}
/////////////////////////////////////////////////////////

//BIO2 function
void PageBIO2(void)
{
	BIO1or2_PIN = 1;
	menuExit = 0;
	dwDisPicWithL(PIC_BIO2);
	dwCutPicL(PIC_BIO2, btnBIO2.xs, btnBIO2.ys, btnBIO2.xe, btnBIO2.ye, btnBIO2.xs, btnBIO2.ys);
	IconDisplay();
	dwDisNum(DW_SIZE_48, 550, 151, BIO2Intensity);
	dwDisChar(DW_SIZE_48, 550, 253, BIO2Mode+'A');
	funDisTime(WorkTime);
	TIM_SetCompare4(TIM4, BIO2IntensityTable[BIO2Intensity]);
	if(BIO2WorkFlag)
	{	
		dwCutPic(PIC_EFFECT, btnStart.xs, btnStart.ys, btnStart.xe, btnStart.ye, btnStart.xs, btnStart.ys);
	}
	
	dwCancelKey();
	dwListenKey(BIO1Pres, 0, &btnBIO1);
	dwListenKey(BIO2Pres, 0, &btnBIO2);
	dwListenKey(ScrubberPres, 0, &btnScrubber);
	dwListenKey(UltrasoundPres, 0, &btnUltrasound);
	dwListenKey(RFPres, 0, &btnRF);
	dwListenKey(O2Pres, 0, &btnO2);
	dwListenKey(CleanPres, 0, &btnClean);
	dwListenKey(IcePres, 0, &btnIce);
	dwListenKey(TimeUpPres, 0, &btnTimeUp);
	dwListenKey(TimeDownPres, 0, &btnTimeDown);
	dwListenKey(IntensityUpPres, 0, &btnIntensityUp);
	dwListenKey(IntensityDownPres, 0, &btnIntensityDown);
	dwListenKey(ModeUpPres, 0, &btnModeUp);
	dwListenKey(ModeDownPres, 0, &btnModeDown);
	dwListenKey(StartPres, 0, &btnStart);
//	dwListenKey(MinPres, 0, &btnMin);
	dwWaitRelease();	
	
	while(!menuExit)
	{
		dwHandler();
		if(nextPage!=FUNCTION_BIO2)
		{
			menuExit = 1;
			if(nextPage!=FUNCTION_MAIN)
			{
				BIO2Intensity = 0;
				BIO2Mode = 0;
				BIO2WorkFlag = 0;
			}
		}
		WorkTimeDeal();		
		if(!BIO2WorkFlag)
		{	
			TIM_SetCompare3(TIM4, 0);
		}
	}
	BIO1or2_PIN = 0;
	TIM_SetCompare4(TIM4, BIO2IntensityTable[0]);
	dwWaitRelease();
	dwHandler();
}

//BIO2 PWM
const u16 BIO2ModPeriod[] = {40, 400, 1200};
const u16 BIO2ModCompare[] = {20, 240, 800};
	
void BIO2PWM(u8 i, u8 Work)
{
	static u16 BIO2TimeCnt = 0;
	static u8 BIO2ModRenew = 0;
	
	if(Work)
	{
		if(++BIO2TimeCnt>=BIO2ModPeriod[BIO2ModRenew])
		{
			BIO2TimeCnt = 0;
			BIO2ModRenew = i;
		}
		if((BIO2TimeCnt<BIO2ModCompare[BIO2ModRenew]))
			TIM_SetCompare3(TIM4, 18000);	//PWM_BIO
		else
			TIM_SetCompare3(TIM4, 0);	//PWM_BIO
	}
	else
	{
		BIO2TimeCnt = 0;
		TIM_SetCompare3(TIM4, 0);	//PWM_BIO
	}
}
/////////////////////////////////////////////////////////

//Bee function
const u16 BeeModPeriod[] = {110, 410, 1000}; 
const u16 BeeModCompare[] = {100, 400, 500};
u8 BeeMod = 0, BeeTime = 0;
void BeeFunction(void)
{	
	static u16 BeeTimeCnt = 0;

	if(BeeTime>0)
	{
		if(++BeeTimeCnt>=BeeModPeriod[BeeMod])
		{
			BeeTimeCnt = 0;
			BeeTime--;
		}
		else
		{
			if(BeeTimeCnt>=BeeModCompare[BeeMod])
				SOUND_PIN = 0;
			else
				SOUND_PIN = 1;		
		}	
	}
	else
	{
		SOUND_PIN = 0;
	}
}

//Rec and process frame
u8 buff2[5];
u8 rx2Flag = 0;
void commRx2Handler(u8 byte)
{
  static u8 count = 0;
  
  if(count == 0){
    if(byte == FRAME_HEAD){
      count++;
    }
  }else if(count < 4){
    buff2[count-1] = byte;
    count++;
  }else{
    count = 0;
    if(byte == FRAME_END){
			rx2Flag = 1;
    }
  }
}

u8 buff1[5];
u8 rx1Flag = 0;
void commRx1Handler(u8 byte)
{
  static u8 count = 0;
  
  if(count == 0){
    if(byte == FRAME_HEAD){
      count++;
    }
  }else if(count < 4){
    buff1[count-1] = byte;
    count++;
  }else{
    count = 0;
    if(byte == FRAME_END){
			rx1Flag = 1;
    }
  }
}

