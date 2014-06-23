#include "menu.h"


void sp_disp_test(uint8 key_num, uint8 key_cnt)
{
	char dispmsg[100];
	memset(dispmsg, 0, 100);
	sprintf(dispmsg, "key=%02x", key_num);
	LcdPrint(0, 10, dispmsg, 0);
	memset(dispmsg, 0, 100);
	sprintf(dispmsg, "cnt=%d", key_cnt);
	LcdPrint(1, 10, dispmsg, 0);
}

void sp_disp_press_key(uint8 key_num, uint8 key_cnt)
{
	char dispmsg[100];
	memset(dispmsg, 0, 100);
	sprintf(dispmsg, "hex=%x,dec=%d", key_num,key_cnt);
	LcdPrint(1, 0, dispmsg, 0);
}

void sp_disp_menu(uint8 start_index, sp_context* p_ctx)
{
	int i = 0;
	LcdCLS(0xFF);
	for(i=0; i<MAX_MENU_LIST_CNT; i++)
	{		
		LcdPrint(i, 0, p_ctx->menu_info[(start_index+i)%MENU_CNT], 0);
	}
}
