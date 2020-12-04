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
#include "Feed.h" //����

WeldDrv Weld_drv_L; //�ⲿʹ��ʵ����
WeldDrv Weld_drv_R; //�ⲿʹ��ʵ����

#define PT drv->Task.PullTask
#define CT drv->Task.ClampTask
#define WT drv->Task.WeldTask
#define PUT drv->Task.PutTask

//static u8 RunWithoutMask; //�޿��ֿ���

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
//������--�ⲿд��ͣ

//1.���� 
static void Pull(WeldDrv *drv)
{
    u8 Ax = drv->Axis.AxPull;
    INITT(PT)
    switch (PT->step)
    {
    case 1:    //��ǰû������
		if(drv->PullSta == 0 )
		{
			if (IGet(drv->In.I_ClampUp) == ON && IGet(drv->In.I_Weldup) == ON )
			{
				GUR.WeldRunstates = 1;	//���߿�ʼ
				if (GSR.AxisUnitPosition[Ax] != drv->para.pull.Para.Pullpos) //������ȡ��
				{
					MotorMove(Ax, drv->para.pull.Para.pushspd, drv->para.pull.Para.Pullpos, ABSMODE);
				}
				QSet(drv->Out.Q_PressEar, OFF); //Ĭ����ѹס��
				PT->step = 2;
			}
		}
        break;
    case 2:  //��Ŀ��λ��
        if (TAR(Ax, drv->para.pull.Para.Pullpos))
        {
            PT->step = 3;
        }
        else if (HZ_AxGetStatus(Ax) == 0)
        {
            PT->step = 1;
        }
        break;
    case 3:  //ѹ��̧��
        if (IGet(drv->In.I_PressEarDw) == ON)
        {
            QSet(drv->Out.Q_Pull, ON);
            TRST(PT);
            PT->step = 4;
            GUR.WeldRunstates = 2	;//���������
        }
        else
        {
            drv->DrvSta = D_ERR;
            AlarmSetBit(2, 0, 2); //ѹ�������쳣
        }
        break;
    case 4:
        if (TCNT(PT) >= drv->para.pull.Para.pullCloseT)
        {
            QSet(drv->Out.Q_PressEar, ON); //ѹ��̧��
            PT->step = 5;
            GUR.WeldRunstates = 3;	//�ȴ�ѹ��̧��
        }
        break;
    case 5:
        if (IGet(drv->In.I_PressEarUp) == ON)
        {
            MotorMove(Ax, drv->para.pull.Para.pullspd, drv->para.pull.Para.readypos, ABSMODE);
            PT->step = 6;
            GUR.WeldRunstates = 4;	//����������
        }
        break;
    case 6: //���ߵ�λ     ѹס
        if (TAR(Ax, drv->para.pull.Para.readypos))
        {
            QSet(drv->Out.Q_PressEar, OFF);
            PT->step = 7;
            GUR.WeldRunstates = 5;	//�ȴ�ѹ��ѹ��
        }
        else if (HZ_AxGetStatus(Ax) == 0)
        {
            PT->step = 5;
        } else if ( IGet(drv->In.I_PressEarUp) == OFF ) {
            drv->DrvSta = D_ERR;    //��������
            AlarmSetBit(2, 0, 2);
        }
        break;
    case 7: //ѹ��ѹ��
        if (IGet(drv->In.I_PressEarDw) == ON)
        {
            drv->PullSta = 1;        //��������ˣ�צ��������
			PT->step = 1;
        }
        break;
    }
}

