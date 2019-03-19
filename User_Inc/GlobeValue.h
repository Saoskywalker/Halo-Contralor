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
    unsigned char RTC : 1;
    unsigned char ms6 : 1;
    unsigned char ms7 : 1;
    unsigned char ms8 : 1;
} _BitErrorBit;

extern _BitErrorBit BitErrorBit;

#endif
