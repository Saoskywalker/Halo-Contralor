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

//music code 
#define	MSC_BUTTON  0
#define	MSC_START   1
#define	MSC_STOP    2

extern u8 nextPage;
#define FUNCTION_RESTART 0
#define FUNCTION_SCRUBBER 1
#define FUNCTION_RF 2
#define FUNCTION_ULTRASOUND 3
#define FUNCTION_ICE 4
#define FUNCTION_BIO1 5
#define FUNCTION_BIO2 6
#define FUNCTION_SCRUBBER_ADJ 7
#define FUNCTION_MAIN 8
#define FUNCTION_O2 9
#define FUNCTION_CLEAN 10
#define TEST 11

//Flash function
#define FLASH_SAVE_ADDR  0X0800F800 				//设置FLASH 保存地址(必须为偶数)
extern u16 ScrubberFrequency;

extern u8 buff2[5];
extern u8 rx2Flag;
void commRx2Handler(u8 byte);
extern u8 buff1[5];
extern u8 rx1Flag;
void commRx1Handler(u8 byte);
extern u8 Flag1ms;
extern long int HallCount;

void ScrubberPWM(u8 i, u8 Work);
void UltrasoundPWM(u8 i, u8 Work);
void BIO1PWM(u8 i, u8 Work);
void BIO2PWM(u8 i, u8 Work);
void RFPWM(u8 i, u8 Work);

extern u8 Flag1s, Flag50ms;

extern u8 BeeMod, BeeTime;
void BeeFunction(void);
#define INLINE_MUSIC_BUTTON() {BeeMod = 0; BeeTime = 1;}
#define INLINE_MUSIC_START() {BeeMod = 1; BeeTime = 1;}
#define INLINE_MUSIC_STOP() {BeeMod = 2; BeeTime = 3;}
#define INLINE_MUSIC_ERROR() {BeeMod = 0; BeeTime = 7;}
#define INLINE_MUSIC_SCR_ADJ() {BeeMod = 1; BeeTime = 2;}

#endif
