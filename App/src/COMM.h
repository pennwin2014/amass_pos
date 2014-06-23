/******************************************************************************************************
** Created by:			TanMuLiang
** Created date:		2007-01-26
** Version:				1.0
** Descriptions:		The original version
*******************************************************************************************************/

#ifndef __COMM_H__
#define __COMM_H__

//485
#define    RS485_TERE     	1<<10

//////////// UART   /////////////////////
#define     COM0   	0	//����0
#define     COM1   	1	//����1
#define 	COMB	2	//��������
typedef  void (*pREVFUNC)(void *pBuff,uint16 nlen);//���ڻص�����
typedef struct tagDCB
{
	uint32 		baudrate;				// ������
	char 		datab;		    			// �ֳ��ȣ�5/6/7/8��ѡ
	char 		stopb;		    			// ֹͣλ��1/2��ѡ
	char 		parity;		    			// ��żУ��λ��0-��У��,1-��У��,2-żУ��
	char 		ruf;						// ����
	uint16 		limitlen;					// ���ȷ�ֵ
	uint32 		timeout;					// ���ճ�ʱʱ��
	pREVFUNC 	pRecvfuc;				// ���ջص�����
}DCB,*pDCB;

/*
*********************************************************************************************************
** �������� ��StartComm()
** �������� �����ڳ�ʼ��,���ô���0��1�����ʡ�λ����ֹͣλ����żУ��λ
** ��ڲ��� ��pComData :���ô���0��1��lBaudBate :���ʣ�cBits :����λ����cStop :ֹͣλ����cParty :��У��λ����
** ���ڲ��� ����
** ��    �� ����
*********************************************************************************************************
*/
void StartComm(uint8 COM,pDCB pComData);

/*
*********************************************************************************************************
** �������� ��COM_SendBuf()
** �������� ���򴮿ڷ���n�ֽ�����
** ��ڲ��� ��dat	Ҫ���͵�����
** ���ڲ��� ����
** ��    �� : 0 : ��ʾ���ɹ��� 1 : ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8 COM_SendBuf(uint8 uComm,uint8 *pcSendBuf, int iLen);

#endif