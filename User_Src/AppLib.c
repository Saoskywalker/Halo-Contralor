#include "AppLib.h"

//music command
const u8 MusicInit[] = {0XAA, 0X0A, 0X00}; //model Init, check Drive
const u8 MusicVol[] = {0xAA, 0x13, 0x01, 0x1E}; //Vol sets biggest
u8 MusicPlay[] = {0xAA, 0x02, 0x00};
const u8 MusicStop[] = {0XAA, 0X10, 0X00};

//distinguish command
const u8 DistInit[] = {0XA0, 0XA0, 0XA0}; //model Init
const u8 DistInitRe[] = {0X50, 0X50, 0X16};
u8 DistGroup[] = {0XA9, 0X01, 0X00}; //Select group
u8 DistGroupRe[] = {0X59, 0X01, 0X00};
u8 DistStartOnce[] = {0XAA, 0X0A, 0X00}; //start once work
u8 DistStartOnceRe[] = {0X5A, 0XFF, 0XFF};

//RGB driver
static const u16 RGB_Table[] = {0X0000, 0xF800, 0x07E0, 0x001F};
//RGB565 decode
static void RGB_PWM(u8 *i, u8 *p, u8 *g)
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

static void RGB_Decode(const u16 *i)
{
	u8 R, G, B;
	
	R = *i>>11;
	G = (*i&0x07E0)>>5;
	B = *i&0x001F;
	
	RGB_PWM(&R, &G, &B);
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
		MusicCommand((u8 *)&MusicInit[0], sizeof(MusicInit));
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
	MusicCommand((u8 *)&MusicVol[0], sizeof(MusicVol));
	MusicCommand(&MusicPlay[0], sizeof(MusicPlay));
	return 1; //error
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
		DistCommand((u8 *)&DistInit[0], sizeof(DistInit));
		delay_ms(100);
		if (UART2_RX_Cnt != 0)
		{
			if ((DistInitRe[0] == UART2_RX_Cache[0]) &&
				(DistInitRe[1] == UART2_RX_Cache[1]))
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
