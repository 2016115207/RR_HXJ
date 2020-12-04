/*
 * @Author: your name
 * @Date: 2020-05-28 18:14:02
 * @LastEditTime: 2020-06-01 12:14:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Drv\Weld.h
 */
#ifndef __WELD_H
#define __WELD_H

#include "LogicData.h"
#include "DrvCom.h"

#define INUM	8	//配置6个输入
#define QNUM	8	//需要配置8个输出
					//还有一个中间压板输出和
typedef union {
	u32 Data[10];
	struct 
	{
		float readypos;	//预备位置	//4000
		float Pullpos;	//后拉位置  //4002
		u32   pushspd;	//前进速度     // 4004
		u32   pullspd;	//后拉速度    //4006
		u32   pullCloseT; //闭合时间  //4008
		u32   pullOpenT;	//张开时间 //4010
	}Para;
} PullPara;
typedef union {
	u32 Data[10];
	struct 
	{
		float readpos[2];     //4020
		float workpos[2];     //4024
		u32   workspd[2];     //4028
		u32   CloseClampT;    //4032
		u32	  OpenClampT;     //4034
		u32   CutTime;		//剪刀时间 4036
		u32   CutOpenTime;	//剪刀张开时间 4038        
				
	}Para;
} ClampPara;

typedef union {
	u32 Data[10];
	struct 
	{
		u32 WeldingTime;	//焊接时间  4040
		u32 CuringTime;	//固化时间      4042
	}Para;
} WeldingPara;
typedef struct
{
	PullPara pull;
	ClampPara clamp;
	WeldingPara welding;
} WeldPara;

typedef struct Weld_type
{
	struct
	{
		u8 AxTurn1;
		u8 AxTurn2;
		u8 AxPull;
	} Axis;
	struct
	{
		IODef I_Weldup;
		IODef I_Welddw;
		IODef I_ClampUp;
		IODef I_ClampDw;
		IODef I_PressEarUp;
		IODef I_PressEarDw;
		IODef I_PushFront;
		IODef I_PushBack;
	} In;
	struct
	{
		IODef Q_Weld;
		IODef Q_ClampUD;
		IODef Q_PressEar;
		IODef Q_Clamp;
		IODef Q_Pull;	
		IODef Q_Cut;
		IODef Q_Ultrasonic;
		IODef Q_Push;		//推
	} Out;
	WeldPara  para;
	DrvStatus DrvSta;	//自己的状态机
	struct
	{
		LogicParaDef *PullTask;	 //拉线
		LogicParaDef *ClampTask; //夹线
		LogicParaDef *PutTask;	 //放线
		LogicParaDef *WeldTask;	 //焊线
	} Task;
	u8 PullSta;		//允许取料 开关 0运行取料 1取到物料
	u8 ClampSta;	//允许取料 开关 0运行取料 1取到物料
	u8 PutSta;		//允许取料 开关 0运行取料 1取到物料
	u8 WeldingSta;	//允许取料 开关 0运行取料 1取到物料

} WeldDrv;

extern WeldDrv Weld_drv_L; //外部使用实体左
extern WeldDrv Weld_drv_R; //外部使用实体右

extern void WeldLoop(WeldDrv *drv);
extern void WeldInit(WeldDrv* drv,WeldPara  para,LogicParaDef* pull,LogicParaDef* clamp,LogicParaDef* put,LogicParaDef* weld,u8 Axis[3],IODef in[6],IODef out[7]);
extern void WeldConfig(WeldDrv* drv,WeldPara* para);
extern void WeldStart(WeldDrv *drv);
extern void WeldStop(WeldDrv* drv);
extern u8 WeldGetSta(WeldDrv* drv);
extern void WeldClearErr(WeldDrv* drv);
extern void WeldClearSta(WeldDrv* drv);

#endif

