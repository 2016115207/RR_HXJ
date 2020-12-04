#ifndef __LOCK_H
#define __LOCK_H
#include "InterLayer_config.h"
#include "bsplib.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "string.h"

typedef struct
{
    u8 year;		//0-99
    u8 month;		//1-12
    u8 day;			//1-31
    u8 week;		//���ֵû�ã���ֹʹ��
} Date; //434
#define UNLOCK 1

typedef struct
{
    u32 flag;	   //�����ʶ
    Date lockdata; //��������
} lockdataflashsave;



#define ZYG 	1234	//�ͻ��룬4λ

//��ʼ��cid
extern void CID_init(unsigned int ClientId,unsigned int* CID);
//ˢ������
extern void Refresh(unsigned int* refreshid,unsigned int* cid);
//��������ʾ�����ڽ����ȷ����
/*
EnDate ���ܵ�����
DeDate ���ܺ������
*/
extern void DeDate(Date* EnDate,Date* DeDate);
//��ȡ������Ϣ �����Ǽ��ܵ�
extern void ReadDate(lockdataflashsave* save);
//��������
/*
decode ע����
save ������ɵ����ڴ�����(�Ǽ�����ʾ��)
cid �ͻ���
rng ����ע��������
return -1 ��������ʧ�� 0 �������ڳɹ�
*/
extern s32 unlock(int* decode,lockdataflashsave* save,unsigned int cid,unsigned int rng);

#endif

