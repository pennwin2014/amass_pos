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
*** ������:		sp_read
*** ��������:		д������ˮ��flash
*** ����: 	������ˮ�ṹ��ָ��
*** ����:		����
*** ʱ��:		2014-07-03
*********************************************************/
uint8 sp_read_transdtl(uint8* ptransdtl);


/*******************************************************
*** ������:		sp_write_sysinfo
*** ��������:		дϵͳ������flash
*** ����: 	��Ҫд�ĸ�ҳ��ȫ������,��ҳ����ʼ��ַ
*** ����:		����
*** ʱ��:		2014-07-03
*********************************************************/
uint8 sp_write_sysinfo(uint8* p_ctx);



/*******************************************************
*** ������:		sp_write_sysinfo
*** ��������:		дϵͳ������flash
*** ����: 	��Ҫд�ĸ�ҳ��ȫ������,��ҳ����ʼ��ַ
*** ����:		����
*** ʱ��:		2014-07-03
*********************************************************/
uint8 sp_read_sysinfo(uint8* p_ctx);





#endif
