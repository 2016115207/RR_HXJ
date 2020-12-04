/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-27 16:13:00
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\reset.h
 */ 
#ifndef __RESET__H
#define __RESET__H

#include "logicdata.h"
#include "fsm.h"

#define REST_TASK ResetTask

extern void Reset(void);
extern void ResetMotor(void);
extern void MotorGoHome(u8 ax);

#endif
