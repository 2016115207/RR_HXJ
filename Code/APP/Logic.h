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

#define NORMALMODE 0 //����ģʽ
#define AGINGMODE 1  //�ϻ�ģʽ
#define STEPMODE 2   //��������
#define AXISDELAY 5  //ͣ����ʱ
#define ENDCASE 0xff //���һ��case
#define NOXYREAD (HZ_AxGetStatus(MOTOR1) != AXSTA_READY || HZ_AxGetStatus(MOTOR2)!= AXSTA_READY)


typedef union
{
	u32 Para[50]; //4040-4139
	struct
	{
		u32 Speedlevel;				// 4040-4041 �����ٶȱ���
		u32 BeltSpeed;			// 4042 �����ٶ�
		float BeltLength;			// 4044 ��λ����
		u32 ClamCheckpEn;			// 4046 ���Ӹ�Ӧ����
		u32 ShutCheckEn;			// 4048 �����Ӧ����
		u32 WeldEn;					// 4050 ���ӿ��� ���߿���
		u32 WeldCheckEn;			// 4052 ���Ӹ�Ӧ����  ���˼��
		
		/*���߲���*/
		u32 ScripBlowDelay;         //4054 �������ʱ��
		u32 Q_DivivdeCut_OFFDelay; //4056
		u32 Q_YDivide_ONDelay;     //4058
		u32 Q_weldPress_ONDelay;  //4060
		u32 Q_FrontPress_ONDelay;  //4062
		u32 Q_FrontPress_OFDelay;  //4064  ѹ�ߴ���ʱ��
		
		/*���Ӳ���*/
		u32 Q_Clam_ONDelay;      //4066 ���Ӽмн���ʱ
		u32 Q_Clam_OFFDelay;     //4068 ���Ӽ��ɿ���ʱ
		u32 Q_Turn_ONDelay;      //4070 ��ת����ʱ
		u32 Q_Turn_OFFDelay;	 //4072	��ת����ʱ
		u32 Q_WeldMove_ONDelay;  //4074 ���ʺ�£��ʱ
		u32 Q_WeldMove_OFFDelay; //4076	���ʷֿ���ʱ	
		u32 Q_WeldDown_ONDelay;  //4078 �����½���ʱ
		u32 Q_WeldDown_OFFDelay; //4080	����������ʱ	
		u32 WeldDelay;		     //4082 ������ʱ
		u32 ColdDelay;		     //4084 ��ȴ��ʱ
		
		/*��������*/
		float FirstFeedLength_R;		//�ұ�һ���������� 4086
		float SecondFeedLength_R;		//�ұ߶����������� 4088
		u32 FirstFeedSpeed_R;			//�ұ�һ�������ٶ� 4090
		u32 SecondFeedSpeed_R;		    //�ұ߶��������ٶ� 4092
		float FirstFeedLength_L;		//���һ���������� 4094
		float SecondFeedLength_L;       //��߶����������� 4096
		u32 FirstFeedSpeed_L;         //���һ�������ٶ� 4098
		u32 SecondFeedSpeed_L;        //��߶��������ٶ� 4100
		
		float BackLength_R;				//�ұ߻��˾��� 4102
		float BackLength_L;				//��߻��˾��� 4104
		u32 BackTinDelay_R;				//�ұ߻�����ʱ 4106
		u32 BackTinDelay_L;				//��߻�����ʱ 4108
		
		/*������*/
		u16 Auto_En;        //4110 �Զ����ֶ������л���ť
		u16 Link_En;        //4111 �����˶�����
		u16 Feed1_L_En;     //4112 ��1��������
		u16 Feed2_L_En;     //4113 ��2��������
		u16 Feed1_R_En;     //4114 ��1��������
		u16 Feed2_R_En;     //4115 ��2��������
		
		/*��ǰ����*/
		u32 Product;   //4116
		u16 WeldMode;   //4118 ��������ģʽ�л�
		
	}Data;
}TechParaDef;


typedef struct //ϵͳ�������
{
    LogicParaDef AxisMoveTask[PULS_NUM];
	LogicParaDef DeviceMainTask;
    LogicParaDef SeparateWire;					//����
	LogicParaDef FeedTinTask;                   //����
	LogicParaDef Weld;							//����
} Task;
extern Task LogicTask;


typedef struct //ϵͳ�������ñ���
{
    PointDef2 xy;   //����
	PointfDef2 fxy;	//����
} SysData;
extern SysData Data;

typedef enum //��������Ŷ���
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
