#ifndef _SET_IO_H
#define _SET_IO_H

#include "sys.h"

//Hardware define
#define LED_RED_PIN PBout(12)   //Contral pin
#define LED_GREEN_PIN PBout(13)   
#define LED_BLUE_PIN PBout(14)
#define MOUTH_PIN PBout(15)

#define STEP1_PIN PBout(1)  //step motor 
#define STEP2_PIN PBout(0)
#define STEP3_PIN PCout(5)
#define STEP4_PIN PCout(4)

#define CON_5v_PIN PDout(2) //+5v contral
#define DEBUG_LED PBout(1)

#define HEAD_M1_PIN PCout(8) //Motor
#define HEAD_M2_PIN PCout(9) 
#define BODY_M1_PIN PCout(6) 
#define BODY_M2_PIN PCout(7) 

#define HALL_EAR_PIN PBin(8)
#define HALL_RIGHT_PIN PBin(6)
#define HALL_LEFT_PIN PBin(7)

#define WAKE_UP_PIN PCin(1)
#define LLL1_PIN PAin(6) //without using
#define LLL2_PIN PAin(7) //without using

void IO_Init(void);

#endif 
