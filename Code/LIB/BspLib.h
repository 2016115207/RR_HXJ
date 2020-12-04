#ifndef __BSPLIB_H
#define __BSPLIB_H

#include "stdint.h"

//V00.01.00
typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;


typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


//系统初始化	//上电先调用此函数
extern void sys_init(void);
extern void sys_init_IAP(void);
//IAP功能循环体
extern void IAP_Loop(void);

//这是给出去的
#define UART_NUM			2
#define DB485_SN			0
#define DB232_SN			1
extern void UART_exeu(void); 
//UART
//fun	:初始化串口
//num 	:串口号
//tx_buf:发送缓冲区指针
//rx_buf:接收缓冲区指针
//bps	:波特率
//SilenceTime	:静默时间（单位100us）（用于接收断帧）
//return 		:0正常
extern s32 Uart_Init(u32 num,void *tx_buf,void * rx_buf,u32 rx_Maxlen,u32 bps,u32 silenceTime);
//fun	:串口发送数据
//buf	:发送缓冲区
//len	:发送长度
//return :0正常1:串口忙 -1：参数错误
extern s32 Uart_Send(u32 num,void *buf,u32 len);
//fun		:接收串口数据
//buf		:接收缓冲区
//len		:接收长度
//return	:0没收到，其他：接收长度
extern u32 Uart_Recv(u32 num,void *buf);

//以太网
//以太网接收
//return 0:未接收，其他：接收数据长度
extern u32 Eth_Recv(void);
//以太网发送
//tx_buf:发送缓冲地址
//tx_len:发送数据长度
extern u32 Eth_Send(u8 * tx_buf,u32 tx_len);

//需要定期轮询的函数
extern void Eth_excu(void);

//网口设置远端IP与端口号
extern void Eth_SetRmt(u8 ip_HH,u8 ip_HL,u8 ip_LH,u8 ip_LL,u16 port);
//网口设置自己IP与端口
extern void Eth_SetLoc(u8 ip_HH,u8 ip_HL,u8 ip_LH,u8 ip_LL,u16 port);

//fun 		:以太网接口初始化
//rx_buf	:接收缓冲区
//rx_Maxlen	:接收缓冲区最大长度
//return 	:0:成功 其他:失败
extern s32 Eth_Init(void * rx_buf,u32 rx_Maxlen);


extern u32 Eth_Recv_Tcp(void);
extern s32 Eth_Send_Tcp(u8 * tx_buf,u32 tx_len);
//fun 		:以太网接口初始化
//rx_buf	:接收缓冲区
//rx_Maxlen	:接收缓冲区最大长度
//u32 checkonlineFlg :是否检测启用掉线检测，0：不检测，其他：检测(200MS检测一次)
//return 	:0:成功 其他:失败
extern s32 Eth_Init_Tcp(void * rx_buf,u32 rx_Maxlen,u32 checkonlineFlg);

//需要定期轮询的函数
extern void Eth_excu_Tcp(void);


//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,LAN8720初始化失败
//      3,网卡添加失败.
//设置IP等信息后，首先需要调用此网口初始化，再调用tcp或udp初始化
extern u8 lwip_comm_init(void); 

//USB(FAT32文件系統)
extern void USB_Mass_Init(void);
extern void USB_Mass_exec(void);



//系统定时器

#define TIMER_STA_PAUSE		0
#define TIMER_STA_CONTINUE	1

extern void SysTim_Init(void);		//系统时钟初始化，单位0.1ms

//用户时钟暂停 0:暂停，1：继续
extern void SysTimer_SetSta(u8 Sta);
//用户时钟状态获取 0:暂停，1：继续
extern u8 SysTim_GetSta(void);

////获取系统时间（可以暂停）
//#define SysTim_GetUser()	SysTimerItf.usertimercnt
////获取用户时间（不可暂停）
//#define SysTim_GetSys()		SysTimerItf.systimercnt


//获取用户时间（可以暂停，单位0.1ms）
extern u32 SysTim_GetUser(void);
//获取系统时间（不可暂停单位0.1ms）
extern u32 SysTim_GetSys(void);




//GPIO

extern void gp_io_init(void);
//设置输出口状态
//num : 序号（从0开始）
//val : 状态 0：ON 1:OFF
//return ：0 成功 其他，参数错误
extern s32 OutPut_SetSta(u8 num,u8 val);

