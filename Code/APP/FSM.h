/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-29 14:46:35
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\FSM.h
 */ 
#ifndef __FSM_H
#define __FSM_H

#include "Logic.h"
#include "reset.h"
#include "axismove.h"
#include "appinit.h"

/****************状态和命令枚举***************/
typedef enum
{
    INIT, 	        //0
    STOP = 1,     //1
    RUN	,     	  //2
    D_RESET,  	  //3源码中使用出现重复定义，需要后期打包后可以正常使用
    SCRAM,    	  //4
    PAUSE,    	  //5
	ERRSTOP,  	  //6 错误停机
	//ERRREST,	  //7 需要重启 伺服报警

} RunStatus;
//命令事件
typedef enum
{
	STOP_EVENT = 1,  //停止事件
	RUN_EVENT,
	RESET_EVENT,
	SCRAM_EVENT,
	PAUSE_EVENT,
	ERR_EVENT,   //错误报警事件

} RunCMD;


/****************状态机列表***************/
typedef struct 
{
    RunCMD event; 		   // 事件
    RunStatus CurState;    // 当前状态
    void (*eventActFun)(void); //函数指针
    int nextState;         // 下一个状态
} FsmTable_type;
extern FsmTable_type Fsm_table[]; //使用的状态表


/****************状态机类***************/
typedef struct FSM_t 
{
    int curState;				//当前状态  (不要在外部更改)
    FsmTable_type* pFsmTable;  //状态机需要维护的列表
    int size;					//表的项数
} FSM_type;

extern FSM_type SysFsm;           //实例的状态机


//void FSM_Regist(FSM_type *pFSM, FsmTable_type *pFsmTable);

void InitFsm(FSM_type* DeFsm);
void FSM(u32* Event,FSM_type* dfsm);

/****************callback function***************/
//状态切换时，会启用的回调函数，根据需求在此处填写
//停止
void stop_handle(void);
//运行
void run_handle(void);
//复位
void reset_handle(void);
//急停
void scram_handle(void);
//暂停
void pause_handle(void);


#endif

