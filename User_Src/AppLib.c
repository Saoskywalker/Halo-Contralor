#include "AppLib.h"

u8 ActionType = 0;

//music command
const u8 _MusicInit[] = {0XAA, 0X0A, 0X00}; //model Init, check Drive
const u8 _MusicVol[] = {0xAA, 0x13, 0x01, 0x1E}; //Vol sets biggest
u8 _MusicPlay[] = {0xAA, 0x07, 0x02, 0x00, 0x01};
const u8 _MusicStop[] = {0XAA, 0X10, 0X00};

//distinguish command
const u8 _DistInit[] = {0XA0, 0XA0, 0XA0}; //model Init
const u8 _DistInitRe[] = {0X50, 0X50, 0X16};
u8 _DistGroup[] = {0XA9, 0X01, 0X00}; //Select group
u8 _DistGroupRe[] = {0X59, 0X01, 0X00};
u8 _DistStartOnce[] = {0XAA, 0X0A, 0X00}; //start once work
u8 _DistStartOnceRe[] = {0X5A, 0XFF, 0XFF};

//RGB driver
static const u16 RGB_Table[] = {0X0000, 0xF800, 0x07E0, 0x001F};
//RGB565 decode
void RGB_PWM(u8 *i, u8 *p, u8 *g)
{
	static u8 _TimeCnt = 0;
	static u8 _RModRenew = 0, _GModRenew = 0, _BModRenew = 0;
	
	if(++_TimeCnt>=160) //cycle 16ms
	{
		_TimeCnt = 0;
		_RModRenew = *i;
		_GModRenew = *p;
		_BModRenew = *g;
	}
	
	//red
	if(_TimeCnt<_RModRenew)
	{	//OPEN
		LED_RED_PIN = 1;	
	}
	else	//COLSE
	{
		LED_RED_PIN = 0;
	}
	
	//green
	if(_TimeCnt<_GModRenew)
	{	//OPEN
		LED_GREEN_PIN = 1;	
	}
	else	//COLSE
	{
		LED_GREEN_PIN = 0;
	}
	
	//blue
	if(_TimeCnt<_BModRenew)
	{	//OPEN
		LED_BLUE_PIN = 1;	
	}
	else	//COLSE
	{
		LED_BLUE_PIN = 0;
	}
}

void RGB_Decode(const u16 *i)
{
	u8 R, G, B;
	
	R = *i>>11;
	G = (*i&0x07E0)>>5;
	B = *i&0x001F;
	
	RGB_Renew(R, G, B);
}

void RGB_Renew(u8 R, u8 G, u8 B)
{
	_CR = R;
	_CG = G;
	_CB = B;
}

void MusicCommand(u8 *i, u8 size)
{
	u8 j = 1, k = 0;

	for (; j<=size; j++, i++)
	{
		uasrt3SendByte(*i);
		k += *i; 
	}
	uasrt3SendByte(k);
}

//init return 0:OK other:ERROR
u8 MusicInitialization(void)
{
	u8 i = 0;

	while (i < 2)
	{
		MusicCommand((u8 *)&_MusicInit[0], sizeof(_MusicInit));
		delay_ms(100);
		if (UART3_RX_Cnt != 0)
		{
			if ((0X01 == UART3_RX_Cache[2]) &&
				(0X02 == UART3_RX_Cache[3]))
			{
				UART3_RX_Cnt = 0;
				return 0; //success
			}
			else
			{
				UART3_RX_Cnt = 0;
			}
		}
		i++;
	}
	MusicCommand((u8 *)&_MusicVol[0], sizeof(_MusicVol));
	return 1; //error
}

void MusicStart(u8 i)
{
	_MusicPlay[4] = i;
	MusicCommand(&_MusicPlay[0], sizeof(_MusicPlay));
}

void DistCommand(u8 *i, u8 size)
{
	u8 j = 1;

	for (; j<=size; j++, i++)
		uasrt2SendByte(*i);
}

//init return 0:OK other:ERROR
u8 DistInitialization(void)
{
	u8 i = 0;

	while (i < 2)
	{
		DistCommand((u8 *)&_DistInit[0], sizeof(_DistInit));
		delay_ms(100);
		if (UART2_RX_Cnt != 0)
		{
			if ((_DistInitRe[0] == UART2_RX_Cache[0]) &&
				(_DistInitRe[1] == UART2_RX_Cache[1]))
			{
				UART2_RX_Cnt = 0;
				return 0; //success
			}
			else
			{
				UART2_RX_Cnt = 0;
			}
		}
		i++;
	}
	return 1; //error
}

//Head Motor
void HeadMotorUp(u16 Speed)
{
	MOTOR1_PIN = 1; //up
	TIM_SetCompare1(TIM1, 7199-Speed);
}

void HeadMotorDown(u16 Speed)
{
	MOTOR1_PIN = 0; //down
	TIM_SetCompare1(TIM1, Speed);
}

void HeadMotorStop(void)
{
	MOTOR1_PIN = 0; //stop
	TIM_SetCompare1(TIM1, 0);
}

//Middle Motor
void MiddleMotorRight(u16 Speed)
{
	MOTOR2_PIN = 1; //right
	TIM_SetCompare4(TIM1, 7199-Speed);
}

void MiddleMotorLeft(u16 Speed)
{
	MOTOR2_PIN = 0; //left
	TIM_SetCompare4(TIM1, Speed);
}

void MiddleMotorStop(void)
{
	MOTOR2_PIN = 0; //stop
	TIM_SetCompare4(TIM1, 0);
}

void Interaction(void)
{
	MusicStart(1);
	delay_ms(300);
	HeadMotorDown(7199);
	MOUTH_PIN = 0;
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorDown(7199);
	MOUTH_PIN = 0;
	MusicStart(1);
	delay_ms(250);
	HeadMotorUp(7199);
	MOUTH_PIN = 1;
	delay_ms(300);
	HeadMotorStop();

	/*
	listening
	*/
}


