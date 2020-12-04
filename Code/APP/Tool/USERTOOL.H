/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:21
 * @LastEditTime: 2020-05-29 16:37:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Tool\USERTOOL.H
 */ 
#ifndef _USERTOOL_H_
#define	_USERTOOL_H_

//#include "DataDefine.h"
#include "bsplib.h"
#include "math.h"
//#include "interlayer_config.h"
//#include "iodefine.h"


s32 ConverToPulse(u32 num,float MM);
float ConvertToUserUnit(u32 num,s32 pulse);

s32 InGet(u8 cardAdr,u8 num);
s32 OutGet(u8 cardAdr,u8 num);
s32 OutSet(u8 cardAdr,u8 num,u8 value);

#endif


