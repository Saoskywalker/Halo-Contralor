#ifndef _APP_LIB_H
#define _APP_LIB_H

#include "UserBaseLib.h"
#include "timer.h"
#include "usart.h"
#include "exti.h"
#include "delay.h"
#include "Set_IO.h"
// #include "adc.h"
#include "stmflash.h"
#include "dht11.h"
#include "rtc.h" 
#include "wkup.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "GlobeValue.h"

//Temperature table
#define TEMP_40 1418
#define TEMP_38 1503
#define TEMP_36 1582
#define TEMP_0 3132
#define TEMP_N5 3307
#define TEMP_N10 3464

//Flash function
//设置FLASH 保存地址(必须为偶数)
#define FLASH_SAVE_ADDR  0X0800F800 	

u8 DistInitialization(void);
u8 MusicInitialization(void);
void MusicStart(u8 i);

void HeadMotorUp(u16 Speed);
void HeadMotorDown(u16 Speed);
void HeadMotorStop(void);

void MiddleMotorRight(u16 Speed);
void MiddleMotorLeft(u16 Speed);
void MiddleMotorStop(void);

void RGB_PWM(u8 *i, u8 *p, u8 *g);
void RGB_Decode(const u16 *i);
void RGB_Renew(u8 R, u8 G, u8 B);

extern u8 ActionType;

#define ACTION_INTERACTION 1
#define ACTION_RADOM 2
#define ACTION_TIME 3
#define ACTION_REMIND 4

void Interaction(void);
void TimeAction(void);
void RemindAction(void);

#endif
