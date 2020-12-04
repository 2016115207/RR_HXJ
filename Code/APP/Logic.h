/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2020-02-24 10:31:18
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-07-01 14:02:12
 */
#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "bsplib.h"
#include "logicdata.h"

#define NORMALMODE 0 //正常模式
#define AGINGMODE 1  //老化模式
#define STEPMODE 2   //单步调试
#define AXISDELAY 5  //停轴延时
#define ENDCASE 0xff //最后一个case
#define NOXYREAD (HZ_AxGetStatus(MOTOR1) != AXSTA_READY || HZ_AxGetStatus(MOTOR2)!= AXSTA_READY)


typedef union
{
	u32 Para[50]; //4040-4139
	struct
	{
		u32 Speedlevel;				// 4040-4041 整机速度比例
		u32 BeltSpeed;			// 4042 链条速度
		float BeltLength;			// 4044 工位距离
		u32 ClamCheckpEn;			// 4046 端子感应开关
		u32 ShutCheckEn;			// 4048 对射感应开关
		u32 WeldEn;					// 4050 焊接开关 检线开关
		u32 WeldCheckEn;			// 4052 焊接感应开关  光纤检测
		
		/*分线参数*/
		u32 ScripBlowDelay;         //4054 半剥吹气时间
		u32 Q_DivivdeCut_OFFDelay; //4056
		u32 Q_YDivide_ONDelay;     //4058
		u32 Q_weldPress_ONDelay;  //4060
		u32 Q_FrontPress_ONDelay;  //4062
		u32 Q_FrontPress_OFDelay;  //4064  压线吹气时间
		
		/*焊接参数*/
		u32 Q_Clam_ONDelay;      //4066 端子夹夹紧延时
		u32 Q_Clam_OFFDelay;     //4068 端子夹松开延时
		u32 Q_Turn_ONDelay;      //4070 翻转开延时
		u32 Q_Turn_OFFDelay;	 //4072	翻转关延时
		u32 Q_WeldMove_ONDelay;  //4074 焊笔合拢延时
		u32 Q_WeldMove_OFFDelay; //4076	焊笔分开延时	
		u32 Q_WeldDown_ONDelay;  //4078 焊笔下降延时
		u32 Q_WeldDown_OFFDelay; //4080	焊笔升起延时	
		u32 WeldDelay;		     //4082 焊接延时
		u32 ColdDelay;		     //4084 冷却延时
		
		/*送锡参数*/
		float FirstFeedLength_R;		//右边一段送锡长度 4086
		float SecondFeedLength_R;		//右边二段送锡长度 4088
		u32 FirstFeedSpeed_R;			//右边一段送锡速度 4090
		u32 SecondFeedSpeed_R;		    //右边二段送锡速度 4092
		float FirstFeedLength_L;		//左边一段送锡长度 4094
		float SecondFeedLength_L;       //左边二段送锡长度 4096
		u32 FirstFeedSpeed_L;         //左边一段送锡速度 4098
		u32 SecondFeedSpeed_L;        //左边二段送锡速度 4100
		
		float BackLength_R;				//右边回退距离 4102
		float BackLength_L;				//左边回退距离 4104
		u32 BackTinDelay_R;				//右边回退延时 4106
		u32 BackTinDelay_L;				//左边回退延时 4108
		
		/*开关类*/
		u16 Auto_En;        //4110 自动，手动运行切换按钮
		u16 Link_En;        //4111 连续运动开关
		u16 Feed1_L_En;     //4112 左1送锡开关
		u16 Feed2_L_En;     //4113 左2送锡开关
		u16 Feed1_R_En;     //4114 右1送锡开关
		u16 Feed2_R_En;     //4115 右2送锡开关
		
		/*当前产量*/
		u32 Product;   //4116
		u16 WeldMode;   //4118 焊锡动作模式切换
		
	}Data;
}TechParaDef;


typedef struct //系统任务队列
{
    LogicParaDef AxisMoveTask[PULS_NUM];
	LogicParaDef DeviceMainTask;
    LogicParaDef SeparateWire;					//分线
	LogicParaDef FeedTinTask;                   //送锡
	LogicParaDef Weld;							//焊锡
} Task;
extern Task LogicTask;


typedef struct //系统程序自用变量
{
    PointDef2 xy;   //整形
	PointfDef2 fxy;	//浮点
} SysData;
extern SysData Data;

typedef enum //触发器编号定义
{
    tg0,
    tg1,
    tg2,
    tg3,
    tg4,
    tg5,
    tg6,
    tg7,
    tg8,
    tg9,
    tg10,
    tg11,
} TgNum;
 

extern void Logic(void);

#endif
