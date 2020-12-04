#include "InterLayer.h"
#include "Algorithm.h"
#include "can_master.h"
#include "usertool.h"
#include "iodefine.h"

void Axis_pos(void);

//ÿ����ѯ�Ľӿں���
void InterLayer()
{
    //��ȡrtcʱ��
    RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);	
	RTC_Get_Date(&GSR.date.year,&GSR.date.month,&GSR.date.day,&GSR.date.week);
    //�ײ��ᶯ����
    HZ_AxMotion();
    //modbusͨѶ����
    HZ_ModBus(uart_232, 1);
    HZ_ModBus(uart_485,1);
    //HZ_ModBus(eth, 1);
    //�����ʱ��
    HZ_TimerThread();
	//���ö���岹
	//HZ_MultiAxMotion();
    //��ַ���еı�������
    HZ_Alarm();
    //��ȡ�ᵱǰ״̬
    Axis_pos();
	
	AxisConfig(GSS.axis);
}
/**
* @author  yang
* @Description: ��ȡ�ᵱǰ״̬
* @param -- �û��Լ������ڲ���Ҫ��ȡ������
* @param --
* @return --
*/
void Axis_pos()
{
    int i;
    for(i=0; i<PULS_NUM; i++)
    {
        GSR.AxisPosition[i]= HZ_AxGetCurPos(i);
		GSR.AxisUnitPosition[i] = ConvertToUserUnit(i, GSR.AxisPosition[i]);
    }
    for(i = 0; i < 4; i++)
    {
        GSR.AxisEncoder[i] = EnCode_Get32(i);
    }
    for(i = 0; i<PULS_NUM; i++)
    {
        GSR.AXSTA[i] = HZ_AxGetStatus(i);
    }
}

/**
* @author  yang
* @Description: �Դ��㶯���ú�������ִ�е㶯ʱ����
* @param --
* @param --
* @return --
*/
void JogGo(u8 axisnum,s32 pos,u32 spd)
{
	switch(spd)
	{
		case 0:    //����
			spd = 2;
			break;
		case 1:    //����
			spd = 50 ;
			break;
		case 2:    //ȫ��
			spd = 100;
			break;
		default:
			spd = 20;
			break;
	}
	
	SetAxisSpdRadio(axisnum, spd);
    if(axisnum<PULS_NUM)	//ֻ���������ֵ㶯����
    {
        if(pos > 0)	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //ֻ�����޴���
                if(0 == (0x0fff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);					
					HZ_AxMoveRel(axisnum,pos);					
                }
            } else 
			{
                //û�б���,�����˶�	
				HZ_AxMoveRel(axisnum,pos);				
            }
        }
        else	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //ֻ�����޴���
                if(0 == (0xf0ff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
					HZ_AxMoveRel(axisnum,pos);	
                }
            } else {
                //û�б���,�����˶�
				HZ_AxMoveRel(axisnum,pos);	
            }
        }
    }
}
//ϵͳ�㶯ֹͣ���ú���
void jogstop(u32 axisnum)
{
    HZ_AxStop(axisnum);
}
//ϵͳ������ú���
void joghome(u32 axisnum)
{
	HZ_AxReset(axisnum);
	HZ_AxHome(axisnum);

}
/**
* @author  yang
* @Description: ��չ���������״̬��Modbus���£���Modbus��ѯ��ָ����ַ�����
* @param --
* @param --
* @return --
*/
void ex_inputupdata()
{
    u8 i;
    //��չ��1
    GSR.InputStatus[4] = 0;
    GSR.InputStatus[5] = 0;
    GSR.InputStatus[6] = 0;
    GSR.InputStatus[7] = 0;
    for(i=0; i<32; i++)
    {
        GSR.InputStatus[4] |=(u32) HZ_ExInPutGet(0,i)<<i;
    }

}

void ex_outputstatusupdata()
{
    u8 i;
    //��չ�����1
    GSW.OutputStatus[4] = 0;
    for(i = 0; i < 16; i ++)
        GSW.OutputStatus[4]	|= (u32)HZ_ExOutPutGet(0,i+16) <<	i;

}

void ex_outputupdata()
{
    u8 i;
    //��չ�����1
    for(i = 0; i < 16; i ++)
	{
		HZ_ExOutPutSet(0,i+16, (GSW.OutputStatus[4]>>(i)&0x01));
	}
        //EX_OUTPUT[0][i]=(GSW.OutputStatus[4]>>i&1);
}

