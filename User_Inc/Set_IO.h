#ifndef _SET_IO_H
#define _SET_IO_H

#include "sys.h"

//Hardware define
#define LED_GREEN_PIN PBout(0)   //Contral pin
#define LED_RED_PIN PAout(6)
#define LED_BLUE_PIN PAout(1)
#define MOUTH_PIN PAout(15)
#define STEP1_PIN PBout(4)  //step motor 
#define STEP2_PIN PBout(5)
#define STEP3_PIN PBout(6)
#define STEP4_PIN PBout(7)
#define CON_5v_PIN PBout(9) //+5v contral
// #define DEBUG_LED PCout(13)
#define DEBUG_LED PAout(15)
#define MOTOR1_PIN PAout(7) //Motor
#define MOTOR2_PIN PAout(12) 

#define WAKE_UP_PIN PAin(0)

void IO_Init(void);

#endif 
