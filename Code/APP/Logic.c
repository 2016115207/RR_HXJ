/*
 * @Author: your name
 * @Date: 2020-06-19 14:36:56
 * @LastEditTime: 2020-07-01 14:03:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZH_ZDJ\Code\APP\Logic.c
 */

#include "Logic.h"
#include "math.h"
#include "DataDefine.h"
#include "interlayer_config.h"
#include "FSM.h"
#include "string.h"
#include "USERTOOL.H"
#include "teach.h"
#include "iodefine.h"
#include "AxisMove.h"

/*全局变量做统一处理*/
Task LogicTask = {0}; //统一的任务类
SysData Data = {0};   //统一的数据类

u32 BackTin_L_flag = 0;  //回锡完成标志
u32 BackTin_R_flag = 0;

extern void initIO(void);

//启动的时候调用一次
void run_handle(void)
{
//	LogicTask.DeviceMainTask.execute = 1;
}

/***************
焊线函数
**************/
void Weld(LogicParaDef *Task)
{
    if(GUS.TechPara.Data.Auto_En == 2)  //机器停止状态
    {
        return;
    }

    if(GUS.TechPara.Data.WeldCheckEn ==0) //屏蔽检线开关
    {
        if(InGet(I_TogSw) == OFF) //来料感应
        {
            return;
        }
    }


    INITT(Task)
    switch (Task->step)
    {
    case 1://检测光纤，端子夹夹端子
        if(GUS.TechPara.Data.ClamCheckpEn == 1)
        {
            if(ton(1,InGet(I_TermSig) == ON,10))
            {
                OutSet(Q_Clam,ON); //夹子夹紧
                TRST(Task);
                STEP = 2;
            }
            else
            {
                if(TCNT(Task) > 3000)
                {
                    AlarmSetBit(2, 0, 3);//缺端子报警
                    STEP = 10;
                }
            }
        }
        else
        {
            OutSet(Q_Clam,ON); //夹子夹紧
            TRST(Task);
            STEP = 2;
        }
        break;

    case 2://夹紧延时，翻转气缸翻转，焊笔下压
        if(TCNT(Task) > GUS.TechPara.Data.Q_Clam_ONDelay)
        {
            OutSet(Q_Turn,ON);
            OutSet(Q_WeldDown,ON);
            //第一段预送锡
            if(GUS.TechPara.Data.Feed1_L_En == 1)
            {
                MotorMove(FeedTin_L_Axis,GUS.TechPara.Data.FirstFeedSpeed_L,GUS.TechPara.Data.FirstFeedLength_L,RELMODE);
            }
            if(GUS.TechPara.Data.Feed1_R_En == 1)
            {
                MotorMove(FeedTin_R_Axis,GUS.TechPara.Data.FirstFeedSpeed_R,GUS.TechPara.Data.FirstFeedLength_R,RELMODE); //第一段送锡长度
            }

            TRST(Task);
            STEP = 3;
        }
        break;

    case 3: //翻转延时，焊笔合拢，焊锡压线
        if(TCNT(Task) > GUS.TechPara.Data.Q_Turn_ONDelay)
        {
            OutSet(Q_WeldPress,ON);//焊锡端压线
            TRST(Task);
            STEP = 0xf0;
        }
        break;

    case 0xf0:
        if(TCNT(Task)>=100) //焊锡压线延时 GUS.TechPara.Data.Q_weldPress_ONDelay
        {
            OutSet(Q_WeldMove,ON); //合拢
            TRST(Task);
            STEP = 4;
        }
        break;

    case 4: //合拢延时
        if(TCNT(Task) > GUS.TechPara.Data.Q_WeldMove_ONDelay)
        {
            //送锡
            if(HZ_AxGetStatus(1)==AXSTA_READY && HZ_AxGetStatus(1)==AXSTA_READY) //保证第一段送锡完成
            {
                if(GUS.TechPara.Data.Feed2_L_En == 1)
                    MotorMove(FeedTin_L_Axis,GUS.TechPara.Data.SecondFeedSpeed_L,GUS.TechPara.Data.SecondFeedLength_L,RELMODE);
                if(GUS.TechPara.Data.Feed2_R_En == 1)
                    MotorMove(FeedTin_R_Axis,GUS.TechPara.Data.SecondFeedSpeed_R,GUS.TechPara.Data.SecondFeedLength_R,RELMODE); //第2段送锡长度

                TRST(Task);
                STEP = 5;
            }

        }
        break;

    case 5: //焊接延时后焊笔抬起
        if(TCNT(Task) > GUS.TechPara.Data.WeldDelay)
        {
            if(GUS.TechPara.Data.WeldMode == 0) //先抬起张开
            {
                OutSet(Q_WeldDown,OFF);
                TRST(Task);
                STEP = 6;
            }
            else if(GUS.TechPara.Data.WeldMode == 1) //先张开抬起
            {
                OutSet(Q_WeldBlow,ON);//冷却吹气
                OutSet(Q_WeldMove,OFF);
                TRST(Task);
                STEP = 0xa0;
            }

        }
        break;

    /*************焊接模式2**********/
    case 0xa0:
        if(TCNT(Task) > GUS.TechPara.Data.ColdDelay)
        {
            OutSet(Q_WeldBlow,OFF);//冷却吹气关
//            OutSet(Q_WeldMove,OFF);//张开
            OutSet(Q_Clam,OFF);//张开
            TRST(Task);
            STEP = 0xa1;
        }
        break;

    case 0xa1:
        if(TCNT(Task) >= 50) //张开延时
        {
            OutSet(Q_WeldDown,OFF);
            TRST(Task);
            STEP = 6;
        }
        break;


    case 6: //抬起时间到
        if(TCNT(Task) > GUS.TechPara.Data.Q_WeldDown_OFFDelay)
        {
            if(GUS.TechPara.Data.WeldMode == 0)
                OutSet(Q_WeldBlow,ON);//冷却吹气
            TRST(Task);
            STEP = 7;
        }
        break;

    case 7: //冷却时间
        if(GUS.TechPara.Data.WeldMode == 0)
        {
            if(TCNT(Task) >= GUS.TechPara.Data.ColdDelay)
            {
                OutSet(Q_WeldBlow,OFF);//冷却吹气关
                TRST(Task);
                STEP = 0xf1;
            }

        }
        else
        {
            TRST(Task);
            STEP = 0xf1;
        }
        break;

    case 0xf1:
        if(TCNT(Task) >= GUS.TechPara.Data.BackTinDelay_L && BackTin_L_flag == 0)
        {
            MotorMove(FeedTin_L_Axis,GUS.TechPara.Data.SecondFeedSpeed_L,-GUS.TechPara.Data.BackLength_L,RELMODE);
            BackTin_L_flag = 1;
        }
        if(TCNT(Task) >= GUS.TechPara.Data.BackTinDelay_R && BackTin_R_flag == 0)
        {
            //锡丝回退距离
            MotorMove(FeedTin_R_Axis,GUS.TechPara.Data.SecondFeedSpeed_R,-GUS.TechPara.Data.BackLength_R,RELMODE);
            BackTin_R_flag = 1;
        }
        if(BackTin_R_flag == 1 && BackTin_L_flag == 1)
        {
            BackTin_L_flag = 0;
            BackTin_R_flag = 0;
            STEP = 8;
        }
        break;
    case 8: //夹子张开
        if (1)
        {
            OutSet(Q_Clam,OFF);
            TRST(Task);
            STEP = 9;
        }
        break;

    case 9: //张开延时后，焊笔张开，压线抬起
        if(TCNT(Task) > 30 ) //GUS.TechPara.Data.Q_Clam_OFFDelay
        {
            OutSet(Q_Turn,OFF);
            OutSet(Q_WeldMove,OFF);
            OutSet(Q_WeldPress,OFF);
            TRST(Task);
            STEP = 10;
        }
        break;

    case 10: //结束
        if(TCNT(Task) > 30)  //GUS.TechPara.Data.Q_Turn_OFFDelay
        {
            PARAINIT(*Task);
        }
        break;

    }
}

