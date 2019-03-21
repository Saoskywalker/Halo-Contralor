#include "GlobeValue.h"

_BitTimeBit BitTimeBit = {0,0,0,0,0,0,0,0};
_BitErrorBit BitErrorBit = {0,0,0,0,0,0,0,0};

unsigned char KeyWakeUpPress = 0;

unsigned char UART2_RX_Cache[UART2_RX_SIZE];
unsigned char UART2_RX_Cnt = 0;

unsigned char UART3_RX_Cache[UART3_RX_SIZE];
unsigned char UART3_RX_Cnt = 0;



