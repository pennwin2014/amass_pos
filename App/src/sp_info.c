#include "sp_info.h"

void sp_init_info(sp_context* p_ctx)
{
	memset(p_ctx, 0, sizeof(sp_context));
	//��ʼ����ˮ�洢������

	//��ʼ���˵�
	sprintf(p_ctx->menu_info[0], "1. ����");
	sprintf(p_ctx->menu_info[1],"2. ����");
	sprintf(p_ctx->menu_info[2], "3. ǩ��/ǩ��");
	sprintf(p_ctx->menu_info[3],"4. �ػ�");
	sprintf(p_ctx->menu_info[4], "5. �����˵�һ");
	sprintf(p_ctx->menu_info[5],"6. �����˵���");
	sprintf(p_ctx->menu_info[6],"7. �����˵���");
	sprintf(p_ctx->menu_info[7],"8. �����˵���");
	//��ʼ����ǰ�˵����
	p_ctx->menu_index = 0;
	p_ctx->menu_level = 0;
	p_ctx->key_cnt = 0;
	p_ctx->old_key_cnt = -1;
}