//获取输出口状态
//num : 序号（从0开始）
//return ：0：ON 1:OFF，其他参数错误
extern s32 OutPut_GetSta(u8 num);
//获取输入口状态
//num : 序号（从0开始）
//return ：0：ON 1:OFF，其他参数错误
extern s32 InPut_GetSta(u8 num);

//设置方向输出口状态
//num : 序号（从0开始）
//val : 状态 0或1
//return ：0 成功 其他，参数错误
extern s32 Dir_SetSta(u8 num,u8 val);

//获取方向输出口状态
//num : 序号（从0开始）
//return ：0或1:OFF，其他参数错误
extern s32 Dir_GetSta(u8 num);


//设置使能输出口状态
//num : 序号（从0开始）
//val : 状态 0或1
//return ：0 成功 其他，参数错误
extern s32 EN_SetSta(u8 num,u8 val);

//获取使能输出口状态
//num : 序号（从0开始）
//return ：0或1，其他参数错误
extern s32 EN_GetSta(u8 num);
//获取报警输入口状态
//num : 序号（从0开始）
//return ：0或1，其他参数错误
extern s32 ALM_GetSta(u8 num);

//获取限位口状态
//num : 序号（从0开始）
//return ：0或1，其他参数错误
extern s32 LIMIT_GetSta(u8 num);


//CAN
								    

//CAN初始化，支持1000，500，250kbps 其他数值，一律500kbps
extern  void Can_Init(u32 Kbps);										 							 				    
//u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
extern  void CAN_exeu(void);


//ID不小于0x800 自动发送扩展帧，
//return ：0：成功 -1：上一帧没发完
extern  s32 Can_SendData(u32 id,u8 len,u8* buf);
//return 0:空闲，1：busy
extern  u8 Can_GetSendSta(void);
//return -1:没有收到数据 其他：接收数据长度
extern  s8 Can_GetRecvData(u32 * id,u8* buf);


//对外接口
extern void Led_Init(void);
extern void Led_Exec(void);

//FLASH存储
//写保存区数据
//buf :要保存的数据buf
//size:大小，单位Byte 最大 120*1024 只能为4的整倍数
//return 0：正常 -1：参数错误 其他：保存失败
extern s32 Flash_WriteData(void *buf,u32 size);
//读保存区数据
//buf :数据buf指针
//size:大小，单位Byte 最大 120*1024 只能为4的整倍数
//return 0：正常 -1：参数错误
extern s32 Flash_ReadData(void *buf,u32 size);
//写备份区数据
//buf :要保存的数据buf
//size:大小，单位Byte 最大 120*1024 只能为4的整倍数
//return 0：正常 -1：参数错误 其他：保存失败
extern s32 Flash_WriteBkData(void *buf,u32 size);
//读备份区数据
//buf :数据buf指针
//size:大小，单位Byte 最大 120*1024 只能为4的整倍数
//return 0：正常 -1：参数错误
extern s32 Flash_ReadBkData(void *buf,u32 size);




//ADC
//ADC初始化
extern void Adc_Init(void);
//获得ADC值
//ch:通道值 0~16
//return:转换结果
extern u16  Adc_Get_Val(u8 ch); 				//获得某个通道值 
//获取进50次平均值
extern u16 Adc_Get_Avg(u8 ch);				//得到某个通道给一定次数采样的平均值  

//DAC

void Dac_Init(void);		//DAC通道1初始化	 
//设置通道1输出电压
//val:0~4095,代表0~10V
extern s32 Dac_SetVal(u8 ch,u16 Val);

//RTC
//RTC初始化
extern u8 RTC_Init(void);						//RTC初始化
//RTC时间设置
extern u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);
//RTC日期设置
extern u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);
//获取RTC时间
extern void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);
//获取RTC日期
extern void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);

//设置备份域数据
//大小4k
//offset :备份域偏移量，以字（4Byte）为单位
//src :要保存的源数据指针（）
//size:数据大小,单位Byte保证（4 * offset + size < 0x1000)
//return 0:z正常 -1：参数错误
extern s32 RTCSetBkSramData(u32 offset,void *src,u32 size);
//读取备份域数据
//大小4k
//offset :备份域偏移量，以字（4Byte）为单位
//tar :要保存的目标数据指针（）
//size:数据大小,单位Byte保证（4 * offset + size < 0x1000)
//return 0:z正常 -1：参数错误
extern s32 RTCGetBkSramData(u32 offset,void *tar,u32 size);

