/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-30 15:23:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\main.c
 */
#include "interlayer.h"
#include "bsp_config.h"
#include "appinit.h"
#include "maintask.h"
#include "can_master.h"
#include "LockNew.h"

const uint32_t Code_Versions __attribute__((at(0x08040800))) = 0xA05;


Date Dset;
Time Tset;
int settime;
int main()
{
//    sys_init();
    sys_init_IAP();
    bsp_init();
    bsp_exec();

    AppInit();
    CID_init(ZYG,GSR.CID);
    while(2 != HZ_ExCanNetStateGet())
    {
        InterLayer();
        bsp_exec();
    }
    while(1)
    {
        InterLayer();
        bsp_exec();
        MainTask();
        Refresh(&GSW.LockPara.refreshId,GSR.CID);
		
    }
}


int pwr_Task(void)
{
    //HZ_Data_Write();
    FRam_Write(0,&GUS,sizeof(GUS));
    while(FRAM_GetSta()!= 0)
    {
        FRam_Exec();
    }
    return 0;
}

void SysTick_Handler(void)
{

}






