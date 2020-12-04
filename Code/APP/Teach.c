/*
 * @Author: your name
 * @Date: 2020-06-19 14:36:56
 * @LastEditTime: 2020-06-30 22:46:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZH_ZDJ\Code\APP\Teach.c
 */
#include "Teach.h"
#include "DataDefine.h"
#include "FSM.h"
#include "userTool.h"
#include "iodefine.h"


typedef struct
{
    u32 excute;
    u32 step;
    u32 card;
    u32 inputnum;
    s32 maxpos; //���Ŀ����
    u32 spdratio; //�ٶȱ���
    s32 state;  //1,������Ŀ�꣬2.�ҵ�Ŀ�� 3.û�ҵ�Ŀ�� -1����
	u32 tm;
} MotorTar;
static MotorTar mt_tar[5];

//�ƶ����������λ�ã��ж��ᵽָ��io�������ֹͣ
void MoveToTarInput(u8 ax, s32 pos,u32 spd, u8 card, u8 num)
{
    if (mt_tar[ax].excute == 0 && HZ_AxGetStatus(ax) == 0)
    {
        mt_tar[ax].excute = 1;
        mt_tar[ax].card = card;
        mt_tar[ax].inputnum = num;
        mt_tar[ax].maxpos = pos;
        mt_tar[ax].spdratio = spd;
    }
}
//���ص�ǰ���Ƿ��Ѿ��ƶ���ָ��Ŀ��ioλ��
s32 GetAxState(u8 ax)
{
    return mt_tar[ax].state;
}

//ѭ������
void MoveTar_exc(void)
{
    for (int i = 0; i < 5; i++)
    {
        if (mt_tar[i].excute == 1 && mt_tar[i].step == 0)
        {
            mt_tar[i].step = 1;
        }
        switch (mt_tar[i].step)
        {
        case 1:
            if (InGet(mt_tar[i].card, mt_tar[i].inputnum) == OFF)
            {
                MotorMove(i, mt_tar[i].spdratio, mt_tar[i].maxpos, RELMODE);
                mt_tar[i].step = 2;
                mt_tar[i].state = 1;
            }
            else
            {
                mt_tar[i].step = 3;
            }
            break;
        case 2:
			if (HZ_AxGetStatus(i) == 0)
			{
				if(InGet(mt_tar[i].card, mt_tar[i].inputnum) == ON)
				{
					//HZ_AxStop(i);
					mt_tar[i].step = 3;
				}
				else //if (HZ_AxGetStatus(i) == 0 && InGet(mt_tar[i].card, mt_tar[i].inputnum) == OFF)
				{
					mt_tar[i].step = 4;
					mt_tar[i].tm = SysTim_GetUser();
				}
			}
            break;
        case 3: //�ҵ���
            if (HZ_AxGetStatus(i) == 0)
            {
                mt_tar[i].state = 2;
                mt_tar[i].step = 0;
                mt_tar[i].excute = 0;
            }
            break;
		case 4:
			if((SysTim_GetUser()- mt_tar[i].tm)/10 >=50)
			{
				if(InGet(mt_tar[i].card, mt_tar[i].inputnum) == OFF)
				{
					mt_tar[i].state = 3; //û���ҵ�
				}else 
				{
					mt_tar[i].state = 2;
				}
                mt_tar[i].step = 0;
                mt_tar[i].excute = 0;
			}
			break;
        }
    }
}

void Teach()
{
    if ((LogicTask.Weld.execute == 1 || LogicTask.SeparateWire.execute == 1) || GUS.TechPara.Data.Auto_En == 1 ||GUS.TechPara.Data.Auto_En == 2)
    {
        return;
    }
//	
	switch (GUW.TeachWord) //1520
    {
		case 1: //�������
			GSW.ClearAlarm = 1;
			GUW.Button.RunCommand = STOP;
        break;
		
		case 2: //�����߹�������
			if(HZ_AxGetStatus(BeltAxis) == AXSTA_READY && InGet(I_PressAlm) == ON && InGet(I_WeldAlm) == ON)
			{
				MotorMove(BeltAxis,GUS.TechPara.Data.BeltSpeed,800,RELMODE);  //GUS.TechPara.Data.BeltLength
			}
        break;
			
		case 3: //ǰ��ѹ��
			if(HZ_AxGetStatus(BeltAxis) == AXSTA_READY)
			{
				if(OutGet(Q_FrontPress) == OFF)
				{
					OutSet(Q_FrontPress,ON);
				}
				else
					OutSet(Q_FrontPress,OFF);
				
			}
        break;
		
		case 4: //����ѹ��
			if(HZ_AxGetStatus(BeltAxis) == AXSTA_READY)
			{
				if(OutGet(Q_WeldPress) == OFF)
				{
					OutSet(Q_WeldPress,ON);
				}
				else
					OutSet(Q_WeldPress,OFF);
			}
		break;
		case 5: //���ӷ�ת
			if(OutGet(Q_Turn) == OFF)
			{
				OutSet(Q_Turn,ON);  //GUS.TechPara.Data.BeltLength
			}
			else if(OutGet(Q_Turn) == ON && OutGet(Q_Clam) == OFF)
			{
				OutSet(Q_Turn,OFF); //�����ſ����ܷ�ת��ȥ
			}
		break;		
			
		case 6:
			GUS.TechPara.Data.Product = 0;
		
			break;
		case 7: //����
			if(OutGet(Q_WeldBlow) == OFF || OutGet(Q_ScripBlow) == OFF)   
			{
				OutSet(Q_WeldBlow,ON);
				OutSet(Q_ScripBlow,ON);
			}
			else
			{
				OutSet(Q_WeldBlow,OFF);
				OutSet(Q_ScripBlow,OFF);
			}
			break;
		case 8: //������
			MotorMove(FeedTin_L_Axis,GUS.TechPara.Data.FirstFeedSpeed_L,GUS.TechPara.Data.FirstFeedLength_L,RELMODE);
			break;
		case 9: //������
			MotorMove(FeedTin_R_Axis,GUS.TechPara.Data.FirstFeedSpeed_R,GUS.TechPara.Data.FirstFeedLength_R,RELMODE);
			break;
	}
	GUW.TeachWord = 0;
}
