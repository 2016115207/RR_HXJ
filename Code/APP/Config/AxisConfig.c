/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:14
 * @LastEditTime: 2020-05-27 15:13:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Config\AxisConfig.c
 */ 
#include "axisconfig.h"
#include "usertool.h"

void AxisConfig(AxisData ax[])
{
    int i;
    for(i=0; i<PULS_NUM; i++)
    {
        switch(ax[i].Axlimitcfg.limitMode)
        {
        case 0: //没有限位
            HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,0,0,0,0,0,0);
            HZ_AxSetLimSoft(i,0,ax[i].Axlimitcfg.softmaxlimit,0,ax[i].Axlimitcfg.softminlimit);
            break;
        case 1: //只有软件限位
            HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,0,0,0,0,0,0);
            HZ_AxSetLimSoft(i,1,ConverToPulse(i,ax[i].Axlimitcfg.softmaxlimit),1,ConverToPulse(i, ax[i].Axlimitcfg.softminlimit));
            break;
        case 2:  //只有硬件限位
            HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,\
                           1,ax[i].Axlimitcfg.poslimitsig,ax[i].Axlimitcfg.poslimitlev,\
                           1,ax[i].Axlimitcfg.neglimitsig,ax[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i,0,ConverToPulse(i, ax[i].Axlimitcfg.softmaxlimit),0,ConverToPulse(i, ax[i].Axlimitcfg.softminlimit));
            break;
        case 3:  //两种限位都有效
			HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,\
                           1,ax[i].Axlimitcfg.poslimitsig,ax[i].Axlimitcfg.poslimitlev,\
                           1,ax[i].Axlimitcfg.neglimitsig,ax[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i,1,ConverToPulse(i, ax[i].Axlimitcfg.softmaxlimit),1,ConverToPulse(i, ax[i].Axlimitcfg.softminlimit));
            break;
        }
		HZ_AxSetAlm(i,ax[i].Axlimitcfg.alarmmode);
		AxisHomeCfg(i,ax[i].Axhomecfg);
    }
}

void AxisHomeCfg(u8 num, AxHomeDef home)
{
	if(num > PULS_NUM)
	 return ;
	HZ_AxSetHomePara(num,home.orgnum,home.orglev,home.homemode,ConverToPulse(num,home.homespeedfast),ConverToPulse(num,home.homespeedslow),ConverToPulse(num,home.homespeedoffset));
}


//设置轴的速度曲线
void AxisSetSpd(u8 AxNum, AxspdDef spd)
{
	if(AxNum > PULS_NUM)
	 return ;
    u32 start,acc,run,dec,end;
    start = ConverToPulse(AxNum,spd.startspeed);
    acc = spd.acctime;
    run = ConverToPulse(AxNum,spd.runspeed);
    dec = spd.dectime;
    end = ConverToPulse(AxNum,spd.endspeed);
    if(start>3000)
        start = 1000;
    if(acc > 200)
        acc = 80;
    if(dec >200)
        dec = 80;
    if(end >3000)
        end = 1000;
    if(run>200000)
        run = 100000;
    if(run<=start)
    {
        run = start +1;
    }
    if(run<=end)
    {
        run = end +1;
    }
    HZ_AxSetCurve(AxNum,start,acc,run,dec,end,0);

}