/*************
分线剥线函数
*************/
void DivideWire(LogicParaDef *Task)
{
    if(GUS.TechPara.Data.Auto_En == 2)  //机器停止状态
    {
        return;
    }

    INITT(Task)
    switch (Task->step)
    {
    case 1: //链条移动到位压线
        if (HZ_AxGetStatus(BeltAxis) == AXSTA_READY)
        {
            OutSet(Q_FrontPress,ON);
            TRST(Task);
            STEP = 2;
        }
        break;

    case 2://压线延时到，剥刀气缸合拢
        if (TCNT(Task) > GUS.TechPara.Data.Q_FrontPress_ONDelay)
        {
            OutSet(Q_TakeLine,ON); //分线前伸
            TRST(Task);
            STEP = 3;
        }
        break;

    case 3://前伸延时
        if (TCNT(Task) > GUS.TechPara.Data.Q_YDivide_ONDelay)
        {
            OutSet(Q_TakeLineTop,ON); //顶块向上
            STEP = 4;
        }
        break;

    case 4: //半剥下
        if (1)
        {
            OutSet(Q_ScripFront,ON);
            TRST(Task);
            STEP = 5;
        }
        break;

    case 5: //延时到，向后剥线
        if (TCNT(Task) > 200)
        {
            OutSet(Q_TakeLine,OFF);
            TRST(Task);
            STEP = 6;
        }
        break;


    case 6: //剥线后退延时到
        if (TCNT(Task) > GUS.TechPara.Data.Q_DivivdeCut_OFFDelay)
        {
            OutSet(Q_TakeLineTop,OFF);  //顶块回位
            OutSet(Q_ScripFront,OFF);   //半剥切刀升起
            OutSet(Q_ScripBlow,ON); //直线吹气
            TRST(Task);
            STEP = 7;
        }
        break;


    case 7: //
        if(TCNT(Task) >= GUS.TechPara.Data.Q_FrontPress_OFDelay) //延时到
        {
            OutSet(Q_ScripBlow,OFF);
            OutSet(Q_FrontPress,OFF);   //前排压线升起
            STEP = 8;
        }
        break;

    case 8:
        if(InGet(I_PressAlm) == ON) //延时到
        {
            PARAINIT(*Task);
        }

        break;
    }

}

