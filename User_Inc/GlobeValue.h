#ifndef __GLOBAL_H
#define __GLOBAL_H

//定时标志
typedef struct
{
    unsigned char Flag1s : 1;
    unsigned char Flag500ms : 1;
    unsigned char Flag100us : 1;
    unsigned char Flag1ms : 1;
    unsigned char ms5 : 1;
    unsigned char ms6 : 1;
    unsigned char ms7 : 1;
    unsigned char ms8 : 1;
} _BitTimeBit;

extern _BitTimeBit BitTimeBit;

//错误标志
typedef struct
{
    unsigned char MPU6050 : 1;
    unsigned char DHT11 : 1;
    unsigned char MUSIC : 1;
    unsigned char DISTINGUISH : 1;
    unsigned char RealTimer : 1;
    unsigned char ERROR : 1; //globe
    unsigned char OTHERS : 1; //other 
    unsigned char ms8 : 1;
} _BitErrorBit;

extern unsigned char RTCAlarm;
extern unsigned char CloseTime;

extern _BitErrorBit BitErrorBit;
extern unsigned char KeyWakeUpPress;
extern unsigned char KeyWakeUpPressLong;

#define UART2_RX_SIZE 8
extern unsigned char UART2_RX_Cnt;
extern unsigned char UART2_RX_Cache[UART2_RX_SIZE];

#define UART3_RX_SIZE 8
extern unsigned char UART3_RX_Cache[UART3_RX_SIZE];
extern unsigned char UART3_RX_Cnt;

extern unsigned char _CR, _CG, _CB;

extern unsigned int SysRunTime;
extern unsigned int RandTime[] ;
extern unsigned char RandActionRun;

extern unsigned char ActionType;

#define ACTION_INTERACTION 1
#define ACTION_RADOM 2
#define ACTION_TIME 3
#define ACTION_REMIND 4

#endif
