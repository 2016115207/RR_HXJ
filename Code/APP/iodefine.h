/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-07-01 11:45:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\iodefine.h
 */
#ifndef __IODEFINE_H
#define __IODEFINE_H
typedef enum //输出口定义
{
	X1,
	X2,
	X3,
	X4,
	X5,
	X6,
	X7,
	X8,
	X9,
	X10,
	X11,
	X12,
	X13,
	X14,
	X15,
	X16,
	X17,
	X18,
	X19,
	X20,
} OutNum;

typedef enum //输入口定义
{
	Y1,
	Y2,
	Y3,
	Y4,
	Y5,
	Y6,
	Y7,
	Y8,
	Y9,
	Y10,
	Y11,
	Y12,
	Y13,
	Y14,
	Y15,
	Y16,
	Y17,
} InPutNum;

typedef enum
{
	EX1,
	EX2,
	EX3,
	EX4,
	EX5,
	EX6,
	EX7,
	EX8,
	EX9,
	EX10,
	EX11,
	EX12,
	EX13,
	EX14,
	EX15,
	EX16,
	EX17,
	EX18,
	EX19,
	EX20,
	EX21,
	EX22,
	EX23,
	EX24,
	EX25,
	EX26,
	EX27,
	EX28,
	EX29,
	EX30,
	EX31,
	EX32
} ExInPutNum;
typedef enum
{
	EY1,
	EY2,
	EY3,
	EY4,
	EY5,
	EY6,
	EY7,
	EY8,
	EY9,
	EY10,
	EY11,
	EY12,
	EY13,
	EY14,
	EY15,
	EY16,
	EY17,
	EY18,
	EY19,
	EY20,
	EY21,
	EY22,
	EY23,
	EY24,
	EY25,
	EY26,
	EY27,
	EY28,
	EY29,
	EY30,
	EY31,
	EY32
} ExOutPutNum;
#define CARD1 0
#define CARD2 1
#define CARD3 2

/******************输入定义*****************************/
#define	I_Start					CARD1,X1		//	脚踏开关
#define	I_BeltSig				CARD1,X2		//	链条感应  
#define	I_TogSw					CARD1,X3		//	拨动开关
#define	I_Check_1				CARD1,X4		//	对射电眼1 （端子传送代带料感应）
#define	I_PressAlm				CARD1,X5		//	前排压线报警（压线上感应）
#define	I_WeldAlm				CARD1,X6		//	焊锡报警 （焊锡端压线上感应）
#define	I_TermSig				CARD1,X10		//	端子感应 （端子夹空感应）


/****************输出***************/
#define	Q_VibDisk				CARD1,Y1 		//	振动盘
#define	Q_FrontPress			CARD1,Y2 		//	前排压线
#define	Q_TakeLine				CARD1,Y4 		//	拉线前后
#define	Q_TakeLineTop			CARD1,Y6 		//	拉线顶块
#define	Q_ScripFront			CARD1,Y7 		//	半剥上下
#define	Q_WeldDown				CARD1,Y11 		//	烙铁上下
#define	Q_WeldMove				CARD1,Y12 		//	烙铁合拢（前后）
#define	Q_Clam					CARD1,Y13 		//	取端子夹
#define	Q_Turn					CARD1,Y14 		//	取端子翻转
#define	Q_ScripBlow				CARD1,Y15		//	剥线吹气
#define	Q_WeldBlow				CARD1,Y16		//	焊线吹气
#define	Q_WeldPress				CARD1,Y17		//	焊线前排

#endif
