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
	u32 Data[10];	//Ԥ��10������
	struct 
	{
		u32 FeedSpd;  //�����ٶ� �ٷֱ� 4120
		float FeedPos;	//����λ��      4122
		u32 RunWithoutMask; //�޿��ֿ���4124
	}Para;
} FeedPara;


typedef struct Feed_type
{
	u8   PlatformSta;								//ƽ̨״̬ 0��ǰƽ̨���� 1��ǰƽ̨�����ƶ� 2.��ǰƽ̨�ƶ���λ�������ϵ�λ3.��ǰ�ƶ���λ����û������ 4.ѹס��
	void (*Config)(FeedPara* para,LogicParaDef *T);
	void (*Start)(void);
	u8	 (*GetSta)(void);
	void (*ClrErr)(void);	//�������
	void (*Stop)(void);
} FeedDrv;
extern FeedDrv Feed_drv;    //�ⲿʹ��ʵ��

extern void Feed(FeedDrv* feeddrv);

#endif 
