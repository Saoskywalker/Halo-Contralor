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

#endif
