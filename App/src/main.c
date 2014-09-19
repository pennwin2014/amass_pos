#include "config.h"
#include "Mifare.h"
#include "sp_disp.h"

#include "sp_communicate.h"
#include "sp_menu.h"


#define HD_POS HD_SC
#ifdef TEST_MODE
static void test_deselect_card(sp_context * p_ctx)
{
  int ret = 0;
  uint8 cardphyid[8];
  sp_card card;
  sp_wait_for_retkey(SP_KEY_CONFIRM, disp_msg, __FUNCTION__, __LINE__, "��ȷ��������");
  while(1)
  {
    //1����λ
    ret = sp_cpucard_select();
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "��λret=%d,��ȷ��������", ret);
    //2������
    ret = sp_read_card(p_ctx, &card);
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "����ret=%d,��ȷ��������", ret);
    //3��ȡ����Ƭѡ��
    ret = sp_cpucard_deselect();
    //4��Ѱ��
    ret = sp_request_card(cardphyid, &card.cardtype);
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "Ѱ��ret=%d,��ȷ��������", ret);

  }
}
#endif
static int do_init_sam_card(sp_context* p_ctx)
{
  int ret = 0;
  uint8 samno[6];

  //��ʼ��psam��
  ret = sp_init_sam_card(p_ctx, samno);
  if(ret)
    return ret;
  if(memcmp(samno, p_ctx->syspara.samno, 6) != 0)
  {
    memcpy(p_ctx->syspara.samno, samno, 6);
    ret = sp_write_syspara(p_ctx);
    if(ret)
    {
      sp_disp_msg(1000, "samno write fail,samno=%s", samno);
      return ret;
    }
  }
  return 0;
}
int main()
{
  sp_context sp_main_ctx;
  int ret = 0;
  InitBoard();
  Beep(3);
  //���Ź���ʼ��
#ifdef OPEN_WDT
  InitWatchDog();
#endif
  //��������
  sp_InitCom();
  //����ģʽ
#ifdef TEST_MODE
  test_deselect_card(&sp_main_ctx);
#endif
  // �ϵ����POS ������
  //0����ʼ��ϵͳ�洢��
  ret = sp_check_context(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "���ȫ��ϵͳ����ʧ��!!��ȷ�ϼ��˳�����,ret=%d", ret);
    return 1;
  }
  //1����ʼ��ȫ�ֽṹ��
  ret = sp_init_syspara(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "��ʼ��ȫ��ϵͳ����ʧ��!!��ȷ�ϼ��˳�����,ret=%d", ret);
    return 1;
  }
  //2����ʼ����ˮ�洢��
  ret = sp_init_transdtl(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "��ʼ����ˮ�洢��ʧ��,ret=%d", ret);
    return 1;
  }
  //3����ʼ��psam��
  ret = do_init_sam_card(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "��ʼ��psam��ʧ��!!��ȷ�ϼ��˳�����");
    return 1;
  }
  //���Ѳ˵�
  sp_menu_consume(&sp_main_ctx);
  //���˵�
  //sp_menu_main(&sp_main_ctx);
  return 0;
}
