#ifndef _SET_IO_H
#define _SET_IO_H

#include "sys.h"

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
#define DEBUG_LED PCout(13)

void IO_Init(void);

#endif 
