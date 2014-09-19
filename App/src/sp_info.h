#ifndef __SP_INFO_H
#define __SP_INFO_H
/*
**��������һЩ���õ�ͷ�ļ���ʵ�ֹ����йص�ͷ�ļ�����������
*/
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "config.h"
#include "M25PE16.h"
#include "sp_errdef.h"

//#define SP_DEBUG 0
//#define SP_DEBUG_MODE 0

typedef enum
{
  CPUCARD  = 0,
  M1CARD,
  SIMCARD,
  UNKNOWN_CARD
}
user_cardtype_t;


#define TRANSFLAG_OK 0x81
#define TRANSFLAG_UNCONFIRM 0x82
#define TRANSFLAG_LOSTCARD 0x13



#define CARD_SUPPORT_CPU (1<<7)//�൱�ڶ�������10000000
#define CARD_SUPPORT_M1 (1<<6)//�൱�ڶ�������1000000
#define CARD_SUPPORT_RFUIM (1<<5)//�൱�ڶ�������100000




#define MAX_FLASH_WRITE_CNT 20



#define MAX_MENU_SCREEN_CNT 3  //��Ļһ�ο�����ʾ�Ĳ˵���Ŀ
//#define MAX_SCREEN_WORDS (16*3)
#define MAX_SCREEN_WORDS (16*4)
#define MAX_SCREEN_ROWS 16
//������Ϣ������
#define SP_MB_OK 0
#define SP_MB_YES_NO 1

//����ģʽ
#define SP_WORK_MODE_NORMAL 1
#define SP_WORK_MODE_PERMENENT 2
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
#define SP_KEY_DOT '.'
#define SP_KEY_EQUAL 0x0d
#define SP_KEY_CONFIRM 0xb5
#define SP_KEY_CLEAR 0xb6
#define SP_KEY_MODE 0xb3
#define SP_KEY_FUNC 0xb4
//������Ϊ�Ϸ��·��İ�ť
#define SP_KEY_NEXT SP_KEY_ADD
#define SP_KEY_PREV SP_KEY_MINUS



//////////////��ַ///////////////////
#define ADDR_BLACKLIST 0x00000
#define ADDR_MASTER_TRANS_SEQNO 0x40000
#define ADDR_SLAVE_TRANS_SEQNO 0x40100
#define ADDR_SYSINFO 0x40200
#define ADDR_TRANS_DATA 0x46930
#define ADDR_TRANS_LAST 0xE2D30
///////ϵͳ����/////
#define OFFSET_WORK_MODE 0   //1byte,����ģʽ0����ţ�1����ֵ��2�����ۣ�
#define OFFSET_CONSUME_AMT 1  //4byte,���ѽ�� ���ڶ�ֵģʽ����Ч
#define OFFSET_CONSUME_GAP_SECONDS 5 //3byte,�������Ѽ����,0Ϊ�����ƣ� ��λ��ǰ
#define OFFSET_MAX_CARD_BAL 8 // 4byte, ������
#define OFFSET_MAX_CONSUME_AMT 12 // 4byte, ������ѽ� 0Ϊ�����ƣ���λ��ǰ
#define OFFSET_RESTART_TIME 16 // 8byte,��ʱ����������4�Σ�ÿ��ʱ��������ֽڣ���
//FF��ʾ������
//ʱ��ΪHEX��ʽ���磺13��30 ��ʾΪ 0x0D1
#define OFFSET_RETURN_FLAG 24 // 1byte,;//�˿�ܿ�����ر�
#define OFFSET_OFFLINE_FLAG 25 // 1byte,;//�ѻ�����ʱ������
#define OFFSET_MIN_CARD_BAL 26 // 2byte, //��С�����
#define OFFSET_TIMEOUT 28 // 1byte ����ʱʱ��
#define OFFSET_HEART_GAP 29 // 1byte,�������
#define OFFSET_SINGLE_CONSUME_LIMIT 30 // 4byte,���������޶�
#define OFFSET_DAY_SUM_COMSUME_LIMIT 34 // 4byte, /���ۼ������޶�
#define OFFSET_CARD_LIMIT_FLAG 38 // 1byte ,/���޿���
#define OFFSET_TERMNO 39 // 4byte,�����豸��
#define OFFSET_HD_VERSION 43 // 5byte, Ӳ���汾��
#define OFFSET_SYSTEM_CAPACITY 48 // 4byte, ϵͳ����
#define OFFSET_ADDR_SAMNO 52 //6byte, SAM����
#define OFFSET_ADDR_KEY_INDEX 58//1byte, ������Կ�汾��


