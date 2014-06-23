#include "menu.h"

void sw_disp_menu()
{
	char cDispMsg[256];
	//第一个菜单
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"设置");
	LcdPrint(0, 0, cDispMsg, 0);
	//第二个菜单	
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"消费");
	LcdPrint(1, 0, cDispMsg, 0);
	//第三个菜单
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"签到/签退");
	LcdPrint(2, 0, cDispMsg, 0);
	//第四个菜单
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"关机");
	LcdPrint(3, 0, cDispMsg, 0); 
}