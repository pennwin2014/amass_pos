#ifndef __PARA_H__
#define __PARA_H__
#include "config.h"

//////////////地址///////////////////
#define ADDR_BLACKLIST 0x00000
#define ADDR_MASTER_TRANS_SEQNO 0x40000
#define ADDR_SLAVE_TRANS_SEQNO 0x40108
#define ADDR_SYSINFO 0x40210
#define ADDR_TRANS_DATA 0x46930
#define ADDR_TRANS_LAST 0xBBC30

///////////////大小/////////////////////
#define FLASH_PAGE_SIZE 264

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
	uint32 cardbefbal;//4交易前卡余额（低位在前）
	uint16 cardbefcnt;//2交易前卡次数（低位在前）
	uint32 amount;//4交易金额（低位在前），包括折扣或搭伙费金额
	uint8 extraamount[3];//3搭伙费或折扣金额（低位在前）
	uint8 transdatetime[6];//6交易日期时间 YYMMDDHHMMSS
	uint8 psamno[6];//6PSAM卡号
	uint8 tac[4];//4TAC码
	uint8 termid[4];//4机号
	uint8 transflag;//1交易标志
	uint8 reserve[4];//4保留
	uint8 crc[2];//2CRC校验
}sp_st_transdtl;

typedef struct 
{ 
	uint8 workmode;
	uint32 amount;
	uint8 timegap[3];
	uint32 maxcardbal;
	uint32 maxamount;
	uint8 restarttime[8];				
	uint8 returnflag;
	uint8 offlineflag;
	uint16 mincardbal;
	uint8 timeout;
	uint8 heartgap;
	uint32 singleamountlim;
	uint32 dayamountlim;
	uint8 amountlimitflag;

}sys_init_para;


#endif
