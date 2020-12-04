#ifndef	__INTERLAYER_CONFIG_H
#define	__INTERLAYER_CONFIG_H

#include "bsplib.h"

//轴信号获取，包括普通IO和轴IO，编码器IO
//普通IO
//0  ->127 :普通IO
//轴
//128 ->143  :ALM
//144 ->159  :LIMIT
//160 ->175  :
//176 ->191  :
//编码器
//192 ->207  :编码器A
//208 ->223  :编码器B(Z相)
//224 ->139
//240 ->255

//十进制转ASCII码
#define KB2ASC(x)	 (unsigned char )(0x30+x)

//ASCII码转十进制
#define ASC2KB(x)	 (unsigned char )(x%(0x30))

//只能对规定的四个字节进行解码
//decode 解锁码 //decodeData 解锁之后的字符串 cid 客户码 rng 随机数* @return -- 0 解码成功，-1 失败
extern int Decode(int* decode,char* decodeData,unsigned int cid,unsigned int rng);
/**
* @author  yang 2019/2/27
* @Description: 解密字符串 
* @param -- word 需要解密的字符串  s 解密之后的字符串 字符串长度
* @return -- 0 解密成功 -1 解密成功
*/
extern int Decrypt(char* word,char* s,int len);	
/**
* @author  yang 2019/2/27
* @Description: 加密字符串 
* @param -- word 需要加密的字符串  s 加密之后的字符串 字符串长度
* @return -- 0 加密成功 -1 解密成功
*/
extern int Encrypt( char* word, char* s,int len);

extern void	HZ_AxMotion(void);
extern void HZ_ModBus(u32 port,char ModbusLocalID); //ModbusLocalID 请默认填1
extern void HZ_TimerThread(void);
extern void	HZ_FlashOperate(void);
//m_axnum 主卡轴数	
extern void HZ_JogOperate(int M_Axnum);
extern void HZ_Alarm(void);
extern void init_config(void* GlobalData,u32* err,void* flah,u32 datalen,u32 writelen,u32 readlen,u32 savelen);
extern s32 HZ_Data_Read(void);
extern s32 HZ_BkData_Read(void);
extern s32 HZ_Data_Write(void);
extern s32 HZ_BkData_Write(void);



//对外接口

#define	SYSTIMERNUM	100
extern u32	sysTimer[SYSTIMERNUM];
#define	USERTIMERNUM	200
extern u32	UserTimer[USERTIMERNUM];
#define	TRIGERNUM	200
#define	TIMER_NUM	200
extern u8	TrigerFlag[TRIGERNUM];
extern u32	TrigerTimer[TRIGERNUM];
extern unsigned char  flagbuf[TIMER_NUM];
extern unsigned int  ton_et[TIMER_NUM];
extern u8 Triger(u8 Num, s32 TrigValue, s32 TrigerInput ,u32 ContTime);
extern unsigned char ton(unsigned int num,unsigned char ex,unsigned int pt);
extern void AlarmSetBit(u8 level, u8 word, u8 bit);
extern void AlarmClearBit(u8 level, u8 word, u8 bit);
extern s32 AlarmGetBit(u8 level, u8 word, u8 bit);

//轴运动函数

extern u32 GetSpd(u8 axNum);
extern u32 GetDecSteps(u8 axNum);

//对外接口
//01绝对定位
extern s32 HZ_AxMoveAbs(u8 AxNum,s32 pos);
//02相对定位
extern s32 HZ_AxMoveRel(u8 AxNum,s32 pos);
//03叠加定位
extern s32 HZ_AxMoveAdd(u8 AxNum,s32 pos);
//重新定位
//extern s32 HZ_AxMoveSuperImposed();
//恒速运行()
//AxNum :轴号，从0开始
//pos	:大于0正向，小于0反向 0：错误
extern s32 HZ_AxMoveVelocity(u8 AxNum,s32 pos);
extern s32 HZ_AxChSpd(u8 AxNum,u32 Spd);
//停止轴
extern s32 HZ_AxStop(u8 AxNum);

//////*****//////////////
//减速停
extern s32 HZ_AxStopDec(u8 AxNum);
//回原点
extern s32 HZ_AxHome(u8 AxNum);


//复位轴(复位错误及把轴状态归为就绪)
extern s32 HZ_AxReset(u8 AxNum);
//读轴状态
extern s32 HZ_AxGetStatus(u8 AxNum);
//读轴错误
extern s32 HZ_AxGetAxisErr(u8 AxNum);
//读当前位置
extern s32 HZ_AxGetCurPos(u8 AxNum);
//写当前位置
extern s32 HZ_AxSetCurPos(u8 AxNum,s32 pos);
//设置参数
extern s32 HZ_AxSetPara(u8 AxNum,\
				u32 Start,u32 TAcc,u32 Run,u32 TDec,u32 End,\
				u32 HomFast,u32 HomSlow,u32 HomOffset,\
				u32 TorS,u32 HomTimeOut);
//设置速度曲线
extern s32 HZ_AxSetCurve(u8 AxNum,u32 Start, u32 TAcc, u32 Run, u32 TDec, u32 End,u32 TorS);
//设置轴回原点参数
extern s32 HZ_AxSetHomePara(u8 AxNum,u8 HomSigNum,u8 HomSigLev,u8 Mod,s32 fast,s32 slow,s32 offset);
//设置轴信号参数
extern s32 HZ_AxSetAxiSig(u8 AxNum,\
					u8 HomSigEn,u8 HomSigNum,u8 HomSigLev,\
					u8 UpSigEn,u8 UpSigNum,u8 UpSigLev,\
					u8 DwSigEn,u8 DwSigNum,u8 DwSigLev);
//设置上下软限位
extern s32 HZ_AxSetLimSoft(u8 AxNum,\
					u8 UpEn,s32 UpPos,\
					u8 DwEn,s32 DwPos);
//设置回原点模式
//AxNum :轴号
//Mod	:模式
//		0：反向找原点
//		1：先正向找上限位，再反向找原点
//		2：先反向找下限位，再正向找原点
extern s32 HZ_AxSetHomMod(u8 AxNum,u8 Mod);
//AxNum :轴号 AlmLev: 0，1 电平性质  其他：报警口不生效
extern s32 HZ_AxSetAlm(u8 AxNum,u8 AlmLev);
//重新加载运行速度
extern s32 HZ_AxReloadSpd(u8 AxNum,u32 Spd);
//获取当前速度
extern s32 HZ_AxGetSpd(u8 AxNum);

extern void AxDirCfg(u8 Axnum,int val);
extern void AxHome_Motion(u8 n);

/****************回调函数，用户使用时，外部实现***************/
 void ex_inputupdata(void);
 void ex_outputstatusupdata(void);
 void ex_outputupdata(void);

#endif
