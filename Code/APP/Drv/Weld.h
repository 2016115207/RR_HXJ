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

#define INUM	8	//����6������
#define QNUM	8	//��Ҫ����8�����
					//����һ���м�ѹ�������
typedef union {
	u32 Data[10];
	struct 
	{
		float readypos;	//Ԥ��λ��	//4000
		float Pullpos;	//����λ��  //4002
		u32   pushspd;	//ǰ���ٶ�     // 4004
		u32   pullspd;	//�����ٶ�    //4006
		u32   pullCloseT; //�պ�ʱ��  //4008
		u32   pullOpenT;	//�ſ�ʱ�� //4010
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
		u32   CutTime;		//����ʱ�� 4036
		u32   CutOpenTime;	//�����ſ�ʱ�� 4038        
				
	}Para;
} ClampPara;

typedef union {
	u32 Data[10];
	struct 
	{
		u32 WeldingTime;	//����ʱ��  4040
		u32 CuringTime;	//�̻�ʱ��      4042
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
		IODef Q_Push;		//��
	} Out;
	WeldPara  para;
	DrvStatus DrvSta;	//�Լ���״̬��
	struct
	{
		LogicParaDef *PullTask;	 //����
		LogicParaDef *ClampTask; //����
		LogicParaDef *PutTask;	 //����
		LogicParaDef *WeldTask;	 //����
	} Task;
	u8 PullSta;		//����ȡ�� ���� 0����ȡ�� 1ȡ������
	u8 ClampSta;	//����ȡ�� ���� 0����ȡ�� 1ȡ������
	u8 PutSta;		//����ȡ�� ���� 0����ȡ�� 1ȡ������
	u8 WeldingSta;	//����ȡ�� ���� 0����ȡ�� 1ȡ������

} WeldDrv;

extern WeldDrv Weld_drv_L; //�ⲿʹ��ʵ����
extern WeldDrv Weld_drv_R; //�ⲿʹ��ʵ����

extern void WeldLoop(WeldDrv *drv);
extern void WeldInit(WeldDrv* drv,WeldPara  para,LogicParaDef* pull,LogicParaDef* clamp,LogicParaDef* put,LogicParaDef* weld,u8 Axis[3],IODef in[6],IODef out[7]);
extern void WeldConfig(WeldDrv* drv,WeldPara* para);
extern void WeldStart(WeldDrv *drv);
extern void WeldStop(WeldDrv* drv);
extern u8 WeldGetSta(WeldDrv* drv);
extern void WeldClearErr(WeldDrv* drv);
extern void WeldClearSta(WeldDrv* drv);

#endif

