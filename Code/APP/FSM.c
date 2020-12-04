/*
* @file  FSM.c
* @brief 设备状态机，通过运行态，停止态，复位态，急停态，暂停态等状态，控制机台安全运行。
* @version 1.1 bsp.lib 1.1 int.lib
* @author  yfs
* @date 2018/08/29
*/
#include "FSM.h"
/****************新状态机使用***************/
//接口定义与声明

FSM_type SysFsm;


/** 
  * @brief 回调函数 
  */ 
__weak void stop_handle(void)
{
   //this is stop;
}
__weak void run_handle(void)
{
    //this is run;
}
__weak void reset_handle(void)
{
    //this is run;
	
}
__weak void  scram_handle(void)
{
    //this is scram;
}
__weak void pause_handle(void)
{
	//this is pause;
}
__weak void errstop_handle(void)
{
	//this is pause;
}
__weak void rdystop_handle(void)
{
}
/*
	RESET_EVENT,
	SCRAM_EVENT,
	PAUSE_EVENT,
	ERR_EVENT,   //错误报警事件
*/
FsmTable_type Fsm_table[] = 
{
  //{到来的事件，当前的状态，将要要执行的函数，下一个状态}
    { STOP_EVENT,  RUN,      stop_handle,  STOP },
	{ STOP_EVENT,  D_RESET,  stop_handle,  STOP },
	{ STOP_EVENT,  SCRAM,    stop_handle,  STOP }, //急停后，只能通过stop进入正常停机态
	{ STOP_EVENT,  PAUSE,    stop_handle,  STOP },
	{ STOP_EVENT,  ERRSTOP,  stop_handle,  STOP }, // 错误停状态后，可以清除报警进入正常停止
	
	{ RUN_EVENT,  STOP,   run_handle,  RUN },
	{ RUN_EVENT,  PAUSE,  run_handle,  RUN },

	{ RESET_EVENT, STOP,  reset_handle, D_RESET},
	{ RESET_EVENT, ERRSTOP,reset_handle,D_RESET },
    { RESET_EVENT, INIT, reset_handle,D_RESET},
	{RESET_EVENT,SCRAM,reset_handle,D_RESET},
	
	{SCRAM_EVENT, STOP, scram_handle, SCRAM},
	{SCRAM_EVENT, RUN, scram_handle, SCRAM},
	{SCRAM_EVENT, D_RESET, scram_handle, SCRAM},
	{SCRAM_EVENT, PAUSE, scram_handle, SCRAM},
	{SCRAM_EVENT, ERRSTOP, scram_handle, SCRAM},
	{SCRAM_EVENT, INIT, scram_handle, SCRAM},
	 
	{PAUSE_EVENT, RUN,  pause_handle, PAUSE},
	
	{ERR_EVENT, RUN, errstop_handle, ERRSTOP},
	{ERR_EVENT, D_RESET, errstop_handle, ERRSTOP},
	{ERR_EVENT, PAUSE, errstop_handle, ERRSTOP},
	

};



/*状态机注册,给它一个状态表*/
void FSM_Regist(FSM_type *pFSM, FsmTable_type *pFsmTable)
{
    pFSM->pFsmTable = pFsmTable;
}

/*状态迁移*/
void FSM_StateTransfer(FSM_type* pFsm, int state)
{
    pFsm->curState = state;
}

int FSM_GetState(FSM_type Fsm)
{
    return Fsm.curState;
}

/*事件处理*/
void FSM_EventHandle(FSM_type* pFsm, int event)
{
    FsmTable_type* pActTable = pFsm->pFsmTable;
    void (*eventActFun)() = NULL;  //函数指针初始化为空
    int NextState;
    int CurState = pFsm->curState;
    int g_max_num = pFsm->size;
    int flag = 0; //标识是否满足条件
    int i;

    for (i = 0; i < g_max_num; i++)
    {
        if (event == pActTable[i].event && CurState == pActTable[i].CurState)
        {
            flag = 1;
            eventActFun = pActTable[i].eventActFun;
            NextState = pActTable[i].nextState;
            break;
        }
    }

    if (flag)
    {
        if (eventActFun)
        {
            eventActFun();
        }
        FSM_StateTransfer(pFsm, NextState);
    }
    else
    {
        //printf("There is no match\n");
    }
}

void InitFsm(FSM_type* DeFsm)
{
	FSM_Regist(DeFsm,Fsm_table);
    DeFsm->curState = INIT;  //上电初始化，是错误停，设备需要复位
	DeFsm->size = sizeof(Fsm_table)/sizeof(FsmTable_type);
}

void FSM(u32* Event,FSM_type* dfsm)
{
	if(*Event)
	{
		FSM_EventHandle(dfsm,*Event);
		*Event = 0;
	}
	
	GUR.RunStatus = dfsm->curState;
}





