#include "AppLib.h"

//music command

//distinguish command
static const u8 DistInit[] = {0XA0, 0XA0, 0XA0};
static const u8 DistInitRe[] = {0X50, 0X50, 0X16};
static u8 DistGroup[] = {0XA9, 0X01, 0X00};
static u8 DistGroupRe[] = {0X59, 0X01, 0X00};
static u8 DistStartOnce[] = {0XAA, 0X0A, 0X00};
static u8 DistStartOnceRe[] = {0X5A, 0XFF, 0XFF};

		if (Check_WKUP())
			Sys_Enter_Standby(); //进入待机模式