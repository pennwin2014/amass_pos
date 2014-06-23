#ifndef __PARA_H__
#define __PARA_H__
#include "config.h"

//////////////��ַ///////////////////
#define ADDR_BLACKLIST 0x00000
#define ADDR_MASTER_TRANS_SEQNO 0x40000
#define ADDR_SLAVE_TRANS_SEQNO 0x40108
#define ADDR_SYSINFO 0x40210
#define ADDR_TRANS_DATA 0x46930
#define ADDR_TRANS_LAST 0xBBC30

///////////////��С/////////////////////
#define FLASH_PAGE_SIZE 264

/////////////�ṹ��/////////////////


typedef struct
{
	uint32 trans_no;
	uint32 trans_addr;
	uint8 crc[2];
}sp_transno_unit;

typedef struct
{
	uint32 termseqno;//4pos���ļ�¼��ˮ��
	uint32 cardno;//4�û���ˮ�ţ������׿��ţ�����λ��ǰ��
	uint32 cardbefbal;//4����ǰ������λ��ǰ��
	uint16 cardbefcnt;//2����ǰ����������λ��ǰ��
	uint32 amount;//4���׽���λ��ǰ���������ۿۻ���ѽ��
	uint8 extraamount[3];//3���ѻ��ۿ۽���λ��ǰ��
	uint8 transdatetime[6];//6��������ʱ�� YYMMDDHHMMSS
	uint8 psamno[6];//6PSAM����
	uint8 tac[4];//4TAC��
	uint8 termid[4];//4����
	uint8 transflag;//1���ױ�־
	uint8 reserve[4];//4����
	uint8 crc[2];//2CRCУ��
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
