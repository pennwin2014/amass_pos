#include "menu.h"

void sw_disp_menu()
{
	char cDispMsg[256];
	//��һ���˵�
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"����");
	LcdPrint(0, 0, cDispMsg, 0);
	//�ڶ����˵�	
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"����");
	LcdPrint(1, 0, cDispMsg, 0);
	//�������˵�
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"ǩ��/ǩ��");
	LcdPrint(2, 0, cDispMsg, 0);
	//���ĸ��˵�
	memset(cDispMsg, 0, 256);
	sprintf(cDispMsg,"�ػ�");
	LcdPrint(3, 0, cDispMsg, 0); 
}