//设备主流程
void DeviceMain(LogicParaDef* Task)
{
    if(GUS.TechPara.Data.Auto_En == 2)  //机器停止状态
    {
        return;
    }

    if(InGet(I_Check_1) == ON)  //传送带缺端子启动振动盘
    {
        OutSet(Q_VibDisk,ON);
    }
    else
    {
        OutSet(Q_VibDisk,OFF);
    }

    //检测到脚踏信号
    if(ton(2,InGet(I_Start) == ON,10) && GUS.TechPara.Data.Auto_En == 1)  //自动运行脚踏才有效
    {
        Task->execute = 1;
    }

    //联动开启
    if(GUS.TechPara.Data.Link_En ==1 && GUS.TechPara.Data.Auto_En == 1 && Task->execute == 0)
    {
        Task->execute = 1;
    }

    INITT(Task)
    switch (Task->step)
    {
    case 1: //链条送线一格
        if (HZ_AxGetStatus(BeltAxis) == 0 && InGet(I_PressAlm) == ON && InGet(I_WeldAlm) == ON)
        {
            MotorMove(BeltAxis,GUS.TechPara.Data.BeltSpeed,GUS.TechPara.Data.BeltLength,RELMODE);
            TRST(Task);
            STEP = 0xf0;
        }
        else if(TCNT(Task) >= 3000) //超时报警
        {
            if(InGet(I_PressAlm) == OFF)
            {
                AlarmSetBit(2, 0, 1);//前排压线报警
            }
            if(InGet(I_WeldAlm) == OFF)
            {
                AlarmSetBit(2, 0, 2);//焊线压线报警
            }
            STEP = ENDCASE;
        }
        break;

    case 0xf0:
        if(Triger(6,ON,InGet(I_BeltSig), 10))   //TCNT(Task)>=200&& InGet(I_BeltSig) == ON
        {
            HZ_AxStop(BeltAxis);
            STEP = 2;
        }
        break;

    case 2: //各个工位开始工作
        if (HZ_AxGetStatus(BeltAxis) == 0)
        {
            //分线半剥工位
            //if(GUS.TechPara.Data.HalfScripEn == 1 && InGet(I_PressAlm) == ON && InGet(I_WeldAlm) == ON)
            {
                LogicTask.SeparateWire.execute = 1;
            }

            //焊锡工位
            if(GUS.TechPara.Data.WeldEn==1)
            {
                if(GUS.TechPara.Data.WeldCheckEn == 1 || ton(3,InGet(I_TogSw) == ON,10))
                {
                    LogicTask.Weld.execute = 1;
                }
            }

            STEP = ENDCASE;
        }
        break;

    case ENDCASE: //
        if(LogicTask.Weld.execute ==0 && LogicTask.SeparateWire.execute == 0)
        {
            GUS.TechPara.Data.Product += 1;
            PARAINIT(*Task);
        }
        break;
    }

}




void Logic()
{
    if (Triger(4,ON,InGet(I_Start), 5))
    {
//		GUW.Button.RunCommand = RUN_EVENT;
    }



    /******工程运行逻辑*******/
    DeviceMain(&LogicTask.DeviceMainTask);
    DivideWire(&LogicTask.SeparateWire); //分线剥线
    Weld(&LogicTask.Weld); //焊线
}

s32 Product(u32 Sum,u32 num)
{
    return 0;
}


