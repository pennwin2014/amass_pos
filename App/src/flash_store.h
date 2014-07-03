#ifndef __FLASH_STORE_H__
#define __FLASH_STORE_H__
#include "sp_info.h"


uint32 sp_get_next_transdtl_addr(uint32 transdtl_addr);
uint8 sp_do_write_card(uint8* ptransdtl);
uint8 sp_update_left_transdtl_info(uint8* ptransdtl);



uint8 sp_write_transno_unit(bool flag, uint8* punit);
uint8 sp_read_transno_unit(bool flag, uint8* punit);



uint8 sp_write_transdtl(uint8* ptransdtl);




/*******************************************************
*** 函数名:		sp_read
*** 函数功能:		写交易流水到flash
*** 参数: 	交易流水结构体指针
*** 作者:		汪鹏
*** 时间:		2014-07-03
*********************************************************/
uint8 sp_read_transdtl(uint8* ptransdtl);


/*******************************************************
*** 函数名:		sp_write_sysinfo
*** 函数功能:		写系统参数到flash
*** 参数: 	需要写的该页的全部内容,该页的起始地址
*** 作者:		汪鹏
*** 时间:		2014-07-03
*********************************************************/
uint8 sp_write_sysinfo(uint8* p_ctx);



/*******************************************************
*** 函数名:		sp_write_sysinfo
*** 函数功能:		写系统参数到flash
*** 参数: 	需要写的该页的全部内容,该页的起始地址
*** 作者:		汪鹏
*** 时间:		2014-07-03
*********************************************************/
uint8 sp_read_sysinfo(uint8* p_ctx);





#endif
