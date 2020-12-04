/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-30 22:51:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\LogicData.h
 */ 
#ifndef __LOGICDATA_H
#define __LOGICDATA_H
#include "bsplib.h"

typedef struct
{
    u8 execute;
    u8 step;
    u8 done;
	u8 err;		//错误码
    u32 count;	//用于记录圈数，必须放大单位
    u32 tm;
	u8 state;
} LogicParaDef;

#define TimerRst(a)		 a.tm = SysTim_GetUser()		  //0.1 ms
#define TimerCnt(a)		((SysTim_GetUser()-a.tm)/10) 	//计时转成ms

#define	ON	0
#define	OFF	1

#define PARAINIT(a)  memset(&a,0,sizeof(a));
#define INITTASK(a) if(a.execute==1&&a.step==0){a.step = 1;}


#define TRST(T) T->tm = SysTim_GetUser()
#define TCNT(T) ((SysTim_GetUser() - (T->tm)) / 10)
#define INITT(a)                      \
	if (a->execute == 1 && a->step == 0) \
	{                                    \
		a->step = 1;                      \
		a->done = 0;						\
		TRST(a);                             \
	}
#define STEP Task->step 

typedef struct PointDef_t
{
	int x;
	int y;
} PointDef2;
typedef struct PointfDef_t
{
	float x;
	float y;
} PointfDef2;

#endif
