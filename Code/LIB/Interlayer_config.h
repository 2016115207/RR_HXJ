#ifndef	__INTERLAYER_CONFIG_H
#define	__INTERLAYER_CONFIG_H

#include "bsplib.h"

//���źŻ�ȡ��������ͨIO����IO��������IO
//��ͨIO
//0  ->127 :��ͨIO
//��
//128 ->143  :ALM
//144 ->159  :LIMIT
//160 ->175  :
//176 ->191  :
//������
//192 ->207  :������A
//208 ->223  :������B(Z��)
//224 ->139
//240 ->255

//ʮ����תASCII��
#define KB2ASC(x)	 (unsigned char )(0x30+x)

//ASCII��תʮ����
#define ASC2KB(x)	 (unsigned char )(x%(0x30))

//ֻ�ܶԹ涨���ĸ��ֽڽ��н���
//decode ������ //decodeData ����֮����ַ��� cid �ͻ��� rng �����* @return -- 0 ����ɹ���-1 ʧ��
extern int Decode(int* decode,char* decodeData,unsigned int cid,unsigned int rng);
/**
* @author  yang 2019/2/27
* @Description: �����ַ��� 
* @param -- word ��Ҫ���ܵ��ַ���  s ����֮����ַ��� �ַ�������
* @return -- 0 ���ܳɹ� -1 ���ܳɹ�
*/
extern int Decrypt(char* word,char* s,int len);	
/**
* @author  yang 2019/2/27
* @Description: �����ַ��� 
* @param -- word ��Ҫ���ܵ��ַ���  s ����֮����ַ��� �ַ�������
* @return -- 0 ���ܳɹ� -1 ���ܳɹ�
*/
extern int Encrypt( char* word, char* s,int len);

extern void	HZ_AxMotion(void);
extern void HZ_ModBus(u32 port,char ModbusLocalID); //ModbusLocalID ��Ĭ����1
extern void HZ_TimerThread(void);
extern void	HZ_FlashOperate(void);
//m_axnum ��������	
extern void HZ_JogOperate(int M_Axnum);
extern void HZ_Alarm(void);
extern void init_config(void* GlobalData,u32* err,void* flah,u32 datalen,u32 writelen,u32 readlen,u32 savelen);
extern s32 HZ_Data_Read(void);
extern s32 HZ_BkData_Read(void);
extern s32 HZ_Data_Write(void);
extern s32 HZ_BkData_Write(void);



//����ӿ�

#define	SYSTIMERNUM	100
extern u32	sysTimer[SYSTIMERNUM];
#define	USERTIMERNUM	200
extern u32	UserTimer[USERTIMERNUM];
#define	TRIGERNUM	200
#define	TIMER_NUM	200
extern u8	TrigerFlag[TRIGERNUM];
extern u32	TrigerTimer[TRIGERNUM];
extern unsigned char  flagbuf[TIMER_NUM];
extern unsigned int  ton_et[TIMER_NUM];
extern u8 Triger(u8 Num, s32 TrigValue, s32 TrigerInput ,u32 ContTime);
extern unsigned char ton(unsigned int num,unsigned char ex,unsigned int pt);
extern void AlarmSetBit(u8 level, u8 word, u8 bit);
extern void AlarmClearBit(u8 level, u8 word, u8 bit);
extern s32 AlarmGetBit(u8 level, u8 word, u8 bit);

//���˶�����

extern u32 GetSpd(u8 axNum);
extern u32 GetDecSteps(u8 axNum);

//����ӿ�
//01���Զ�λ
extern s32 HZ_AxMoveAbs(u8 AxNum,s32 pos);
//02��Զ�λ
extern s32 HZ_AxMoveRel(u8 AxNum,s32 pos);
//03���Ӷ�λ
extern s32 HZ_AxMoveAdd(u8 AxNum,s32 pos);
//���¶�λ
//extern s32 HZ_AxMoveSuperImposed();
//��������()
//AxNum :��ţ���0��ʼ
//pos	:����0����С��0���� 0������
extern s32 HZ_AxMoveVelocity(u8 AxNum,s32 pos);
extern s32 HZ_AxChSpd(u8 AxNum,u32 Spd);
//ֹͣ��
extern s32 HZ_AxStop(u8 AxNum);

//////*****//////////////
//����ͣ
extern s32 HZ_AxStopDec(u8 AxNum);
//��ԭ��
extern s32 HZ_AxHome(u8 AxNum);


//��λ��(��λ���󼰰���״̬��Ϊ����)
extern s32 HZ_AxReset(u8 AxNum);
//����״̬
extern s32 HZ_AxGetStatus(u8 AxNum);
//�������
extern s32 HZ_AxGetAxisErr(u8 AxNum);
//����ǰλ��
extern s32 HZ_AxGetCurPos(u8 AxNum);
//д��ǰλ��
extern s32 HZ_AxSetCurPos(u8 AxNum,s32 pos);
//���ò���
extern s32 HZ_AxSetPara(u8 AxNum,\
				u32 Start,u32 TAcc,u32 Run,u32 TDec,u32 End,\
				u32 HomFast,u32 HomSlow,u32 HomOffset,\
				u32 TorS,u32 HomTimeOut);
//�����ٶ�����
extern s32 HZ_AxSetCurve(u8 AxNum,u32 Start, u32 TAcc, u32 Run, u32 TDec, u32 End,u32 TorS);
//�������ԭ�����
extern s32 HZ_AxSetHomePara(u8 AxNum,u8 HomSigNum,u8 HomSigLev,u8 Mod,s32 fast,s32 slow,s32 offset);
//�������źŲ���
extern s32 HZ_AxSetAxiSig(u8 AxNum,\
					u8 HomSigEn,u8 HomSigNum,u8 HomSigLev,\
					u8 UpSigEn,u8 UpSigNum,u8 UpSigLev,\
					u8 DwSigEn,u8 DwSigNum,u8 DwSigLev);
//������������λ
extern s32 HZ_AxSetLimSoft(u8 AxNum,\
					u8 UpEn,s32 UpPos,\
					u8 DwEn,s32 DwPos);
//���û�ԭ��ģʽ
//AxNum :���
//Mod	:ģʽ
//		0��������ԭ��
//		1��������������λ���ٷ�����ԭ��
//		2���ȷ���������λ����������ԭ��
extern s32 HZ_AxSetHomMod(u8 AxNum,u8 Mod);
//AxNum :��� AlmLev: 0��1 ��ƽ����  �����������ڲ���Ч
extern s32 HZ_AxSetAlm(u8 AxNum,u8 AlmLev);
//���¼��������ٶ�
extern s32 HZ_AxReloadSpd(u8 AxNum,u32 Spd);
//��ȡ��ǰ�ٶ�
extern s32 HZ_AxGetSpd(u8 AxNum);

extern void AxDirCfg(u8 Axnum,int val);
extern void AxHome_Motion(u8 n);

/****************�ص��������û�ʹ��ʱ���ⲿʵ��***************/
 void ex_inputupdata(void);
 void ex_outputstatusupdata(void);
 void ex_outputupdata(void);

#endif
