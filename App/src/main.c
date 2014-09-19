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
  sp_wait_for_retkey(SP_KEY_CONFIRM, disp_msg, __FUNCTION__, __LINE__, "按确定键继续");
  while(1)
  {
    //1、复位
    ret = sp_cpucard_select();
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "复位ret=%d,按确定键继续", ret);
    //2、读卡
    ret = sp_read_card(p_ctx, &card);
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "读卡ret=%d,按确定键继续", ret);
    //3、取消卡片选择
    ret = sp_cpucard_deselect();
    //4、寻卡
    ret = sp_request_card(cardphyid, &card.cardtype);
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "寻卡ret=%d,按确定键继续", ret);

  }
}
#endif
static int do_init_sam_card(sp_context* p_ctx)
{
  int ret = 0;
  uint8 samno[6];

  //初始化psam卡
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
  //看门狗初始化
#ifdef OPEN_WDT
  InitWatchDog();
#endif
  //串口设置
  sp_InitCom();
  //测试模式
#ifdef TEST_MODE
  test_deselect_card(&sp_main_ctx);
#endif
  // 上电后检测POS 机参数
  //0、初始化系统存储区
  ret = sp_check_context(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "检查全局系统参数失败!!按确认键退出程序,ret=%d", ret);
    return 1;
  }
  //1、初始化全局结构体
  ret = sp_init_syspara(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "初始化全局系统参数失败!!按确认键退出程序,ret=%d", ret);
    return 1;
  }
  //2、初始化流水存储区
  ret = sp_init_transdtl(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "初始化流水存储区失败,ret=%d", ret);
    return 1;
  }
  //3、初始化psam卡
  ret = do_init_sam_card(&sp_main_ctx);
  if(ret)
  {
    sp_wait_for_retkey(SP_KEY_CONFIRM, __FUNCTION__, __LINE__, "初始化psam卡失败!!按确认键退出程序");
    return 1;
  }
  //消费菜单
  sp_menu_consume(&sp_main_ctx);
  //主菜单
  //sp_menu_main(&sp_main_ctx);
  return 0;
}
