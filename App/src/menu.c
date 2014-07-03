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
//////////////////////////菜单显示////////////////////////////////////

void sp_disp_menu_main(sp_context* p_ctx)
{
	int i = 0;
	int8 start_index = p_ctx->key_cnt;
	if(start_index+MAX_MENU_SCREEN_CNT>MAX_MENU_CNT)
	{
		start_index = MAX_MENU_CNT - MAX_MENU_SCREEN_CNT;
	}
	else if(start_index<0)
	{
		start_index = 0;
	}
	LcdCLS(0xFF);
	for(i=0; i<MAX_MENU_SCREEN_CNT; i++)
	{		
		LcdPrint(i, 0, p_ctx->menu_info[start_index+i], 0);
	}
}

void sp_disp_menu_set(sp_context* p_ctx)
{
	LcdCLS(0xFF);
	LcdPrint(2, 0, p_ctx->menu_info[0], 0);
	/*
	int i = 0;
	int8 start_index = p_ctx->key_cnt;
	if(start_index+MAX_MENU_SCREEN_CNT>MAX_MENU_CNT)
	{
		start_index = MAX_MENU_CNT - MAX_MENU_SCREEN_CNT;
	}
	else if(start_index<0)
	{
		start_index = 0;
	}
	LcdCLS(0xFF);
	for(i=0; i<MAX_MENU_SCREEN_CNT; i++)
	{		
		LcdPrint(i, 0, p_ctx->menu_info[start_index+i], 0);
	}
	*/
	
}

void sp_disp_menu_consume(sp_context* p_ctx)
{
	
}

////////////////////处理按键相关//////////////////////////////
void sp_do_sckey(uint8 keynum, sp_context* p_ctx)
{
	if(p_ctx->menu_level == 0)
	{
		switch(keynum)
		{
			case SP_KEY_1:
				p_ctx->menu_level = 1;
				p_ctx->menu_index = 1;
				break;
			case SP_KEY_2:
				p_ctx->menu_level = 1;
				p_ctx->menu_index = 2;
				break;
			case SP_KEY_0:
				p_ctx->menu_level = 0;
				p_ctx->menu_level = 1;
		}
		if(keynum == SP_KEY_ADD)
		{
			if(p_ctx->key_cnt<MAX_MENU_CNT-MAX_MENU_SCREEN_CNT)
				p_ctx->key_cnt++;
		}
		else if(keynum == SP_KEY_MINUS)
		{
			if(p_ctx->key_cnt>0)
				p_ctx->key_cnt--;
		}

		if(p_ctx->key_cnt!=p_ctx->old_key_cnt)
		{
			sp_disp_menu_main(p_ctx);
			p_ctx->old_key_cnt = p_ctx->key_cnt;
		}
	}
	//根据所在的层级以及序号处理按键
	else if(p_ctx->menu_level == 1)
	{
		
		switch(p_ctx->menu_index)
		{
			case 1:
				sp_disp_menu_set(p_ctx);
				break;
			default:
				break;
				
		}	
	}
	
}
