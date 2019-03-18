#ifndef __GLOBAL_H
#define __GLOBAL_H

//定时标志
typedef struct
{
    unsigned char menuExit : 1;
    unsigned char rx1Flag : 1;
    unsigned char Flag1s : 1;
    unsigned char WorkFlag : 1;
    unsigned char ms5 : 1;
    unsigned char ms6 : 1;
    unsigned char ms7 : 1;
    unsigned char ms8 : 1;
} _BitTimeBit;

extern _BitTimeBit BitTimeBit;

#endif
