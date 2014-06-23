#ifndef __MENU_H__
#define __MENU_H__
#include "config.h"
#include "lcd.h"
#include "sp_info.h"

void sp_disp_menu(uint8 start_index, sp_context* p_ctx);

void sp_disp_press_key(uint8 key_num, uint8 key_cnt);

void sp_disp_test(uint8 key_num, uint8 key_cnt);

#endif