/////////////////////////////////////////
#define SP_TP_REQUEST_CARD 0x01
#define SP_TP_USER_BALANCE 0x02
#define SP_TP_CONSUME_SUCCESS 0x03
#define SP_TP_INSUFFICIENT_BALANCE 0x04
#define SP_TP_CANCEL_CONSUME 0x05


#define SP_READ_FILE10 0x01//00000001B
#define SP_READ_FILE12 0x02//00000010B
#define SP_READ_FILE15 0x04//00000100B
#define SP_READ_FILE16 0x08//00001000B
///////////////��С/////////////////////
#define FLASH_PAGE_SIZE 256
#define MAX_SENDMSG_LEN 2048 //����ͳ���
#define MAX_DATA_LEN 207


#define SAMSLOT 1
#define SAMBAND 9600

/////////////�ṹ��/////////////////
typedef void (* menu_func_t)(void* arg);
/*
typedef struct
{
	const char* menu_desc_list[MAX_MENU_SCREEN_CNT];
	menu_func_t menu_func_list[MAX_MENU_SCREEN_CNT];
}page_info_t;
*/
#pragma pack(push) //�������״̬
#pragma pack(1)

typedef struct
{
  const char* menu_desc;//�˵�����
  menu_func_t menu_func;//�˵�����
} menu_info_t;


typedef struct
{
  uint32 last_trans_no;//���һ����ˮ��
  uint32 last_trans_addr;//���һ����ˮ��ַ
  uint8 date[3];//����
  uint32 sum_amt;//�����ۼƽ��
  uint8 rev[3];//����
  uint8 crc[2];//crc
} sp_transno_unit;

typedef struct
{
  uint32 termseqno;//4pos���ļ�¼��ˮ��
  uint32 cardno;//4�û���ˮ�ţ������׿��ţ�����λ��ǰ��
  uint16 lastcnt;// 2 ��һ�ν��״���
  uint8 lastlimamt[3];//��һ�ʵ�͸���޶�
  uint32 lastamount;//��һ�ʵĽ��׽��
  uint8 lasttransflag;//��һ�ʵĽ��ױ�־
  uint8 lasttermno[6];//��һ�ʵ��ն˱��
  uint8 lastdatetime[6];//��һ�ʵ�����ʱ��
  uint32 cardbefbal;//4����ǰ������λ��ǰ��
  uint16 cardbefcnt;//2����ǰ����������λ��ǰ��
  uint32 amount;//4���׽���λ��ǰ���������ۿۻ���ѽ��
  uint8 extraamount[3];//3���ѻ��ۿ۽���λ��ǰ��
  uint8 transdatetime[6];//6��������ʱ�� YYMMDDHHMMSS
  uint8 psamno[6];//6PSAM����
  uint8 tac[4];//4TAC��
  uint8 transflag;//1���ױ�־
  uint8 reserve[2];//2����
  uint8 crc[2];//2CRCУ��
} sp_transdtl;

