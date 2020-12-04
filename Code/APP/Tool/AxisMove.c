#include "axismove.h"
#include "userTool.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "datadefine.h"
#include "AxisConfig.h"
#if 0
/**
* @author 19/02/25  yang
* @Description: ��λ�����ƶ��ӿ�
* @param -- Para �ƶ�����
* @param -- T ������
* @return --
*/
void Move(GoPosPara Para[], LogicParaDef T[])
{
    for (int i = 0; i < AXIS_NUM; i++)
    {
        if (Para[i].execute == 1&&Para[i].mode != STOPMODE) //�����˶�
        {
            T[i].execute = 1;
        }
        if(Para[i].execute == 1&& Para[i].mode == STOPMODE)  //����ͣ��
        {
            PARAINIT(T[i]);
            PARAINIT(Para[i]);
            HZ_AxStop(i);
        }
        INITTASK(T[i])
        switch (T[i].step)
        {
        case 1:
            if (HZ_AxGetStatus(i) == AXSTA_READY)
            {
                MotorMove(i, Para[i].speed, Para[i].pos, Para[i].mode);
            }
            T[i].step = 2;
            break;
        case 2:
            if (HZ_AxGetStatus(i) == AXSTA_READY||HZ_AxGetStatus(i)==AXSTA_ERRSTOP)
            {
                PARAINIT(T[i]);
                PARAINIT(Para[i]);
            }
            break;
        default:
            break;
        }
    }
}
#endif
/**
* @author 19/02/19  yang
* @Description: ���ƶ��ӿ�
* @param -- ��� �ٶ� λ�� ģʽ
* @param --  �ٶ� �ٷֱ�
* @return --
*/
s32 MotorMove(u32 num,u32 speed,float pos,u32 mode)
{
    if(num>PULS_NUM)
        return -1;
    SetAxisSpdRadio(num, speed);	//�����ٶ�
    switch(mode)
    {
    case ABSMODE:
        HZ_AxMoveAbs(num,ConverToPulse(num, pos));
        break;
    case RELMODE:
        HZ_AxMoveRel(num, ConverToPulse(num, pos));
        break;
    case SPDMODE:
        HZ_AxMoveVelocity(num, ConverToPulse(num, pos));
        break;
    case GOHOMEMODE:
        HZ_AxHome(num);
        break;
    default:
        break;
    }
    return 0;
}
/*
 * Created: 11/16
 * Description:
 * param:
 * Modified By:yang
 */
void SetAxisSpdRadio(u8 i,u8 spd)
{
    if(spd>=100)
    {
        spd = 100;
    }
    if(spd == 0)
    {
        spd = 1;
    }
	if (i > PULS_NUM)
    {
        return ;
    }
    if(spd > 100)
    {
        spd = 100;
    }
    if(spd == 0)
    {
        spd = 1;
    }
	AxspdDef Axspd;
	Axspd = GSS.axis[i].AxSpd;
	float radio = (float)spd / 100.0f;
	Axspd.runspeed = Axspd.runspeed * radio;
	AxisSetSpd(i,Axspd);
}

s32 SetSpd(u8 i, float RunSpeed)
{
    if (i > PULS_NUM)
    {
        return -1;
    }
	AxspdDef spd;
	spd = GSS.axis[i].AxSpd;
	spd.runspeed = RunSpeed;
	AxisSetSpd(i,spd);
	return 0;
}

//�鿴���״̬
u32 CheckHoming(u32 num)
{
	for(int i=0;i<num;i++)
	{
        if(HZ_AxGetStatus(i)>0)
		{
            return 1;
		}else if(HZ_AxGetCurPos(i)!= 0)
		{
			return 2;
		}
	}
    return 0;
}

