#ifndef __SP_INFO_H 
#define __SP_INFO_H

#include "string.h"
#include "stdio.h"
#include "config.h"
#include "M25PE16.h"

//#include "flash_store.h"


//常规
/*
enum SP_ERR_TYPE
{
   	SP_FLASH_FAIL = 0,
   	SP_FLASH_SUCCESS = 1,
   	SP_FLASH_NOT_FOUNT = 2,
	SP_FLASH_ERROR = 3
}SP_ERR_VAL;
*/

#define SP_FLASH_SUCCESS 1
#define SP_FLASH_FAIL 0
#define SP_FLASH_NOT_FOUNT 2
#define SP_FLASH_ERROR 3


#define MAX_FLASH_WRITE_CNT 20



#define MAX_MENU_LENGTH 56 //菜单名最长长度
#define MAX_MENU_SCREEN_CNT 4	 //屏幕一次可以显示的菜单项目
#define MAX_MENU_CNT 8//需要显示的菜单总数


//重新定义键盘按键
#define SP_KEY_0 '0'
#define SP_KEY_1 '1'
#define SP_KEY_2 '2'
#define SP_KEY_3 '3'
#define SP_KEY_4 '4'
#define SP_KEY_5 '5'
#define SP_KEY_6 '6'
#define SP_KEY_7 '7'
#define SP_KEY_8 '8'
#define SP_KEY_9 '9'
#define SP_KEY_ADD '+'//0x0b
#define SP_KEY_MINUS '-'//0x0d
#define SP_KEY_MUL '*'
#define SP_KEY_EQUAL 0x0d
#define SP_KEY_CONFIRM 81
#define SP_KEY_CLEAR 82
#define SP_KEY_MOD 79
#define SP_KEY_FUNC 80


//////////////地址///////////////////
#define ADDR_BLACKLIST 0x00000
#define ADDR_MASTER_TRANS_SEQNO 0x40000
#define ADDR_SLAVE_TRANS_SEQNO 0x40108
#define ADDR_SYSINFO 0x40210
#define ADDR_TRANS_DATA 0x46930
#define ADDR_TRANS_LAST 0xBBC30



///////////////大小/////////////////////
#define FLASH_PAGE_SIZE 256

/////////////结构体/////////////////


typedef struct
{
	uint32 trans_no;
	uint32 trans_addr;
	uint8 crc[2];
}sp_transno_unit;

typedef struct
{
	uint32 termseqno;//4pos机的记录流水号
	uint32 cardno;//4用户流水号（即交易卡号）（低位在前）
	uint16 lastcnt;// 2 上一次交易次数
	uint8 lastlimamt[3];//上一笔的透资限额
	uint32 lastamount;//上一笔的交易金额
	uint8 lasttransflag;//上一笔的交易标志
	uint8 lasttermno[6];//上一笔的终端编号
	uint8 lastdatetime[6];//上一笔的日期时间
	uint32 cardbefbal;//4交易前卡余额（低位在前）
	uint16 cardbefcnt;//2交易前卡次数（低位在前）
	uint32 amount;//4交易金额（低位在前），包括折扣或搭伙费金额
	uint8 extraamount[3];//3搭伙费或折扣金额（低位在前）
	uint8 transdatetime[6];//6交易日期时间 YYMMDDHHMMSS
	uint8 psamno[6];//6PSAM卡号
	uint8 tac[4];//4TAC码
	uint8 transflag;//1交易标志
	uint8 reserve[2];//2保留
	uint8 crc[2];//2CRC校验
}sp_st_transdtl;

typedef struct 
{ 
	uint8 workmode;//工作模式0：编号，1：定值，2：单价，
	uint32 amount;//消费金额
	uint8 timegap[3];//两次消费时间间隔
	uint32 maxcardbal;//最大卡余额
	uint32 maxamount;//最大消费金额
	uint8 restarttime[8];//定时重新启动				
	uint8 returnflag;//退款功能开启与关闭
	uint8 offlineflag;//脱机消费时间限制
	uint16 mincardbal;//最小卡余额
	uint8 timeout;//超时时间
	uint8 heartgap;//心跳间隔
	uint32 singleamountlim;//单笔消费限额
	uint32 dayamountlim;//日累计消费限额
	uint8 amountlimitflag;//卡线开关
	uint8 crc[2];//2字节校验位
}sys_init_para;




typedef struct
{
	uint8 menu_level;//菜单级数,0:主菜单   1:第二级别菜单
	uint8 menu_index;//菜单序号
	int8 key_cnt;
	int8 old_key_cnt;
   	char menu_info[MAX_MENU_CNT][MAX_MENU_LENGTH];
	//系统参数
	sys_init_para sp_sysinfo;
}sp_context;

void sp_init_info(sp_context* p_ctx);


void load_sys_info(sp_context* p_ctx);

uint16 sp_get_transno_lastaddr(uint16 pageno);

void pb_protocol_crc(const uint8* buffer, uint16 buffer_len, uint8 crc[2]);


#endif
