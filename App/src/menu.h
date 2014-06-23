#ifndef __MENU_H__
#define __MENU_H__
#include "config.h"
#include "lcd.h"
#include "sp_info.h"


void sp_disp_press_key(uint8 key_num, uint8 key_cnt);

void sp_disp_test(uint8 key_num, uint8 key_cnt);

//处理按键过程
void sp_do_sckey(uint8 keynum, sp_context* p_ctx);

//菜单显示
void sp_disp_menu_main(sp_context* p_ctx);

void sp_disp_menu_set(sp_context* p_ctx);

void sp_disp_menu_consume(sp_context* p_ctx);

#endif
