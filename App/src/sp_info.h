#ifndef __SP_INFO_H 
#define __SP_INFO_H
#include "config.h"
#include "string.h"
#include "stdio.h"

#define MAX_MENU_LENGTH 56 //�˵��������
#define MAX_MENU_SCREEN_CNT 4	 //��Ļһ�ο�����ʾ�Ĳ˵���Ŀ
#define MAX_MENU_CNT 8//��Ҫ��ʾ�Ĳ˵�����


//���¶�����̰���
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




typedef struct
{
	uint8 menu_level;//�˵�����,0:���˵�   1:�ڶ�����˵�
	uint8 menu_index;//�˵����
	int8 key_cnt;
	int8 old_key_cnt;
   	char menu_info[MAX_MENU_CNT][MAX_MENU_LENGTH];
}sp_context;

void sp_init_info(sp_context* p_ctx);




#endif
