#ifndef __SP_INFO_H
#define __SP_INFO_H
/*
**仅仅包含一些公用的头文件，实现功能有关的头文件不包含在这
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



#define CARD_SUPPORT_CPU (1<<7)//相当于二进制数10000000
#define CARD_SUPPORT_M1 (1<<6)//相当于二进制数1000000
#define CARD_SUPPORT_RFUIM (1<<5)//相当于二进制数100000




#define MAX_FLASH_WRITE_CNT 20



#define MAX_MENU_SCREEN_CNT 3  //屏幕一次可以显示的菜单项目
//#define MAX_SCREEN_WORDS (16*3)
#define MAX_SCREEN_WORDS (16*4)
#define MAX_SCREEN_ROWS 16
//定义消息框类型
#define SP_MB_OK 0
#define SP_MB_YES_NO 1

//消费模式
#define SP_WORK_MODE_NORMAL 1
#define SP_WORK_MODE_PERMENENT 2
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
#define SP_KEY_DOT '.'
#define SP_KEY_EQUAL 0x0d
#define SP_KEY_CONFIRM 0xb5
#define SP_KEY_CLEAR 0xb6
#define SP_KEY_MODE 0xb3
#define SP_KEY_FUNC 0xb4
//定义作为上翻下翻的按钮
#define SP_KEY_NEXT SP_KEY_ADD
#define SP_KEY_PREV SP_KEY_MINUS



//////////////地址///////////////////
#define ADDR_BLACKLIST 0x00000
#define ADDR_MASTER_TRANS_SEQNO 0x40000
#define ADDR_SLAVE_TRANS_SEQNO 0x40100
#define ADDR_SYSINFO 0x40200
#define ADDR_TRANS_DATA 0x46930
#define ADDR_TRANS_LAST 0xE2D30
///////系统参数/////
#define OFFSET_WORK_MODE 0   //1byte,工作模式0：编号，1：定值，2：单价，
#define OFFSET_CONSUME_AMT 1  //4byte,消费金额 ，在定值模式下有效
#define OFFSET_CONSUME_GAP_SECONDS 5 //3byte,两次消费间隔秒,0为不限制， 低位在前
#define OFFSET_MAX_CARD_BAL 8 // 4byte, 最大卡余额
#define OFFSET_MAX_CONSUME_AMT 12 // 4byte, 最大消费金额， 0为不限制，低位在前
#define OFFSET_RESTART_TIME 16 // 8byte,定时重新启动（4段，每个时间段两个字节），
//FF表示结束，
//时间为HEX格式例如：13：30 表示为 0x0D1
#define OFFSET_RETURN_FLAG 24 // 1byte,;//退款功能开启与关闭
#define OFFSET_OFFLINE_FLAG 25 // 1byte,;//脱机消费时间限制
#define OFFSET_MIN_CARD_BAL 26 // 2byte, //最小卡余额
#define OFFSET_TIMEOUT 28 // 1byte ，超时时间
#define OFFSET_HEART_GAP 29 // 1byte,心跳间隔
#define OFFSET_SINGLE_CONSUME_LIMIT 30 // 4byte,单笔消费限额
#define OFFSET_DAY_SUM_COMSUME_LIMIT 34 // 4byte, /日累计消费限额
#define OFFSET_CARD_LIMIT_FLAG 38 // 1byte ,/卡限开关
#define OFFSET_TERMNO 39 // 4byte,本机设备号
#define OFFSET_HD_VERSION 43 // 5byte, 硬件版本号
#define OFFSET_SYSTEM_CAPACITY 48 // 4byte, 系统容量
#define OFFSET_ADDR_SAMNO 52 //6byte, SAM卡号
#define OFFSET_ADDR_KEY_INDEX 58//1byte, 消费密钥版本号


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
///////////////大小/////////////////////
#define FLASH_PAGE_SIZE 256
#define MAX_SENDMSG_LEN 2048 //最大发送长度
#define MAX_DATA_LEN 207


#define SAMSLOT 1
#define SAMBAND 9600

/////////////结构体/////////////////
typedef void (* menu_func_t)(void* arg);
/*
typedef struct
{
	const char* menu_desc_list[MAX_MENU_SCREEN_CNT];
	menu_func_t menu_func_list[MAX_MENU_SCREEN_CNT];
}page_info_t;
*/
#pragma pack(push) //保存对齐状态
#pragma pack(1)

typedef struct
{
  const char* menu_desc;//菜单名称
  menu_func_t menu_func;//菜单函数
} menu_info_t;


typedef struct
{
  uint32 last_trans_no;//最后一笔流水号
  uint32 last_trans_addr;//最后一笔流水地址
  uint8 date[3];//日期
  uint32 sum_amt;//当日累计金额
  uint8 rev[3];//保留
  uint8 crc[2];//crc
} sp_transno_unit;

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
} sp_transdtl;

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
  uint8 amountlimitflag;//卡限开关
  uint8 termno[4];//本机设备号;
  uint8 hdversion[5];//硬件版本
  uint32 systemcapacity;//系统容量
  uint8 samno[6];//sam卡号
  uint8 key_index;//消费密钥版本号
  uint8 crc[2];//2字节校验位
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
  uint8 day_sum_amt[4];//日累计交易金额
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
  // 大学名称标题
  char school_name[17];
  //是否在线
  bool online_flag;
  // 终端物理ID
  uint8 devphyid[4];
  // 机号
  uint8 deviceno;
  uint16 sam_aid;
  // 当日消费总额
  uint32 today_total_amt;
  // 当前消费统计日期
  uint8 today[4];
  // 昨天累计交易金额
  uint32 yesterday_total_amt;
  // 允许使用卡类别
  uint8 feetype_bitmap[32];
  // 费率表
  uint8 feerate_table[256];
  // 未上传流水最小日期
  uint8 offline_min_day[4];
  // 卡密钥
  uint8 card_key[16];
  // 是否有流水未采集
  uint8 has_offline_rec;
  // 当前流水 flash 是否已经存满
  uint8 rec_flash_full;
  // d当前黑名单版本
  uint8 cardverno[6];
  // 基础参数
  sp_param_t param;
  // 黑名单地址
  uint32 blacklist_begin;
  uint32 blacklist_end;

  uint8 user_code[6];
  uint8 user_index;

  uint8 sam_ok;
  uint32 collect_record_ptr;
  // 当前流水
  sp_transdtl record;
  // 当前卡
  sp_card card;
  //系统参数
  sp_syspara syspara;
  // 当前时间
  uint8 current_datetime[7];//yy-yy-mm-dd-hh-mi-ss
  SYSTIME systime;
} sp_context;

////////////////////////////////定义网络通讯结构体////////////////////////////////////////////

typedef struct
{
  uint8 guide_code[3];//引导码
  uint8 cmd_code;//命令码
  uint16 data_len;//数据长度
  uint8 machine_addr[2];//机器地址
  uint8 data[MAX_DATA_LEN];
  uint8 check_sum;
} sp_tcp_trans_unit;
#pragma pack(pop)//恢复对齐状态


#endif
