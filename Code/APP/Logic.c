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

/*ȫ�ֱ�����ͳһ����*/
Task LogicTask = {0}; //ͳһ��������
SysData Data = {0};   //ͳһ��������

u32 BackTin_L_flag = 0;  //������ɱ�־
u32 BackTin_R_flag = 0;

extern void initIO(void);

//������ʱ�����һ��
void run_handle(void)
{
//	LogicTask.DeviceMainTask.execute = 1;
}

/***************
���ߺ���
**************/
void Weld(LogicParaDef *Task)
{
    if(GUS.TechPara.Data.Auto_En == 2)  //����ֹͣ״̬
    {
        return;
    }

    if(GUS.TechPara.Data.WeldCheckEn ==0) //���μ��߿���
    {
        if(InGet(I_TogSw) == OFF) //���ϸ�Ӧ
        {
            return;
        }
    }


    INITT(Task)
    switch (Task->step)
    {
    case 1://�����ˣ����Ӽмж���
        if(GUS.TechPara.Data.ClamCheckpEn == 1)
        {
            if(ton(1,InGet(I_TermSig) == ON,10))
            {
                OutSet(Q_Clam,ON); //���Ӽн�
                TRST(Task);
                STEP = 2;
            }
            else
            {
                if(TCNT(Task) > 3000)
                {
                    AlarmSetBit(2, 0, 3);//ȱ���ӱ���
                    STEP = 10;
                }
            }
        }
        else
        {
            OutSet(Q_Clam,ON); //���Ӽн�
            TRST(Task);
            STEP = 2;
        }
        break;

    case 2://�н���ʱ����ת���׷�ת��������ѹ
        if(TCNT(Task) > GUS.TechPara.Data.Q_Clam_ONDelay)
        {
            OutSet(Q_Turn,ON);
            OutSet(Q_WeldDown,ON);
            //��һ��Ԥ����
            if(GUS.TechPara.Data.Feed1_L_En == 1)
            {
                MotorMove(FeedTin_L_Axis,GUS.TechPara.Data.FirstFeedSpeed_L,GUS.TechPara.Data.FirstFeedLength_L,RELMODE);
            }
            if(GUS.TechPara.Data.Feed1_R_En == 1)
            {
                MotorMove(FeedTin_R_Axis,GUS.TechPara.Data.FirstFeedSpeed_R,GUS.TechPara.Data.FirstFeedLength_R,RELMODE); //��һ����������
            }

            TRST(Task);
            STEP = 3;
        }
        break;

    case 3: //��ת��ʱ�����ʺ�£������ѹ��
        if(TCNT(Task) > GUS.TechPara.Data.Q_Turn_ONDelay)
        {
            OutSet(Q_WeldPress,ON);//������ѹ��
            TRST(Task);
            STEP = 0xf0;
        }
        break;

    case 0xf0:
        if(TCNT(Task)>=100) //����ѹ����ʱ GUS.TechPara.Data.Q_weldPress_ONDelay
        {
            OutSet(Q_WeldMove,ON); //��£
            TRST(Task);
            STEP = 4;
        }
        break;

    case 4: //��£��ʱ
        if(TCNT(Task) > GUS.TechPara.Data.Q_WeldMove_ONDelay)
        {
            //����
            if(HZ_AxGetStatus(1)==AXSTA_READY && HZ_AxGetStatus(1)==AXSTA_READY) //��֤��һ���������
            {
                if(GUS.TechPara.Data.Feed2_L_En == 1)
                    MotorMove(FeedTin_L_Axis,GUS.TechPara.Data.SecondFeedSpeed_L,GUS.TechPara.Data.SecondFeedLength_L,RELMODE);
                if(GUS.TechPara.Data.Feed2_R_En == 1)
                    MotorMove(FeedTin_R_Axis,GUS.TechPara.Data.SecondFeedSpeed_R,GUS.TechPara.Data.SecondFeedLength_R,RELMODE); //��2����������

                TRST(Task);
                STEP = 5;
            }

        }
        break;

    case 5: //������ʱ�󺸱�̧��
        if(TCNT(Task) > GUS.TechPara.Data.WeldDelay)
        {
            if(GUS.TechPara.Data.WeldMode == 0) //��̧���ſ�
            {
                OutSet(Q_WeldDown,OFF);
                TRST(Task);
                STEP = 6;
            }
            else if(GUS.TechPara.Data.WeldMode == 1) //���ſ�̧��
            {
                OutSet(Q_WeldBlow,ON);//��ȴ����
                OutSet(Q_WeldMove,OFF);
                TRST(Task);
                STEP = 0xa0;
            }

        }
        break;

    /*************����ģʽ2**********/
    case 0xa0:
        if(TCNT(Task) > GUS.TechPara.Data.ColdDelay)
        {
            OutSet(Q_WeldBlow,OFF);//��ȴ������
//            OutSet(Q_WeldMove,OFF);//�ſ�
            OutSet(Q_Clam,OFF);//�ſ�
            TRST(Task);
            STEP = 0xa1;
        }
        break;

    case 0xa1:
        if(TCNT(Task) >= 50) //�ſ���ʱ
        {
            OutSet(Q_WeldDown,OFF);
            TRST(Task);
            STEP = 6;
        }
        break;


    case 6: //̧��ʱ�䵽
        if(TCNT(Task) > GUS.TechPara.Data.Q_WeldDown_OFFDelay)
        {
            if(GUS.TechPara.Data.WeldMode == 0)
                OutSet(Q_WeldBlow,ON);//��ȴ����
            TRST(Task);
            STEP = 7;
        }
        break;

    case 7: //��ȴʱ��
        if(GUS.TechPara.Data.WeldMode == 0)
        {
            if(TCNT(Task) >= GUS.TechPara.Data.ColdDelay)
            {
                OutSet(Q_WeldBlow,OFF);//��ȴ������
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
            //��˿���˾���
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
    case 8: //�����ſ�
        if (1)
        {
            OutSet(Q_Clam,OFF);
            TRST(Task);
            STEP = 9;
        }
        break;

    case 9: //�ſ���ʱ�󣬺����ſ���ѹ��̧��
        if(TCNT(Task) > 30 ) //GUS.TechPara.Data.Q_Clam_OFFDelay
        {
            OutSet(Q_Turn,OFF);
            OutSet(Q_WeldMove,OFF);
            OutSet(Q_WeldPress,OFF);
            TRST(Task);
            STEP = 10;
        }
        break;

    case 10: //����
        if(TCNT(Task) > 30)  //GUS.TechPara.Data.Q_Turn_OFFDelay
        {
            PARAINIT(*Task);
        }
        break;

    }
}

/*************
���߰��ߺ���
*************/
void DivideWire(LogicParaDef *Task)
{
    if(GUS.TechPara.Data.Auto_En == 2)  //����ֹͣ״̬
    {
        return;
    }

    INITT(Task)
    switch (Task->step)
    {
    case 1: //�����ƶ���λѹ��
        if (HZ_AxGetStatus(BeltAxis) == AXSTA_READY)
        {
            OutSet(Q_FrontPress,ON);
            TRST(Task);
            STEP = 2;
        }
        break;

    case 2://ѹ����ʱ�����������׺�£
        if (TCNT(Task) > GUS.TechPara.Data.Q_FrontPress_ONDelay)
        {
            OutSet(Q_TakeLine,ON); //����ǰ��
            TRST(Task);
            STEP = 3;
        }
        break;

    case 3://ǰ����ʱ
        if (TCNT(Task) > GUS.TechPara.Data.Q_YDivide_ONDelay)
        {
            OutSet(Q_TakeLineTop,ON); //��������
            STEP = 4;
        }
        break;

    case 4: //�����
        if (1)
        {
            OutSet(Q_ScripFront,ON);
            TRST(Task);
            STEP = 5;
        }
        break;

    case 5: //��ʱ����������
        if (TCNT(Task) > 200)
        {
            OutSet(Q_TakeLine,OFF);
            TRST(Task);
            STEP = 6;
        }
        break;


    case 6: //���ߺ�����ʱ��
        if (TCNT(Task) > GUS.TechPara.Data.Q_DivivdeCut_OFFDelay)
        {
            OutSet(Q_TakeLineTop,OFF);  //�����λ
            OutSet(Q_ScripFront,OFF);   //����е�����
            OutSet(Q_ScripBlow,ON); //ֱ�ߴ���
            TRST(Task);
            STEP = 7;
        }
        break;


    case 7: //
        if(TCNT(Task) >= GUS.TechPara.Data.Q_FrontPress_OFDelay) //��ʱ��
        {
            OutSet(Q_ScripBlow,OFF);
            OutSet(Q_FrontPress,OFF);   //ǰ��ѹ������
            STEP = 8;
        }
        break;

    case 8:
        if(InGet(I_PressAlm) == ON) //��ʱ��
        {
            PARAINIT(*Task);
        }

        break;
    }

}

//�豸������
void DeviceMain(LogicParaDef* Task)
{
    if(GUS.TechPara.Data.Auto_En == 2)  //����ֹͣ״̬
    {
        return;
    }

    if(InGet(I_Check_1) == ON)  //���ʹ�ȱ������������
    {
        OutSet(Q_VibDisk,ON);
    }
    else
    {
        OutSet(Q_VibDisk,OFF);
    }

    //��⵽��̤�ź�
    if(ton(2,InGet(I_Start) == ON,10) && GUS.TechPara.Data.Auto_En == 1)  //�Զ����н�̤����Ч
    {
        Task->execute = 1;
    }

    //��������
    if(GUS.TechPara.Data.Link_En ==1 && GUS.TechPara.Data.Auto_En == 1 && Task->execute == 0)
    {
        Task->execute = 1;
    }

    INITT(Task)
    switch (Task->step)
    {
    case 1: //��������һ��
        if (HZ_AxGetStatus(BeltAxis) == 0 && InGet(I_PressAlm) == ON && InGet(I_WeldAlm) == ON)
        {
            MotorMove(BeltAxis,GUS.TechPara.Data.BeltSpeed,GUS.TechPara.Data.BeltLength,RELMODE);
            TRST(Task);
            STEP = 0xf0;
        }
        else if(TCNT(Task) >= 3000) //��ʱ����
        {
            if(InGet(I_PressAlm) == OFF)
            {
                AlarmSetBit(2, 0, 1);//ǰ��ѹ�߱���
            }
            if(InGet(I_WeldAlm) == OFF)
            {
                AlarmSetBit(2, 0, 2);//����ѹ�߱���
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

    case 2: //������λ��ʼ����
        if (HZ_AxGetStatus(BeltAxis) == 0)
        {
            //���߰����λ
            //if(GUS.TechPara.Data.HalfScripEn == 1 && InGet(I_PressAlm) == ON && InGet(I_WeldAlm) == ON)
            {
                LogicTask.SeparateWire.execute = 1;
            }

            //������λ
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



    /******���������߼�*******/
    DeviceMain(&LogicTask.DeviceMainTask);
    DivideWire(&LogicTask.SeparateWire); //���߰���
    Weld(&LogicTask.Weld); //����
}

s32 Product(u32 Sum,u32 num)
{
    return 0;
}


