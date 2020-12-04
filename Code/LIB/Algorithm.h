#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#include "bsplib.h"

typedef enum
{
    LINEMODE = 1,  		//ֱ��ģʽ
    CIRMODE = 2,			//Բ��ģʽ
    CIRCLEMODE = 3,		//��Բģʽ
} InterpolationMode;


//�岹����//
//ֱ�߲岹
typedef struct
{
    float x;
    float y;
    float z;
} PointfDef;

typedef struct
{
    PointfDef center;  //Բ��
    float r;      //�뾶
    float angle;    //Բ���Ƕ�
    float p1Angle;  //P1��������
    float p2Angle;  //P2��������
    float p3Angle;  //P3��������
    float dir;  //����
    float curAngle;  //��ǰ����
    float relAngle;  //�߹�����ֵ
} CircleDefNyz;


typedef struct
{
    float startSpeed; //��ʼ�ٶ� MM/S
    float acc;        //���ٶ�, MM/S2
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
    u8 endFlag;  					//�岹������־
    u8 stopFlag;  				//�岹��ͣ��־
    u8 mode;   						//ģʽ��1��ֱ��  2����Բ   3��Բ��
    u8 currMode;   				//��ǰģʽ��1��ֱ��  2����Բ   3��Բ��
    SpdDataDef SpeedSet; 	//�ٶȲ���

    float currSpeed; 			//��ǰ�ٶ�, MM/s
    u8 xAx;								//X���
    u8 yAx;								//Y���
    u8 zAx;								//z���

    PointfDef p1;   			//��һ������,ֱ�ߵ�1��Բ����1
    PointfDef p2;   			//�ڶ�������,ֱ�ߵ�2��Բ����2
    PointfDef p3;   			//�ڶ�������,Բ����3

    CircleDefNyz Circle;  		//Բ������Բ

    PointfDef preP;  			//��һ�����
    PointfDef currP;  		//��ǰ����
    float xDiv;   				//X��ÿһ���ƶ���
    float yDiv;   				//Y��ÿһ���ƶ���
    float zDiv;   				//z��ÿһ���ƶ���

    float xTrgPos; //X���ƶ���
    float yTrgPos; //Y���ƶ���
    float zTrgPos; //z���ƶ���

    float InterDiv; //Բ���岹���࣬��

    s32 xTrgPosPP; //X���ƶ���, ������
    s32 yTrgPosPP; //Y���ƶ���, ������
    s32 zTrgPosPP; //z���ƶ���, ������

    s32 xTrgPosBuf; //X���ƶ���, ������
    s32 yTrgPosBuf; //Y���ƶ���, ������
    s32 zTrgPosBuf; //z���ƶ���, ������

    u32 xTgSpd; //X���ٶ�
    u32 yTgSpd; //Y���ٶ�
    u32 zTgSpd; //Y���ٶ�

    float length;    //���߳���
//		float xDis;      //ֱ��x�����
//		float yDis;				//ֱ��y�����
//		float zDis;				//ֱ��z�����

    float currLen;  //�����߳���
    u32 pointNum;
    u8 quadrant;	//��������
	
	u32 PPR[3];		//ÿת������ 3����
    float MPR[3];	//ÿת����__����
	
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
//����������ٶ�
extern s32 HZ_MultiAxSetVal(u8 Group,VelocityDef Vel);

//Բ���������� Pmid��Բ���м��  Ptar��Բ��ĩ��
extern s32 HZ_MultiAxMoveArc(u8 Group,u8 Num, PointDef Pmid, PointDef Ptar);

//��Բ�������� Pmid��Բ���м��  Ptar: Բ��ĩ�� rot:Բ����
extern s32 HZ_MultiAxMoveCirc(u8 Group,u8 AxNum, PointDef Pmid, PointDef Ptar,u8 rot);

//extern s32 HZ_MultiAxMove(u8 Group,u8 AxNum, PointDef Ptar);
//��ȡ����Ĳ岹״̬��0���У�1����
extern s32 HZ_MultiAxStatus(u8 Group);

//����ֱ�߲岹����:Ax ���ڵ���
extern s32 HZ_MultiAxSet(int Group,int Ax,...);

//�岹�������У�Tar �������Ŀ��
extern s32 HZ_MultiAxMove(u8 Group,int Tar,...);

//�岹���鰴������ȡ����
extern mP2Def HZ_GetCircRatioPoint(u8 Group,float ratio);

//�岹�����ȡ�м�� Ĭ��rotд0 �����Ҫ�ı䷽��rotд1
extern mP2Def HZ_GetCircMiddlePoint(u8 Group,PointDef Pinit,PointDef Ptar,u8 rot);

//����Բ����Ϣ
extern s32 HZ_MultiAxCalcArc(u8 Group,u8 Num, PointDef Pinit,PointDef Pmid, PointDef Ptar);

//������Բ��Ϣ
extern s32 HZ_MultiAxCalcCirc(u8 Group,u8 AxNum,PointDef Pinit, PointDef Pmid, PointDef Ptar,u8 rot);

//��ȡԲ�뾶
extern float HZ_GetCircR(u8 Group);

//��ȡԲ��X����
extern float HZ_GetCircRx(u8 Group);
	
//��ȡԲ��Y����	
extern float HZ_GetCircRy(u8 Group);

//��ȡԲ����
extern float HZ_GetCircArc(u8 Group);

//��ȡԲ����ת����
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