typedef struct
{
  uint8 workmode;//����ģʽ0����ţ�1����ֵ��2�����ۣ�
  uint32 amount;//���ѽ��
  uint8 timegap[3];//��������ʱ����
  uint32 maxcardbal;//������
  uint32 maxamount;//������ѽ��
  uint8 restarttime[8];//��ʱ��������
  uint8 returnflag;//�˿�ܿ�����ر�
  uint8 offlineflag;//�ѻ�����ʱ������
  uint16 mincardbal;//��С�����
  uint8 timeout;//��ʱʱ��
  uint8 heartgap;//�������
  uint32 singleamountlim;//���������޶�
  uint32 dayamountlim;//���ۼ������޶�
  uint8 amountlimitflag;//���޿���
  uint8 termno[4];//�����豸��;
  uint8 hdversion[5];//Ӳ���汾
  uint32 systemcapacity;//ϵͳ����
  uint8 samno[6];//sam����
  uint8 key_index;//������Կ�汾��
  uint8 crc[2];//2�ֽ�У��λ
} sp_syspara;

typedef struct
{
  uint8 cardphyid[8];
  char username[32];
  char stuempno[20];
  uint32 cardno;
  uint8 cardtype;
  uint8 status;
  uint8 cardverno[6];
  uint8 feetype;
  uint8 expiredate[4];
  uint32 cardbefbal;
  uint16 paycnt;
  uint32 amount;
  uint8 keyver;
  uint8 keyindex;
  uint8 payrandom[4];
  uint8 tac[4];
  uint32 samseqno;
  uint8 day_sum_amt[4];//���ۼƽ��׽��
  uint8 last_trans_dev[6];
  uint8 last_trans_date[4];
  uint8 last_trans_time[3];
  uint8 last_trans_amt[4];
  uint8 last_trans_cnt[2];
  uint8 last_tac[4];
  // m1 card package block data
  uint8 m1_package_block[16];
} sp_card;

typedef struct
{
  uint32 base_amt;
  uint16 consume_interval;
  uint8 offline_day;
  uint8 blacklist_index;
  uint8 record_index;
  uint8 m1_alg;
  uint8 card_support;
  uint8 rfu[51];
  uint8 crc;
} sp_param_t;

typedef struct
{
  uint8 disp_type;
  // ��ѧ���Ʊ���
  char school_name[17];
  //�Ƿ�����
  bool online_flag;
  // �ն�����ID
  uint8 devphyid[4];
  // ����
  uint8 deviceno;
  uint16 sam_aid;
  // ���������ܶ�
  uint32 today_total_amt;
  // ��ǰ����ͳ������
  uint8 today[4];
  // �����ۼƽ��׽��
  uint32 yesterday_total_amt;
  // ����ʹ�ÿ����
  uint8 feetype_bitmap[32];
  // ���ʱ�
  uint8 feerate_table[256];
  // δ�ϴ���ˮ��С����
  uint8 offline_min_day[4];
  // ����Կ
  uint8 card_key[16];
  // �Ƿ�����ˮδ�ɼ�
  uint8 has_offline_rec;
  // ��ǰ��ˮ flash �Ƿ��Ѿ�����
  uint8 rec_flash_full;
  // d��ǰ�������汾
  uint8 cardverno[6];
  // ��������
  sp_param_t param;
  // ��������ַ
  uint32 blacklist_begin;
  uint32 blacklist_end;

  uint8 user_code[6];
  uint8 user_index;

  uint8 sam_ok;
  uint32 collect_record_ptr;
  // ��ǰ��ˮ
  sp_transdtl record;
  // ��ǰ��
  sp_card card;
  //ϵͳ����
  sp_syspara syspara;
  // ��ǰʱ��
  uint8 current_datetime[7];//yy-yy-mm-dd-hh-mi-ss
  SYSTIME systime;
} sp_context;

////////////////////////////////��������ͨѶ�ṹ��////////////////////////////////////////////

typedef struct
{
  uint8 guide_code[3];//������
  uint8 cmd_code;//������
  uint16 data_len;//���ݳ���
  uint8 machine_addr[2];//������ַ
  uint8 data[MAX_DATA_LEN];
  uint8 check_sum;
} sp_tcp_trans_unit;
#pragma pack(pop)//�ָ�����״̬


#endif
