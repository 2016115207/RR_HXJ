/*
 * @Author: your name
 * @Date: 2020-05-28 18:13:56
 * @LastEditTime: 2020-06-04 19:01:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Drv\Weld.c
 */
#include "Weld.h"
#include "Interlayer_config.h"
#include "DataDefine.h"
#include "USERTOOL.H"
#include "AxisMove.h"
#include "Feed.h" //进料

WeldDrv Weld_drv_L; //外部使用实体左
WeldDrv Weld_drv_R; //外部使用实体右

#define PT drv->Task.PullTask
#define CT drv->Task.ClampTask
#define WT drv->Task.WeldTask
#define PUT drv->Task.PutTask

//static u8 RunWithoutMask; //无口罩空跑

s32 IGet(IODef io)
{
    return InGet(io.card, io.num);
}
s32 QGet(IODef io)
{
    return OutGet(io.card, io.num);
}
s32 QSet(IODef io, u8 val)
{
    return OutSet(io.card, io.num, val);
}
//待处理--外部写急停

//1.拉线 
static void Pull(WeldDrv *drv)
{
    u8 Ax = drv->Axis.AxPull;
    INITT(PT)
    switch (PT->step)
    {
    case 1:    //当前没有线了
		if(drv->PullSta == 0 )
		{
			if (IGet(drv->In.I_ClampUp) == ON && IGet(drv->In.I_Weldup) == ON )
			{
				GUR.WeldRunstates = 1;	//拉线开始
				if (GSR.AxisUnitPosition[Ax] != drv->para.pull.Para.Pullpos) //拉线轴取线
				{
					MotorMove(Ax, drv->para.pull.Para.pushspd, drv->para.pull.Para.Pullpos, ABSMODE);
				}
				QSet(drv->Out.Q_PressEar, OFF); //默认是压住的
				PT->step = 2;
			}
		}
        break;
    case 2:  //到目标位置
        if (TAR(Ax, drv->para.pull.Para.Pullpos))
        {
            PT->step = 3;
        }
        else if (HZ_AxGetStatus(Ax) == 0)
        {
            PT->step = 1;
        }
        break;
    case 3:  //压耳抬起
        if (IGet(drv->In.I_PressEarDw) == ON)
        {
            QSet(drv->Out.Q_Pull, ON);
            TRST(PT);
            PT->step = 4;
            GUR.WeldRunstates = 2	;//拉线轴夹线
        }
        else
        {
            drv->DrvSta = D_ERR;
            AlarmSetBit(2, 0, 2); //压耳气缸异常
        }
        break;
    case 4:
        if (TCNT(PT) >= drv->para.pull.Para.pullCloseT)
        {
            QSet(drv->Out.Q_PressEar, ON); //压耳抬起
            PT->step = 5;
            GUR.WeldRunstates = 3;	//等待压耳抬起
        }
        break;
    case 5:
        if (IGet(drv->In.I_PressEarUp) == ON)
        {
            MotorMove(Ax, drv->para.pull.Para.pullspd, drv->para.pull.Para.readypos, ABSMODE);
            PT->step = 6;
            GUR.WeldRunstates = 4;	//拉线往后走
        }
        break;
    case 6: //拉线到位     压住
        if (TAR(Ax, drv->para.pull.Para.readypos))
        {
            QSet(drv->Out.Q_PressEar, OFF);
            PT->step = 7;
            GUR.WeldRunstates = 5;	//等待压耳压紧
        }
        else if (HZ_AxGetStatus(Ax) == 0)
        {
            PT->step = 5;
        } else if ( IGet(drv->In.I_PressEarUp) == OFF ) {
            drv->DrvSta = D_ERR;    //保护拉线
            AlarmSetBit(2, 0, 2);
        }
        break;
    case 7: //压耳压紧
        if (IGet(drv->In.I_PressEarDw) == ON)
        {
            drv->PullSta = 1;        //拉线完成了，爪子上有线
			PT->step = 1;
        }
        break;
    }
}

