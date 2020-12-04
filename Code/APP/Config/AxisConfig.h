/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:14
 * @LastEditTime: 2020-05-27 15:14:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Config\AxisConfig.h
 */ 
#ifndef __AXISCONFIG_H
#define __AXISCONFIG_H
#include "bsplib.h"
#include "DataDefine.h"

extern void AxisConfig(AxisData ax[]);
//设置轴的速度曲线
extern void AxisSetSpd(u8 AxNum, AxspdDef spd);
//设置轴回零
extern void AxisHomeCfg(u8 num, AxHomeDef home);
#endif
