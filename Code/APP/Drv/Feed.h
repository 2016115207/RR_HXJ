/*
 * @Author: your name
 * @Date: 2020-05-28 15:18:38
 * @LastEditTime: 2020-06-01 11:22:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Drv\Feed.h
 */ 
#ifndef __FEED_H
#define __FEED_H
#include "iodefine.h"
#include "LogicData.h"


typedef union  
{
	u32 Data[10];	//预留10个参数
	struct 
	{
		u32 FeedSpd;  //进料速度 百分比 4120
		float FeedPos;	//进给位置      4122
		u32 RunWithoutMask; //无口罩空跑4124
	}Para;
} FeedPara;


typedef struct Feed_type
{
	u8   PlatformSta;								//平台状态 0当前平台空闲 1当前平台正在移动 2.当前平台移动到位且有物料到位3.当前移动到位但是没有物料 4.压住料
	void (*Config)(FeedPara* para,LogicParaDef *T);
	void (*Start)(void);
	u8	 (*GetSta)(void);
	void (*ClrErr)(void);	//清除错误
	void (*Stop)(void);
} FeedDrv;
extern FeedDrv Feed_drv;    //外部使用实体

extern void Feed(FeedDrv* feeddrv);

#endif 
