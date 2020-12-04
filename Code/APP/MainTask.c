/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-30 22:40:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\MainTask.c
 */
#include "maintask.h"
#include "teach.h"
#include "usertool.h"
#include "Datadefine.h"
#include "FSM.h"
#include "iodefine.h"

void AppErrHandle(void)
{
    if(GUR.HaveToReset == 1)
    {
        AlarmSetBit(3,0,0);
    }
    if(GUR.HaveToRepower == 1)
    {
        AlarmSetBit(4,0,4);
    }
    if(GSR.ErrorLevel<4&&GSR.ErrorLevel>0)
    {
        if(GUR.RunStatus == RUN)
        {
            GUW.Button.RunCommand = ERRSTOP;
        }
		
    }
	
    if(GSR.ErrorLevel ==4)
    {
        GUR.HaveToRepower = 1;
        GUW.Button.RunCommand = ERRSTOP;
    }
	
    if(GSR.ErrorLevel == 3)
    {
        GUR.HaveToReset = 1;
    }
	for(int i=0;i<5;i++)
	{
		if (AX_ERR_AX_ALM == HZ_AxGetAxisErr(i))
        {
            AlarmSetBit(3, 0, i+1); //ÖáµÄÇý¶¯Æ÷±¨¾¯
        }
	}

}

void stop_handle(void)
{
    HZ_AxStop(0);
	HZ_AxStop(1);
	HZ_AxStop(2);
}

extern void initIO(void);


void  scram_handle(void)
{
	for(int i=0;i<4;i++)
	{
		HZ_AxStop(i);
	}
	GUR.HaveToReset = 1;
	PARAINIT(LogicTask);

}


extern void DataInit(void);

void MainTask(void)
{
    AppErrHandle();
    FSM(&GUW.Button.RunCommand,&SysFsm);
    Logic();
    Reset();
    Teach();
	DataInit();
	ResetMotor();
    if(GUR.RunStatus != RUN && GUR.RunStatus != D_RESET && GUR.RunStatus != SCRAM)
    {
        HZ_JogOperate(7);
    }
	else
	{
		PARAINIT(GSW.JogBackward);
		PARAINIT(GSW.JogForward);
		PARAINIT(GSW.JogGohome);
	}

}