//2.����
static void Clamp(WeldDrv *drv)
{
    u8 Ax1 = drv->Axis.AxTurn1;
    u8 Ax2 = drv->Axis.AxTurn2;
    static s32 pulltime;
    INITT(CT)
    switch (CT->step)
    {
    case 1:   //���������� ƽ̨������
        if (drv->PullSta == 1&& Feed_drv.PlatformSta != 0 && drv->ClampSta ==0) 
        {
            GUR.ClampRunstates = 1;
            CT->step = 2;
            QSet(drv->Out.Q_Push,OFF);  //Ĭ��״̬�Ǻ���
            MotorMove(Ax1, drv->para.clamp.Para.workspd[0], drv->para.clamp.Para.readpos[0], ABSMODE);
            MotorMove(Ax2, drv->para.clamp.Para.workspd[1], drv->para.clamp.Para.readpos[1], ABSMODE);
        }
        break;
    case 2: //����ȡ��λ��
        if (HZ_AxGetStatus(Ax2) == 0 && HZ_AxGetStatus(Ax1) == 0&&IGet(drv->In.I_PushBack)==ON)
        {
            QSet(drv->Out.Q_ClampUD, ON);
            CT->step = 3;
            GUR.ClampRunstates = 2;
        }
        break;
    case 3: //�½���λ �պ�צ��
        if (IGet(drv->In.I_ClampDw) == ON)
        {
            QSet(drv->Out.Q_Clamp, ON);
            TRST(CT);
            CT->step = 4;
            GUR.ClampRunstates = 3;
        }
        break;
    case 4: //�պ�ʱ�䵽
        if (TCNT(CT) >= drv->para.clamp.Para.CloseClampT)
        {
            QSet(drv->Out.Q_Cut, ON);   //����
            QSet(drv->Out.Q_Pull, OFF); //���߼���
            pulltime = SysTim_GetUser();
            TRST(CT);
            CT->step = 5;
        }
        break;
    case 5:   //����ʱ�䵽
        if (TCNT(CT) >= drv->para.clamp.Para.CutTime)
        {
            QSet(drv->Out.Q_Cut, OFF);
            TRST(CT);
            CT->step = 6;
        }
        break;
    case 6:   //������������ߵļ���Ҳ����
        if ((SysTim_GetUser() - pulltime / 10) >= drv->para.pull.Para.pullOpenT)
        {
			QSet(drv->Out.Q_ClampUD, OFF);  //��������
			CT->step = 7;
        }
        break;
	case 7: //�ϵ�λ
		if(IGet(drv->In.I_ClampUp)==ON)
        {
			QSet(drv->Out.Q_Push,ON);	//��ǰ��
			CT->step = 8;
		}
		break;
	case 8:
		if(IGet(drv->In.I_PushFront)==ON)
        {   //������ǰ��
            MotorMove(Ax1, drv->para.clamp.Para.workspd[0], drv->para.clamp.Para.workpos[0], ABSMODE);
            MotorMove(Ax2, drv->para.clamp.Para.workspd[1], drv->para.clamp.Para.workpos[1], ABSMODE);
			CT->step = 9;
		}
		break;
	case 9:
		if (TAR(Ax1, drv->para.clamp.Para.workpos[0]) && TAR(Ax2, drv->para.clamp.Para.workpos[1]))
        {
			//ƽ̨�����Ѿ�׼����
			if(Feed_drv.PlatformSta == 2)
			{
				QSet(drv->Out.Q_ClampUD, ON); //��������ѹ
				CT->step = 10;
			}
		}
		break;
	case 10:
		if (IGet(drv->In.I_ClampDw)==ON)
		{
			QSet(drv->Out.Q_Weld, ON); //���������½�
			CT->step = 11;
		}
		break;
	case 11: //���ӵ�λ
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
    case 1:      //�����Ѿ��ź� 
		if(drv->ClampSta == 1)
		{
			drv->PutSta = 0;
			QSet(drv->Out.Q_Clamp, OFF);   //�򿪼��� 
			TRST(CT);
			PUT->step = 2;
		}
        break;
	case 2:
		if (TCNT(CT) >= drv->para.clamp.Para.OpenClampT)
		{
			QSet(drv->Out.Q_ClampUD, OFF); //��������̧ 
			PUT->step = 3;
		}
		break;
	case 3: //���Ӵ�
		if(IGet(drv->In.I_ClampUp)==ON )
        {
			drv->PullSta = 0;	//���߼�����û��������
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
    case 5: //��Ԥ��λ��
        if(IGet(drv->In.I_PushBack)==ON)
        {
            drv->ClampSta = 0; //û�����ϱ���ס��
            PUT->step = 1;
        }
        break;

    }
}

//3.����
static void Welding(WeldDrv *drv)
{
    INITT(WT)
    switch (WT->step)
    {
    case 1: //�������£���������Ҫ��
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
    if (welddrv->DrvSta == D_INIT || welddrv->DrvSta == D_ERR) //δ��ʼ��
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

//���ӿ�ʼ
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

//��ʼ������ֵ
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

//�״�ʹ����Ҫ��ʼ��
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

//��������
void WeldConfig(WeldDrv* drv,WeldPara* para)
{
    if(para->clamp.Para.CloseClampT == 0)
    {
        WeldParaInit(para);
    }
    drv->para = *para;
}
