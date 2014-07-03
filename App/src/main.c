#include "config.h"
#include "Mifare.h"
#include "can.h"
#include "sp_info.h"
#include "menu.h"

#define HD_POS HD_SC
//#define USE_PSAM
//#define TEST_FLASH

uint16 nCode = 0;
uint8 gcComBuf[1024];
SYSTIME gSysTime;
sp_context sp_main_ctx;



__inline void DelayNS2(int dly)
{
	int i;

	for (i=0; i<dly; i++);
	
}


void ByteToDecStr(unsigned char cValue, unsigned char *pChar)
{
	unsigned char i;
	for(i=0; i<3; i++)
	{
		pChar[2-i] = cValue % 10 + '0';
		cValue /= 10;
	}
	pChar[3] = '\0';
}	
		
		
		
void ReceiveData(void *pBuf, uint16 nLen)
{
	memcpy(gcComBuf, (uint8 *)pBuf, nLen);//接收PC数据
	COM_SendBuf(COM0, gcComBuf, nLen);   //送回PC
}

void InitCom(void)
{
	DCB sDCB;
	sDCB.baudrate 	= 115200;	  
	sDCB.datab		= 8;
	sDCB.parity 	= 0;
	sDCB.stopb		= 1;
	sDCB.limitlen	= 1000;
	sDCB.timeout 	= 100;//300
	sDCB.pRecvfuc   = ReceiveData;
	// 初始化串口
	StartComm(COM0, &sDCB);
	sDCB.baudrate = 38400;
	StartComm(COM1, &sDCB);
}

uint8 get_test_comm_data(uint8* comm_data)
{
//控制字
	comm_data[0] = 0x88;
//id
	comm_data[1] = 0x12;
	comm_data[2] = 0x34;
	comm_data[3] = 0x56;
	comm_data[4] = 0x78;
//
	comm_data[5] = 0x11;
	comm_data[6] = 0x22;
	comm_data[7] = 0x33;
	comm_data[8] = 0x44;
	comm_data[9] = 0x55;
	comm_data[10] = 0x66;
	comm_data[11] = 0x77;
	comm_data[12] = 0x88;
	return 1;
}


int main()
{
	uint8 cSendBuf[7] = {0x00, 0xA4, 0x00, 0x00, 0x00, 0x3F, 0x00};
	uint8 cRcvBuf[64], cRcvLen;
	uint8 press_key_num = 0;

	InitBoard();
	Beep(3);

	//看门狗初始化
	#ifdef OPEN_WDT
	InitWatchDog();
	#endif

	GetTime(&gSysTime);
	//串口设置
	InitCom();

	#ifdef USE_PSAM
	{
		uint8 cPsamBuf[7] = {0x00, 0xA4, 0x00, 0x00, 0x02, 0xDF, 0x01};
		uint8 cPsamBuf1[5] = {0x00, 0xb0, 0x95, 0x00, 0x08};
		Init_Psam();
		Rst_Psam_Card(cRcvBuf, &cRcvLen);
		DelayNS2(50000);
		//针对不同厂家卡片需做适当延迟后才进行apdu操作
		nCode = CosCommand(cPsamBuf, 7, cRcvBuf, &cRcvLen);
		nCode = CosCommand(cPsamBuf1, 5, cRcvBuf, &cRcvLen);
	}
	#endif

	
	//初始化全局结构体
	sp_init_info(&sp_main_ctx);
	load_sys_info(&sp_main_ctx);
	while(1)
	{
		press_key_num = GetSCKEY();
		sp_do_sckey(press_key_num, &sp_main_ctx);

		if(ResetCard_A())
		{
			nCode = CpuCard_Apdu(0x0f, 7, cSendBuf, &cRcvLen, cRcvBuf);
			Beep(3);
			Deselect_A();
			Reset_Reader_TimeOut();
		}
			
		//喂狗定义防止溢出程序复位		
		KillWatchDog();
	}	
}
