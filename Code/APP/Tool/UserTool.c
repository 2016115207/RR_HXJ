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
* @Description: ÿСʱ����
* @param -- ����
* @param -- ʱ�� �����룩
* @return -- units per hour(ͨ��ʹ��ʱ��Ҫ��1000���ѵ�λ���k)
*/
float UPH(u32 time,u32 products)
{
    return (float)products * 1000 * 3600 / (float)time;
}
/**
* @author 19/02/19  yang
* @Description: ��λ���� ����ת����
* @param -- num ���
* @param -- MM ����
* @return -- ������
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
* @Description: ��λת�� ����ת����
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
//��ȡ�����״̬
//cardAdr : ��չ����ַ  0:����  1���ӿ�1 ......
//num : ������
//return ��0��ON 1:OFF��������������
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


//��ȡ�����״̬
//cardAdr : ��չ����ַ  0:����  1���ӿ�1 ......
//num
//return ��0��ON 1:OFF��������������
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

//��ȡ�����״̬
//cardAdr : ��չ����ַ  0:����  1���ӿ�1 ......
//num��������
//return ��0��ON 1:OFF��������������
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



