#ifndef _APP_LIB_H
#define _APP_LIB_H

#include "UserBaseLib.h"
#include "dwDriver.h"
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
#define SOUND_PIN PBout(15)
#define ULT_PIN PAout(12)
#define OUTPUT14_PIN PBout(14)
#define INPUT14_PIN PBin(14)
#define OUTPUT13_PIN PBout(13)
#define POWER_ON_PIN PBout(5)
#define COLD_HOT_PIN PBout(6)
#define BIO1or2_PIN PBout(7)
#define PUMP220_PIN PBout(4)
#define PUMP12_PIN PAout(0)
#define VALVE1_PIN PAout(4)
#define BOTTLE1_PIN PAout(5)
#define BOTTLE2_PIN PAout(6)
#define BOTTLE3_PIN PAout(7)
#define LED_RED_PIN PBout(0)
#define LED_GREEN_PIN PBout(10)
#define LED_BLUE_PIN PBout(11)
#define FAN1_PIN PBout(12)

//Picture Code
#define PIC_START_ANIMATION 0
#define PIC_MAIN 11
#define PIC_SCRUBBER 16
#define PIC_SCRUBBER_ADJ 2
#define PIC_RF 15
#define PIC_ULTRASOUND 14
#define PIC_ICE 17
#define PIC_CLEAN 19
#define PIC_O2 18
#define PIC_BIO1 12
#define PIC_BIO2 13
#define PIC_EFFECT 20
#define PIC_NUMBER 7
#define PIC_ORIG 20

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
extern u8 BIO1Mode, BIO2Mode, ScrubberMode, UltrasoundMode, RFMode, ColdMode, O2Mode, CleanMode;
extern u8 ScrubberWorkFlag, O2WorkFlag, RFWorkFlag, BIO1WorkFlag, BIO2WorkFlag, CleanWorkFlag, UltrasoundWorkFlag, ColdWorkFlag;
extern u8 menuExit;
void IO_Init(void);
void PageRestartAnimation(void);
void PageMain(void);
void PageScrubber(void);
void PageBIO1(void);
void PageUltrasound(void);
void PageIce(void);
void PageBIO2(void);
void PageO2(void);
void PageClean(void);
void PageRF(void);
void PageScrubberAdj(void);
void test(void);

extern u8 BeeMod, BeeTime;
void BeeFunction(void);
#define INLINE_MUSIC_BUTTON() {BeeMod = 0; BeeTime = 1;}
#define INLINE_MUSIC_START() {BeeMod = 1; BeeTime = 1;}
#define INLINE_MUSIC_STOP() {BeeMod = 2; BeeTime = 3;}
#define INLINE_MUSIC_ERROR() {BeeMod = 0; BeeTime = 7;}
#define INLINE_MUSIC_SCR_ADJ() {BeeMod = 1; BeeTime = 2;}

#endif
