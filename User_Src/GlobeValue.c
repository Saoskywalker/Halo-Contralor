#include "GlobeValue.h"

_BitTimeBit BitTimeBit = {0,0,0,0,0,0,0,0};
_BitErrorBit BitErrorBit = {0,0,0,0,0,0,0,0};

unsigned char KeyWakeUpPress = 0;
unsigned char KeyWakeUpPressLong = 0;

unsigned char UART2_RX_Cache[UART2_RX_SIZE];
unsigned char UART2_RX_Cnt = 0;

unsigned char UART3_RX_Cache[UART3_RX_SIZE];
unsigned char UART3_RX_Cnt = 0;

unsigned char _CR = 0, _CG = 0, _CB = 0;

unsigned char RTCAlarm = 0;
unsigned char CloseTime = 0;

unsigned int SysRunTime = 0;
unsigned int RandTime[] = {0, 0};
unsigned char RandActionRun = 0;

unsigned char ActionType = 0;
