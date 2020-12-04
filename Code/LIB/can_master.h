#ifndef __CAN_NET_H
#define __CAN_NET_H

#include "stdint.h"

typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;
typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;



//设置参数
//轴走绝对位置
extern s32 HZ_ExAxSetPara(u8 ExAxNum,\
				u32 Start,u32 TAcc,u32 Run,u32 TDec,u32 End,\
				u32 HomFast,u32 HomSlow,u32 HomOffset,\
				u32 TorS,u32 HomTimeOut);
//设置回原点模式
//AxNum :轴号
//en	:使能
//Sig	:原点信号
//Mod	:模式
//		0：反向找原点
//		1：先正向找上限位，再反向找原点
//		2：先反向找下限位，再正向找原点
extern s32 HZ_ExAxSetHomMod(u8 ExAxNum,u8 En,u8 Sig,u8 Lev,u8 Mod);
//设置硬限位
extern s32 HZ_ExAxSetLimit_H(u8 ExAxNum,u8 en_Up,u8 sig_Up,u8 lev_Up,u8 en_Dw,u8 sig_Dw,u8 lev_Dw);
//设置软限位
extern s32 HZ_ExAxSetLimit_S(u8 ExAxNum,s32 pos_Up,u8 en_Up,s32 pos_Dw,u8 en_Dw);
//获取轴位置
extern s32 HZ_ExAxGetCurPos(u8 ExAxNum);
//获取轴运行状态
//return 0：空闲
//		1：忙
extern s32 HZ_ExAxGetExe(u8 ExAxNum);



extern void HZ_ExAxMoveRel(u8 ExAxNum,s32 pos);
extern void HZ_ExAxMoveAbs(u8 ExAxNum,s32 pos);
extern void HZ_ExAxMoveVelocity(u8 ExAxNum,s32 pos);
extern s32  HZ_ExAxGetStatus(u8 ExAxNum);
extern void HZ_ExAxStopDec(u8 ExAxNum);
extern void HZ_ExAxStop(u8 ExAxNum);
extern void HZ_ExAxHome(u8 ExAxNum);
//获取错误状态
//return bit8: 硬上限bit9:软上限bit12：硬下限bit13：软下限
extern u32 HZ_ExAxGetErr(u8 ExAxNum);


extern s32 HZ_ExOutPutSet(u8 bid,u8 num,u8 val);
extern s32 HZ_ExOutPutGet(u8 bid,u8 num);
extern s32 HZ_ExEnSet(u8 bid,u8 num,u8 val);
extern s32 HZ_ExEnGet(u8 bid,u8 num);
extern s32 HZ_ExInPutGet(u8 bid,u8 num);
extern s32 HZ_ExAlmGet(u8 bid,u8 num);
extern s32 HZ_ExLimGet(u8 bid,u8 num);
//获取can网络状态，只有=2时才可以进行其他设置动作
extern s32 HZ_ExCanNetStateGet(void);
//板卡通讯错误，每个位代表一个板卡错误
extern u32 HZ_ExCanComErrGet(void);


u32 HZ_ExD32IGet(u32 bid,u32 num);
u32 HZ_ExD32OGet(u32 bid,u32 num);
s32 HZ_ExD32OSet(u32 bid,u32 num,u32 val);


void can_netLoop(void);

#endif

