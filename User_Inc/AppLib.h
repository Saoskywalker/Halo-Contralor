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

#define V_ZERO 2
#define V_ONE 3
#define V_TWO 4
#define V_THREE 5
#define V_FOUR 6
#define V_FIVE 7
#define V_SIX 8
#define V_SEVEN 9
#define V_EIGHT 10
#define V_NINE 11
#define V_TEN 12
#define V_HOUR 18
#define V_MIN 15
#define V_TIME_NOW 1
#define V_HELLO 13
#define V_GETUP 16
#define V_TEMP 19
#define V_HUM 17
#define V_NULL 14

#define DIST_BAO_GAO 0X00
#define DIST_WEN_DU 0X01
#define DIST_SHI_DU 0X02
#define DIST_HA_LUO 0X03
#define DIST_NI_HAO 0X04

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

extern u8 DistStartOnceRe[];
u8 DistInitialization(void);
u8 MusicInitialization(void);
extern u8 VoiceTable[];
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

void Interaction(void);
void TimeAction(void);
void RemindAction(void);
void RandomAction(void);

#endif
