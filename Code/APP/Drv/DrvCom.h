/*
 * @Author: your name
 * @Date: 2020-05-28 18:10:07
 * @LastEditTime: 2020-05-29 15:10:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Drv\DrvCom.h
 */ 

#ifndef __DRVCOM_H
#define __DRVCOM_H
#include "LogicData.h"

typedef enum
{
    D_INIT,
    D_REDY,
    D_RUN,
    D_ERR,
    D_PASE,
    D_STOP
}DrvStatus;

typedef struct 
{
	DrvStatus DrvSta;  //0 ��ʼ 1.�������׼�� 2.�������� 3.����ͣ 4.��ͣ5�ⲿֹͣ
	LogicParaDef* Task;
} DrvType;

typedef struct 
{
    u8 card;
    u8 num;
} IODef;

#endif 

