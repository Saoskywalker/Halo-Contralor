#ifndef _APP_LIB_H
#define _APP_LIB_H

#include "UserBaseLib.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"
#include "stmflash.h"

//Temperature table
#define TEMP_40 1418
#define TEMP_38 1503
#define TEMP_36 1582
#define TEMP_0 3132
#define TEMP_N5 3307
#define TEMP_N10 3464

//Hardware define
//NEW
#define CON1_PIN PAout(6)   //Contral pin
#define CON2_PIN PBout(0)
#define CON3_PIN PBout(2)
#define CON4_PIN PAin(15)
#define STEP1_PIN PBout(4)  //step motor 
#define STEP2_PIN PBout(5)
#define STEP3_PIN PBout(6)
#define STEP4_PIN PBout(7)
#define CON_5v_PIN PBout(9) //+5v contral

//Flash function
#define FLASH_SAVE_ADDR  0X0800F800 				//设置FLASH 保存地址(必须为偶数)

#endif
