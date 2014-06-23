#include "config.h"
#include "Mifare.h"
#include "para.h"
#include "can.h"
#include "menu.h"

#define HD_POS HD_SC
//#define USE_PSAM
//#define TEST_FLASH

uint16 nCode = 0;
uint8 gcComBuf[1024];
SYSTIME gSysTime;

//void sp_show_menu(sp_context* pos);


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


void DispTime(void)
{
#if (HD_POS == HD_SC)
	uint8  cTime[20];
    GetTime(&gSysTime);	
	ByteToDecStr(gSysTime.chMonth, cTime);
	ByteToDecStr(gSysTime.chDay, &cTime[3]);
	cTime[3] = '-';
	ByteToDecStr(gSysTime.chHour, &cTime[6]);
	ByteToDecStr(gSysTime.chMinute, &cTime[9]);
	ByteToDecStr(gSysTime.chSecond, &cTime[12]);
	cTime[0] = cTime[6] = cTime[15] = ' ';
	cTime[9] = cTime[12] = ':';
	cTime[16] = 0;
//	memset(cTime,0,16);
//	sprintf(cTime,"supwisdom",9);
	LcdPrint(3, 0,(char *) cTime, 0);

#elif (HD_POS == HD_S2)
	unsigned char  cTime[16];
	GetTime(&gSysTime);	
	cTime[0] = gSysTime.chHour / 10;			//Hour
	cTime[1] = gSysTime.chHour % 10 ;	
	cTime[3] = gSysTime.chMinute / 10; 			//Minute
	cTime[4] = gSysTime.chMinute % 10;
	cTime[6] = gSysTime.chSecond / 10; 		//Second
	cTime[7] = gSysTime.chSecond % 10;
/*
	cTime[8] = (gSysTime.chYear % 100) /10;	//Year
	cTime[9] = (gSysTime.chYear % 100) %10;	
	cTime[11] = gSysTime.chMonth/ 10; 			//Month
	cTime[12] = gSysTime.chMonth % 10;
	cTime[14] = gSysTime.chDay / 10; 			//Day
	cTime[15] = gSysTime.chDay % 10;*/
#endif	
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
	uint8 ret1,ret2,ret3;
	int32 cnt = 0;
	uint8 writebuf[64];
	uint8 comm_data[128] = {0};

	memset(writebuf, 0, 64);

	
	InitBoard();
	Beep(3);

	sprintf((char*)writebuf, "i like pizzar");
	
	ret3 = IsFlashBusy();
	if(ret3 == 1)
	{
		//sprintf(cDispMsg,"flash is busy");	
	}
	else
	{
		//SF_ErasePage(1);
		ret0 = SF_Write(1, 0, writebuf, 64, 1);
		//ret2 = SF_Read(1, 0, cRcvBuf, 64);
		//sprintf(cDispMsg, "%d,%d,%s", ret1, ret2, cRcvBuf);
		
		ret1 = SF_Read(0, 0, cRcvBuf, 64);
		ret2 = SF_Read(1, 0, writebuf, 64);
		//sprintf(cDispMsg, "%d,%d,%s,%s", ret0, ret2, cRcvBuf, writebuf);
	}

	

	#ifdef TEST_FLASH
	//Flash读写操作
	SF_ErasePage(0);
	SF_Read(0,0, cFlashBuf, 254);
	memset(cFlashBuf, 0xAA, 254);
	SF_Write(0,0,cFlashBuf,254, 1);
	memset(cFlashBuf, 0x00, 254);
	SF_Read(0,0, cFlashBuf, 254);
	#endif

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

	
	while(1)
	{
		GetSCKEY();
		if(ResetCard_A())
		{
			nCode = CpuCard_Apdu(0x0f, 7, cSendBuf, &cRcvLen, cRcvBuf);
			Beep(3);
			Deselect_A();
			Reset_Reader_TimeOut();
		}
		//500ms或1S刷新一次 从Flash读取点阵需要时间可放定时器回调函数处理
	//	DispTime();
		sw_disp_menu();
		get_test_comm_data(comm_data);
		//ret0 = COM_SendBuf(COM0,  comm_data,  8);
		ret1 = COM_SendBuf(COM1, comm_data, 14);

		//DelayNS(100);
		//sprintf(cDispMsg, "r0=%d,r1=%d,%d,%s", ret0, ret1, cnt++, writebuf);
		if(cnt >= 2000)
			cnt = 0;
		//喂狗定义防止溢出程序复位		
		KillWatchDog();
	}	
}
