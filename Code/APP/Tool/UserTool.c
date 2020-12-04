/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:21
 * @LastEditTime: 2020-05-29 16:35:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Tool\UserTool.c
 */ 
#include "USERTOOL.H"
#include "can_master.h"
#include "DataDefine.h"
/********************************************************/

/**
* @author 19/02/19  yang
* @Description: 每小时产出
* @param -- 产量
* @param -- 时间 （毫秒）
* @return -- units per hour(通常使用时是要除1000，把单位变成k)
*/
float UPH(u32 time,u32 products)
{
    return (float)products * 1000 * 3600 / (float)time;
}
/**
* @author 19/02/19  yang
* @Description: 单位换算 毫米转脉冲
* @param -- num 轴号
* @param -- MM 毫米
* @return -- 脉冲数
*/
s32 ConverToPulse(u32 num,float MM)
{
    //return	MM *GSS.AxisData[num].PPR / GSS.Conversion[num].MPR;
	if( GSS.axis[num].Axconver.MPR > 0.001f)
	return (MM*GSS.axis[num].Axconver.PPR/GSS.axis[num].Axconver.MPR);
	else 
	return 1;
}
/**
* @author 19/02/19  yang
* @Description: 单位转换 脉冲转毫米
* @param --
* @param --
* @return --
*/
float ConvertToUserUnit(u32 num,s32 pulse)
{
    //return (float)GSS.Conversion[num].MPR * pulse / (float)GSS.Conversion[num].PPR;
	if(GSS.axis[num].Axconver.PPR>=1)
	{
		return (float)GSS.axis[num].Axconver.MPR*pulse/(float)GSS.axis[num].Axconver.PPR;
	}
	else
	{
		return 1;
	}
}

/*****************************************************************/
//获取输入口状态
//cardAdr : 扩展卡地址  0:本体  1：从卡1 ......
//num : 输入编号
//return ：0：ON 1:OFF，其他参数错误
s32 InGet(u8 cardAdr,u8 num)
{
	if(cardAdr == 0)
	{
		return InPut_GetSta(num);
	}
	else
	{
		return HZ_ExInPutGet(cardAdr - 1,num);
	}
}


//获取输入口状态
//cardAdr : 扩展卡地址  0:本体  1：从卡1 ......
//num
//return ：0：ON 1:OFF，其他参数错误
s32 OutGet(u8 cardAdr,u8 num)
{
	if(cardAdr == 0)
	{
		return OutPut_GetSta(num);
	}
	else
	{
		return HZ_ExOutPutGet(cardAdr - 1,num+16);
	}
}

//获取输入口状态
//cardAdr : 扩展卡地址  0:本体  1：从卡1 ......
//num：输入编号
//return ：0：ON 1:OFF，其他参数错误
s32 OutSet(u8 cardAdr,u8 num,u8 value)
{
	if(cardAdr == 0)
	{
		return OutPut_SetSta(num,value);
	}
	else
	{
		return HZ_ExOutPutSet(cardAdr - 1,num+16,value);
	}
}



