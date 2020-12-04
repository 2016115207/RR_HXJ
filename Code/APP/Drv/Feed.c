/*
 * @Author: your name
 * @Date: 2020-05-28 15:18:32
 * @LastEditTime: 2020-06-04 18:44:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Drv\Feed.c
 */ 
#include "Feed.h"
#include "USERTOOL.H"
#include "DrvCom.h"
#include "weld.h"
#include "AxisMove.h"
#include "math.h"

//function   
static void FeedConfig(FeedPara*p,LogicParaDef *T);
static void FeedStart(void);
static u8 FeedGetSta(void);
static void FeedClrErr(void);
static void FeedStop(void);

//定义
DrvType drv;
FeedPara para;
FeedDrv Feed_drv = 
{
    0,
    FeedConfig,
    FeedStart,
    FeedGetSta,
	FeedClrErr,
    FeedStop
};



/**
 * 接口函数
*/
static void FeedStart(void)
{
    if(drv.DrvSta==D_REDY)
	{
	    drv.Task->execute = 1;
		drv.DrvSta = D_RUN;
	}
}

static u8 FeedGetSta(void)
{
    return drv.DrvSta;
}

static void FeedClrErr(void)
{
	drv.DrvSta = D_REDY;
}
static void FeedStop(void)
{
    if(drv.DrvSta==D_RUN)
        drv.DrvSta = D_REDY;
}

static void FeedConfig(FeedPara* p,LogicParaDef *T)
{
	if(p->Para.FeedPos == 0)
	{
		p->Para.FeedPos = 100;
	}
	if(p->Para.FeedSpd == 0)
	{
		p->Para.FeedSpd = 10;
	}
    para = *p;
    drv.Task = T; 
}


//实际运行loop 中间供料机构,负责物料前进一格,并检查
void Feed(FeedDrv* feeddrv)
{
	static float tar;
    if(drv.DrvSta != D_RUN) //还没有初始化
    {
        return;
    }
    if(drv.Task->execute == 1 && drv.Task->step == 0 )
    {
        drv.Task->step = 1;
    }
    switch (drv.Task->step)
    {
    case 1:   //当前平台没有需要加工的物料
		if(feeddrv->PlatformSta == 0)
		{ //确保感应器在上方
			if(InGet(I_MidPressUp)==ON&&InGet(I_LWeldUp)==ON&&InGet(I_RWeldUp)==ON \
				&& InGet(I_LClampUp)==ON&&InGet(I_RClampUp)==ON)
			{
				tar =  ConvertToUserUnit(BEMOTOR,HZ_AxGetCurPos(BEMOTOR))+para.Para.FeedPos;
				MotorMove(BEMOTOR,para.Para.FeedSpd,tar,ABSMODE);
				GUR.FeedRunstates = 1;	//进料轴送料
				drv.Task->step = 2;
			}else 
			{
				AlarmSetBit(2,0,1); //检查气缸是否已经在上端
				drv.DrvSta = D_ERR;
			}
		}
        break;
    case 2://
		if(TAR(BEMOTOR,tar))
		{	
			if(para.Para.RunWithoutMask == 0)  //需要检查物料
			{
				if(InGet(I_MaskSensor)==ON)     //有物料
				{
					feeddrv->PlatformSta = 1;
					drv.Task->step = 3;
				}
				else 
				{
					drv.Task->step = 1;
				}
			}
			else 
			{
				feeddrv->PlatformSta = 1;
				drv.Task->step = 3;
			}
		}else if(HZ_AxGetStatus(BEMOTOR)==0)
		{
			MotorMove(BEMOTOR,para.Para.FeedSpd,tar,ABSMODE);
		}
        break;
    case 3: //压料板下压
		GUR.FeedRunstates = 4;	
		OutSet(Q_MidPress,ON);
		drv.Task->step = 4; 
        break;
    case 4:
        if(InGet(I_MidPressDw) == ON)
        {
            drv.Task->step = 5;
			feeddrv->PlatformSta = 2;	//平台物等待焊接
			GUR.FeedRunstates = 5;	//等待焊接完成
        }
        break;
    case 5: //等待焊接结束
        if(Weld_drv_R.WeldingSta == 1&&Weld_drv_L.WeldingSta == 1)
        {
            OutSet(Q_MidPress,OFF);
            drv.Task->step = 6;
			GUR.FeedRunstates = 6;	//等待压板抬起
        }
        break;
    case 6:
        if(InGet(I_MidPressUp) == ON)
        {
            drv.Task->step = 7;
        }
        break;
	case 7:
		{
			feeddrv->PlatformSta = 0;
			Weld_drv_R.WeldingSta = 0;
			Weld_drv_L.WeldingSta = 0;
			GUS.Product.production ++;
			GUR.FeedRunstates = 0;
			drv.Task->step = 1;
		}
		break;
    default:
        break;
    }
}

