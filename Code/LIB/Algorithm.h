#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#include "bsplib.h"

typedef enum
{
    LINEMODE = 1,  		//直线模式
    CIRMODE = 2,			//圆弧模式
    CIRCLEMODE = 3,		//整圆模式
} InterpolationMode;


//插补部分//
//直线插补
typedef struct
{
    float x;
    float y;
    float z;
} PointfDef;

typedef struct
{
    PointfDef center;  //圆心
    float r;      //半径
    float angle;    //圆弧角度
    float p1Angle;  //P1点向量角
    float p2Angle;  //P2点向量角
    float p3Angle;  //P3点向量角
    float dir;  //方向
    float curAngle;  //当前弧度
    float relAngle;  //走过弧度值
} CircleDefNyz;


typedef struct
{
    float startSpeed; //开始速度 MM/S
    float acc;        //加速度, MM/S2
    float speed;
    float dec;
    float endSpeed;
} SpdDataDef;

typedef struct
{
    u8 execute;
    u8 step;
    u8 busy;
    u8 done;
    u8 count;
    u32 index;
    u8 endFlag;  					//插补结束标志
    u8 stopFlag;  				//插补暂停标志
    u8 mode;   						//模式：1：直线  2：整圆   3：圆弧
    u8 currMode;   				//当前模式：1：直线  2：整圆   3：圆弧
    SpdDataDef SpeedSet; 	//速度参数

    float currSpeed; 			//当前速度, MM/s
    u8 xAx;								//X轴号
    u8 yAx;								//Y轴号
    u8 zAx;								//z轴号

    PointfDef p1;   			//第一点坐标,直线点1，圆弧点1
    PointfDef p2;   			//第二点坐标,直线点2，圆弧点2
    PointfDef p3;   			//第二点坐标,圆弧点3

    CircleDefNyz Circle;  		//圆弧、整圆

    PointfDef preP;  			//上一坐标点
    PointfDef currP;  		//当前坐标
    float xDiv;   				//X轴每一补移动量
    float yDiv;   				//Y轴每一补移动量
    float zDiv;   				//z轴每一补移动量

    float xTrgPos; //X轴移动量
    float yTrgPos; //Y轴移动量
    float zTrgPos; //z轴移动量

    float InterDiv; //圆弧插补步距，度

    s32 xTrgPosPP; //X轴移动量, 脉冲量
    s32 yTrgPosPP; //Y轴移动量, 脉冲量
    s32 zTrgPosPP; //z轴移动量, 脉冲量

    s32 xTrgPosBuf; //X轴移动量, 脉冲量
    s32 yTrgPosBuf; //Y轴移动量, 脉冲量
    s32 zTrgPosBuf; //z轴移动量, 脉冲量

    u32 xTgSpd; //X轴速度
    u32 yTgSpd; //Y轴速度
    u32 zTgSpd; //Y轴速度

    float length;    //曲线长度
//		float xDis;      //直线x轴距离
//		float yDis;				//直线y轴距离
//		float zDis;				//直线z轴距离

    float currLen;  //已走线长度
    u32 pointNum;
    u8 quadrant;	//坐标象限
	
	u32 PPR[3];		//每转脉冲数 3个轴
    float MPR[3];	//每转毫米__导程
	
} AxisGroupDataDef;
typedef struct
{
	s32 x;
	s32 y;
	s32 z;
	s32 w;
	//float rad;
}PointDef;

typedef struct{
	s32 x;
	s32 y;
}mP2Def;

typedef struct
{
	u32 Vinit;
	u32 Tacc;
	u32 Vtar;
}VelocityDef;

extern s32 HZ_MultiAxMotion(void);
//extern s32 HZ_MultiAxSet(u8 Group,u8 AxNum,u8 Ax0,u8 Ax1,u8 Ax2,u8 Ax3);
//设置轴组的速度
extern s32 HZ_MultiAxSetVal(u8 Group,VelocityDef Vel);

//圆弧轴组运行 Pmid：圆弧中间点  Ptar：圆弧末点
extern s32 HZ_MultiAxMoveArc(u8 Group,u8 Num, PointDef Pmid, PointDef Ptar);

//整圆轴组运行 Pmid：圆弧中间点  Ptar: 圆弧末点 rot:圆方向
extern s32 HZ_MultiAxMoveCirc(u8 Group,u8 AxNum, PointDef Pmid, PointDef Ptar,u8 rot);

//extern s32 HZ_MultiAxMove(u8 Group,u8 AxNum, PointDef Ptar);
//获取轴组的插补状态，0空闲，1运行
extern s32 HZ_MultiAxStatus(u8 Group);

//设置直线插补轴组:Ax 组内的轴
extern s32 HZ_MultiAxSet(int Group,int Ax,...);

//插补轴组运行：Tar 组内轴的目标
extern s32 HZ_MultiAxMove(u8 Group,int Tar,...);

//插补轴组按比例获取弧长
extern mP2Def HZ_GetCircRatioPoint(u8 Group,float ratio);

//插补轴组获取中间点 默认rot写0 如果需要改变方向rot写1
extern mP2Def HZ_GetCircMiddlePoint(u8 Group,PointDef Pinit,PointDef Ptar,u8 rot);

//计算圆弧信息
extern s32 HZ_MultiAxCalcArc(u8 Group,u8 Num, PointDef Pinit,PointDef Pmid, PointDef Ptar);

//计算整圆信息
extern s32 HZ_MultiAxCalcCirc(u8 Group,u8 AxNum,PointDef Pinit, PointDef Pmid, PointDef Ptar,u8 rot);

//获取圆半径
extern float HZ_GetCircR(u8 Group);

//获取圆心X坐标
extern float HZ_GetCircRx(u8 Group);
	
//获取圆心Y坐标	
extern float HZ_GetCircRy(u8 Group);

//获取圆弧度
extern float HZ_GetCircArc(u8 Group);

//获取圆弧旋转方向
extern float HZ_GetCircRot(u8 Group);


extern void MoveLineXYRun(AxisGroupDataDef* LIn);
extern s8 AxGroupGetSta(AxisGroupDataDef LIn);
extern s8 AxGroupMoveXYLine(u8 xAixs, u8 yAxis, u8 zAxis, PointfDef p1, PointfDef p2, float startSpeed, float acc, float speed, float dec, float endSpeed, AxisGroupDataDef* LIn);
extern void MoveInterXyRUN(AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYCircle(u8 xAixs, u8 yAxis, PointfDef p1, PointfDef p2, PointfDef p3, float startSpeed, float acc, float speed, float dec, float endSpeed, AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYCir(u8 xAixs, u8 yAxis, PointfDef p1, PointfDef p2, PointfDef p3, float startSpeed, float acc, float speed, float dec, float endSpeed, AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYStop(AxisGroupDataDef* LIn);
extern s8 AxGroupMoveXYContinue(AxisGroupDataDef* LIn);

#endif