//2.夹线
static void Clamp(WeldDrv *drv)
{
    u8 Ax1 = drv->Axis.AxTurn1;
    u8 Ax2 = drv->Axis.AxTurn2;
    static s32 pulltime;
    INITT(CT)
    switch (CT->step)
    {
    case 1:   //拉线上有线 平台上有料
        if (drv->PullSta == 1&& Feed_drv.PlatformSta != 0 && drv->ClampSta ==0) 
        {
            GUR.ClampRunstates = 1;
            CT->step = 2;
            QSet(drv->Out.Q_Push,OFF);  //默认状态是后面
            MotorMove(Ax1, drv->para.clamp.Para.workspd[0], drv->para.clamp.Para.readpos[0], ABSMODE);
            MotorMove(Ax2, drv->para.clamp.Para.workspd[1], drv->para.clamp.Para.readpos[1], ABSMODE);
        }
        break;
    case 2: //到达取线位置
        if (HZ_AxGetStatus(Ax2) == 0 && HZ_AxGetStatus(Ax1) == 0&&IGet(drv->In.I_PushBack)==ON)
        {
            QSet(drv->Out.Q_ClampUD, ON);
            CT->step = 3;
            GUR.ClampRunstates = 2;
        }
        break;
    case 3: //下降到位 闭合爪子
        if (IGet(drv->In.I_ClampDw) == ON)
        {
            QSet(drv->Out.Q_Clamp, ON);
            TRST(CT);
            CT->step = 4;
            GUR.ClampRunstates = 3;
        }
        break;
    case 4: //闭合时间到
        if (TCNT(CT) >= drv->para.clamp.Para.CloseClampT)
        {
            QSet(drv->Out.Q_Cut, ON);   //剪刀
            QSet(drv->Out.Q_Pull, OFF); //拉线夹松
            pulltime = SysTim_GetUser();
            TRST(CT);
            CT->step = 5;
        }
        break;
    case 5:   //剪线时间到
        if (TCNT(CT) >= drv->para.clamp.Para.CutTime)
        {
            QSet(drv->Out.Q_Cut, OFF);
            TRST(CT);
            CT->step = 6;
        }
        break;
    case 6:   //夹线完成且拉线的夹子也开了
        if ((SysTim_GetUser() - pulltime / 10) >= drv->para.pull.Para.pullOpenT)
        {
			QSet(drv->Out.Q_ClampUD, OFF);  //夹子上行
			CT->step = 7;
        }
        break;
	case 7: //上到位
		if(IGet(drv->In.I_ClampUp)==ON)
        {
			QSet(drv->Out.Q_Push,ON);	//往前推
			CT->step = 8;
		}
		break;
	case 8:
		if(IGet(drv->In.I_PushFront)==ON)
        {   //步进往前走
            MotorMove(Ax1, drv->para.clamp.Para.workspd[0], drv->para.clamp.Para.workpos[0], ABSMODE);
            MotorMove(Ax2, drv->para.clamp.Para.workspd[1], drv->para.clamp.Para.workpos[1], ABSMODE);
			CT->step = 9;
		}
		break;
	case 9:
		if (TAR(Ax1, drv->para.clamp.Para.workpos[0]) && TAR(Ax2, drv->para.clamp.Para.workpos[1]))
        {
			//平台物料已经准备好
			if(Feed_drv.PlatformSta == 2)
			{
				QSet(drv->Out.Q_ClampUD, ON); //夹子往下压
				CT->step = 10;
			}
		}
		break;
	case 10:
		if (IGet(drv->In.I_ClampDw)==ON)
		{
			QSet(drv->Out.Q_Weld, ON); //焊接气缸下降
			CT->step = 11;
		}
		break;
	case 11: //焊接到位
		if(IGet(drv->In.I_Welddw)==ON)
		{
			drv->ClampSta = 1;
			drv->Task.WeldTask->execute = 1;
			drv->WeldingSta = 0;
			CT->step = 1;
		}
		break;
    }

}


static void Put(WeldDrv* drv)
{
	u8 Ax1 = drv->Axis.AxTurn1;
    u8 Ax2 = drv->Axis.AxTurn2;
    INITT(PUT)
    switch(PUT->step)
    {
    case 1:      //物料已经放好 
		if(drv->ClampSta == 1)
		{
			drv->PutSta = 0;
			QSet(drv->Out.Q_Clamp, OFF);   //打开夹子 
			TRST(CT);
			PUT->step = 2;
		}
        break;
	case 2:
		if (TCNT(CT) >= drv->para.clamp.Para.OpenClampT)
		{
			QSet(drv->Out.Q_ClampUD, OFF); //夹子往上抬 
			PUT->step = 3;
		}
		break;
	case 3: //夹子打开
		if(IGet(drv->In.I_ClampUp)==ON )
        {
			drv->PullSta = 0;	//拉线夹子上没有物料了
            MotorMove(Ax1, drv->para.clamp.Para.workspd[0], drv->para.clamp.Para.readpos[0], ABSMODE);
            MotorMove(Ax2, drv->para.clamp.Para.workspd[1], drv->para.clamp.Para.readpos[1], ABSMODE);
			PUT->step = 4;
		}
		break;
	case 4:
		if (HZ_AxGetStatus(Ax2) == 0 && HZ_AxGetStatus(Ax1) == 0)
		{
			QSet(drv->Out.Q_Push,OFF);
			PUT->step = 5;
		}
		break;
    case 5: //到预备位等
        if(IGet(drv->In.I_PushBack)==ON)
        {
            drv->ClampSta = 0; //没有物料被夹住了
            PUT->step = 1;
        }
        break;

    }
}

