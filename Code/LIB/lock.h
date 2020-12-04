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
    u8 week;		//这个值没用，禁止使用
} Date; //434
#define UNLOCK 1

typedef struct
{
    u32 flag;	   //解码标识
    Date lockdata; //解码日期
} lockdataflashsave;



#define ZYG 	1234	//客户码，4位

//初始化cid
extern void CID_init(unsigned int ClientId,unsigned int* CID);
//刷新日期
extern void Refresh(unsigned int* refreshid,unsigned int* cid);
//将加密显示的日期解成正确日期
/*
EnDate 加密的日期
DeDate 解密后的日期
*/
extern void DeDate(Date* EnDate,Date* DeDate);
//读取加密信息 读出是加密的
extern void ReadDate(lockdataflashsave* save);
//设置日期
/*
decode 注册码
save 解锁完成的日期存入区(是加密显示的)
cid 客户码
rng 本次注册的随机数
return -1 设置日期失败 0 设置日期成功
*/
extern s32 unlock(int* decode,lockdataflashsave* save,unsigned int cid,unsigned int rng);

#endif

