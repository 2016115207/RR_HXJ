/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-04 18:41:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Reset.c
 */
#include "reset.h"
#include "usertool.h"
#include "iodefine.h"
#include "logic.h"
LogicParaDef ResetTask;

void reset_handle(void)
{
    REST_TASK.execute = 1;
}

LogicParaDef MtRst[5];

void MotorGoHome(u8 ax)
{
	if(MtRst[ax].execute==0&&HZ_AxGetStatus(ax)==0)
		MtRst[ax].execute = 1;
}

void ResetMotor(void)
{
    for(int i=0; i<5; i++)
    {
        if(MtRst[i].execute==1&&MtRst[i].step==0)
        {
            MtRst[i].step=1;
			MtRst[i].err = 0;
        }
        switch(MtRst[i].step)
        {
        case 1:
            if(HZ_AxGetStatus(i)==0)
            {
                HZ_AxHome(i);
				TimerRst(MtRst[i]);
                MtRst[i].step = 2;
            }
            break;
        case 2:
            if(HZ_AxGetStatus(i)==0)
            {
                HZ_AxSetCurve(i,200,100,1000,100,200,0);
                HZ_AxMoveRel(i,-50);
				TimerRst(MtRst[i]);
                MtRst[i].step = 3;
            }
			else if(TimerCnt(MtRst[i])>=10000)
			{
				PARAINIT(MtRst[i]);
				MtRst[i].err = 1;
			}
            break;
			
        case 3:
            if(InPut_GetSta(GSS.axis[i].Axhomecfg.orgnum)==GSS.axis[i].Axhomecfg.orglev)
            {
                HZ_AxStop(i);
                MtRst[i].step=4;
            }            
			if(i == 2)
            {
                HZ_AxStop(i);
                MtRst[i].step=4;
            }
            break;
			
		case 4:
			if(HZ_AxGetStatus(i)==0)
            {
				MtRst[i].step = 5;
			}
			break;
        case 5:
            if(HZ_AxGetStatus(i)==0)
            {
                HZ_AxSetCurPos(i,0);
                PARAINIT(MtRst[i]);
            }
            break;
        }
    }
}

extern void initIO(void);

//设备复位函数
void Reset()
{
    if(GUR.RunStatus == D_RESET)
    {
        if(REST_TASK.execute == 1&&REST_TASK.step == 0)
        {
            REST_TASK.step = 1;
        }
        switch(REST_TASK.step)
        {
			case 1:
					PARAINIT(LogicTask);
					initIO();
					REST_TASK.step = 2;
					break;

			case 2:
						if(InGet(I_PressAlm) == ON && InGet(I_WeldAlm) == ON)
						{
							HZ_AxHome(0);
							REST_TASK.step = 3; 
						}
						               
					 
					break;
					
			case 3:
					if(HZ_AxGetStatus(0) == AXSTA_READY)
					{
							PARAINIT(REST_TASK);
							GSW.ClearAlarm = 1;
							GUR.HaveToReset = 0;
							GUW.Button.RunCommand = STOP;
					}
					break;
        }
    }
    else
    {
        PARAINIT(REST_TASK);

    }
}