//pwr	电源管理

extern void pwr_init(void);
extern void pwr_exec(void);
extern s32 pwr_Task(void);	//外部实现（需要在外部实现的函数）

//脉冲设置

extern void Puls_Init(void);
extern void Puls_Exec(void);
extern s32 Puls_Stop(u8 num);
extern s32 Puls_Start(u8 num,u32 steps);
extern s32 Puls_SetWidth(u8 num,float us);
extern s32 Puls_GetSta(u8 num);
extern u32 Puls_GetRan(u8 num);
extern u32 Puls_GetRun(u8 num);
extern s32 Puls_SetSpd(u8 num,u32 spd);
//慎用(仅用于运动中改变行程)
extern u32 Puls_SetRun(u8 num,u32 steps);
//ECODE 编码器
//硬件初始化
extern void EnCode_Init(void);
//需要轮询的执行函数
extern void EnCode_Exec(void);
//获取对应编码器32位计数
extern s32 EnCode_Get32(u8 num);
//设置编码器计数值（不推荐使用）
extern void EnCode_SetVal(u8 num,s32 val);

//FILE 文件操作


//20180704 zwg
//写入文件，如果没有则创建
//name		: 文件名所在的字符串（为0时，num起作用）
//buf 		: 所要写入的内容所在的地址（指针）
//num 		: 所要写入的文件序列号，（仅当name为0时起作用）
//len 		: 所要写入的长度
//offset	: 所要写入的文件偏移地址（从文件的offset处开始写）
//return	: 错误类型
extern u8 File_Write(char * name,void * buf,u32 num, u32 len,u32 offset);

//20180704 zwg
//读文件
//name		: 文件名所在的字符串（为0时，num起作用）
//buf 		: 所要读的内容所在的地址（指针）
//num 		: 所要读的文件序列号，（仅当name为0时起作用）
//len 		: 所要读的长度
//offset	: 所要读的文件偏移地址（从文件的offset处开始写）
//return	: 错误类型
extern u8 File_Read(char * name,void * buf,u32 num, u32 len,u32 offset);

//20180704 zwg
//读文件大小
//name		: 文件名所在的字符串（为0时，num起作用）
//num 		: 所要读的文件序列号，（仅当name为0时起作用）
//return	: 非负:文件大小
//			: 负  :错误
extern s32 File_GetSize(char * name,u32 num);

//20180704 zwg
//删文件
//name		: 文件名所在的字符串（为0时，num起作用）
//num 		: 所要删除的文件序列号，（仅当name为0时起作用）
//return	: 错误类型
extern u8 File_Delete(char * name,u32 num);

//20180704 zwg
//扫描文件夹文件列表
//name 文件夹名称(不需要加前面的“/”)
//返回值，0: 错误
//		其他： 列表地址
extern char* File_List(char * name);


//铁电操作
//初始化SPI FLASH的IO口
void FM25LXXB_Init(void);
void FRam_Exec(void);
//获取FRAM的操作状态
u8 FRAM_GetSta(void);
//writeaddr:开始写入的地址
//writebuff:数据存储区
//length:写入的字节数
u8 FRam_Write(u32 writeaddr, void* writebuff, u32 size);
//readaddr:开始读取的地址
//readbuff:数据存储区
//length:读取的字节数
u8 FRam_Read(u32 readaddr, void* Readbuff, u32 size);

/*独立看门狗*/
//定时器最小超时时间大约为1ms（cnt减1时间），最大超时（0xfff）大约4秒钟
void IWDG_Init(u16 cnt);
void IWDG_Feed(void);

/*窗口看门狗*/
void WWDG_Init(void);
void WWDG_Exec(void);
//立即重装载值，一定要在窗口允许刷新时使用，否则会引起复位
void WWDG_Set_Counter(u8 cnt);


//Plus
#define PULS_NUM				5			//脉冲（轴）数量

//ECode
#define ECD_NUM					4			//编码器数量
#define ECD_FLUSH_INTERVAL		1000		//编码器32位更新间隔，0.1ms(并不需要很高频率，只需要在16位溢出时间之内更新一次就好)


//GPIO
#define GPI_NUM					20	//I个数
#define GPO_NUM					16	//Q数量


#endif