//3.焊线
static void Welding(WeldDrv *drv)
{
    INITT(WT)
    switch (WT->step)
    {
    case 1: //焊线在下，且有物料要焊
        if (IGet(drv->In.I_Welddw) == ON )
        {
            QSet(drv->Out.Q_Ultrasonic,ON);
            TRST(WT);
            WT->step = 2;
        }
        break;
    case 2:
        if (TCNT(WT) >= drv->para.welding.Para.WeldingTime)
        {
            QSet(drv->Out.Q_Ultrasonic,OFF);
            TRST(WT);
            WT->step = 3;
        }
        break;
    case 3:
        if (TCNT(CT) >= drv->para.welding.Para.CuringTime)
        {
            QSet(drv->Out.Q_Weld, OFF);
            WT->step = 4;
        }
        break;
    case 4:
        if (IGet(drv->In.I_Weldup) == ON)
        {
			drv->WeldingSta = 1;
            WT->step = 0;
            WT->execute = 0;
        }
        break;
    }
}

void WeldLoop(WeldDrv *welddrv)
{
    if (welddrv->DrvSta == D_INIT || welddrv->DrvSta == D_ERR) //未初始化
    {
        return;
    }
    if(welddrv->DrvSta == D_RUN)
    {
        Pull(welddrv);
        Clamp(welddrv);
		Put(welddrv);
    }
    Welding(welddrv);
}

//焊接开始
void WeldStart(WeldDrv *drv)
{
    if (drv->DrvSta == D_REDY)
    {
        drv->DrvSta = D_RUN;
        drv->Task.PullTask->execute = 1;
		drv->Task.ClampTask->execute = 1;
		drv->Task.PutTask->execute = 1;
    }
}


void WeldStop(WeldDrv* drv)
{
    if(drv->DrvSta == D_RUN)
    {
        drv->DrvSta = D_REDY;
    }
}


u8 WeldGetSta(WeldDrv* drv)
{
    return drv->DrvSta;
}

void WeldClearErr(WeldDrv* drv)
{
    drv->DrvSta = D_REDY;
}

void WeldClearSta(WeldDrv* drv)
{
	drv->PullSta = 0;
	drv->PutSta = 0;
	drv->WeldingSta = 0;
}

//初始化数据值
static void WeldParaInit(WeldPara* initdata)
{
    initdata->clamp.Para.CloseClampT = 1000;
    initdata->clamp.Para.OpenClampT = 1000;
    initdata->clamp.Para.CutTime = 1000;
    initdata->clamp.Para.CutOpenTime = 1000;
    initdata->clamp.Para.readpos[0] = 0;
    initdata->clamp.Para.readpos[1] = 0;
    initdata->clamp.Para.workpos[0] = 4;
    initdata->clamp.Para.workpos[1] = 4;
    initdata->clamp.Para.workspd[0] = 10;
    initdata->clamp.Para.workspd[1] = 10;

    initdata->pull.Para.pullCloseT = 1000;
    initdata->pull.Para.pullOpenT = 1000;
    initdata->pull.Para.Pullpos = 40;
    initdata->pull.Para.pullspd = 10;
    initdata->pull.Para.pushspd = 10;
    initdata->pull.Para.readypos = 0;

    initdata->welding.Para.CuringTime = 1000;
    initdata->welding.Para.WeldingTime = 1000;

}

//首次使用需要初始化
void WeldInit(WeldDrv* drv,WeldPara  para,LogicParaDef* pull,LogicParaDef* clamp,LogicParaDef* put,LogicParaDef* weld,u8 Axis[3],IODef in[6],IODef out[7])
{
    if(drv->DrvSta == D_INIT)
    {
        drv->DrvSta = D_REDY;
    }
    drv->para = para;
    drv->Task.PullTask = pull;
    drv->Task.ClampTask = clamp;
    drv->Task.WeldTask = weld;
	drv->Task.PutTask = put;
    drv->Axis.AxPull = Axis[0];
    drv->Axis.AxTurn1 = Axis[1];
    drv->Axis.AxTurn2 = Axis[2];
    memcpy(&(drv->In),in,sizeof(IODef)*INUM);
    memcpy(&(drv->Out),out,sizeof(IODef)*QNUM);

}

//参数配置
void WeldConfig(WeldDrv* drv,WeldPara* para)
{
    if(para->clamp.Para.CloseClampT == 0)
    {
        WeldParaInit(para);
    }
    drv->para = *para;
}
