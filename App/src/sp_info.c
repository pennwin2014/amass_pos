#include "sp_info.h"

void sp_init_info(sp_context* p_ctx)
{
	memset(p_ctx, 0, sizeof(sp_context));
	//初始化流水存储区数据

	//初始化菜单
	sprintf(p_ctx->menu_info[0], "1. 设置");
	sprintf(p_ctx->menu_info[1],"2. 消费");
	sprintf(p_ctx->menu_info[2], "3. 签到/签退");
	sprintf(p_ctx->menu_info[3],"4. 关机");
	sprintf(p_ctx->menu_info[4], "5. 保留菜单一");
	sprintf(p_ctx->menu_info[5],"6. 保留菜单二");
	sprintf(p_ctx->menu_info[6],"7. 保留菜单三");
	sprintf(p_ctx->menu_info[7],"8. 保留菜单四");
	//初始化当前菜单序号
	p_ctx->menu_index = 0;
	p_ctx->menu_level = 0;
	p_ctx->key_cnt = 0;
	p_ctx->old_key_cnt = -1;
}



