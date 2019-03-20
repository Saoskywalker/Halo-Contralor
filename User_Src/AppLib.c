#include "AppLib.h"

//music command

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

void MusicCommand(u8 i)
{
	u8 CC[] = {0XAA, 0X90, 0X55, 0XAA, 0X5A, 0XA5, 0X01, 0XDE,
			   0XE9, 0XF8, 0X00, 0X01, 0XCC, 0X33, 0XC3, 0X3C};
	u8 j = 1, k = 0;

	CC[11] = i; //open or close
	for (; sizeof(CC) >= j; j++, k++)
		uasrt2SendByte(CC[k]);
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

	DistCommand(&DistInit, sizeof(DistInit));
	delay_ms(100);
	
}
