/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-07-01 11:10:36
 */

#include "AppInit.h"
#include "Algorithm.h"
#include "can_master.h"
#include "UserTool.h"
#include "stdio.h"
#include "iodefine.h"

//��������  ����app����ĳ�ʼ����ʵ��������
GlobalDataDef GlobalData = {0}; //�����û�ͨѶ���ݿ�
u8 uart_sendbuf[2][300];		//����ͨѶ���պͷ��ͻ���
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];

/****************��Ŀʹ�ýṹ��ʵ��������***************/


/****************end define***************/

/**
 * @author: yfs
 * @Date: 2020-01-14 16:50:27
 * @description: �忨�����ϵ�flashȫ-1������Ҫ��ʼ��
 * @para:
 * @return:
 */
void DataInit()
{
    /*
		�˴���ӳ�ʼ���洢�����ݣ���֤�����ϵ�ûʹ�õ����ݰ�ȫ
    */
	for(int i=0;i<5;i++)
	{
		GSS.axis[i].Axconver.MPR = 10;
		GSS.axis[i].Axconver.PPR = 10;
		GSS.axis[i].Axhomecfg.homemode = 0;
		GSS.axis[i].Axlimitcfg.alarmmode = 2;
		GSS.axis[i].Axlimitcfg.limitMode = 0; //����λ
//		GSS.axis[i].Axhomecfg.orgnum = i;
		GSS.axis[i].Axhomecfg.orglev = ON;
		GSS.axis[i].Axhomecfg.homespeedfast = 800;
		GSS.axis[i].Axhomecfg.homespeedslow = 800;
		GSS.axis[i].Axhomecfg.homespeedoffset = 400;
		GSS.axis[i].AxSpd.startspeed = 500;
		GSS.axis[i].AxSpd.endspeed = 500;
		GSS.axis[i].AxSpd.runspeed = GUS.Runspd[i];
		GSS.axis[i].AxSpd.acctime = (GSS.axis[i].AxSpd.dectime = GUS.AccTm[i]);	
		GSS.axis[i].AxSpd.dectime = GSS.axis[i].AxSpd.acctime; //�Ӽ���ʱ����ͬ
	}
		
		GSS.axis[0].Axhomecfg.orgnum = X2;
//		GSS.axis[1].Axhomecfg.orgnum = 2;
//		GSS.axis[0].Axlimitcfg.neglimitlev = ON;
//		GSS.axis[0].Axlimitcfg.neglimitsig = X1;
//		GSS.axis[0].Axlimitcfg.poslimitlev = ON;
//		GSS.axis[0].Axlimitcfg.poslimitsig = X6;
//	
//		GSS.axis[1].Axlimitcfg.neglimitlev = ON;
//		GSS.axis[1].Axlimitcfg.neglimitsig = X2;
//		GSS.axis[1].Axlimitcfg.poslimitlev = ON;
//		GSS.axis[1].Axlimitcfg.poslimitsig = X7;
	
}

void initIO(void)
{
    int i;
    for (i = 0; i < 24; i++) //��ʼ�� �����
        OutPut_SetSta(i, OFF);
    for (i = 0; i < 32; i++)
        HZ_ExOutPutSet(1, i, OFF);
    for (i = 0; i < PULS_NUM; i++) //���ʹ��,��ƽ����ͨ����෴
        EN_SetSta(i, 1);

}

void AppInit()
{
    //���õ�ַ�����modbus
    init_config(&GlobalData, GSR.ErrorCode, &GlobalData.SpaceType.Save, GLOBALDATALEN, USERWRITELEN, USERREADLEN, USERSAVELEN);
    //���ݶ�ȡ
	FRam_Read(0,&GUS,sizeof(GUS));
	while(FRAM_GetSta()!= 0)
    {
        FRam_Exec();
    }
	DataInit();
    AxisConfig(GSS.axis);	//������ԭ�����λ
    initIO();
    GSW.JogMode = 2;
    GSR.HardWare_Ver[0] = 201008; 
	GSR.SoftWare_Ver[0] = 201008;  

    //��ʼ��״̬�������豸״̬��ʼ�Ǵ���ͣ
    InitFsm(&SysFsm);
	GUR.HaveToReset = 1;

}
