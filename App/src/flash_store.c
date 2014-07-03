#include "flash_store.h"

/**************************************************************
******SF_Write
//���һ������
//1-----�Ȳ���ԭ����������д���϶�����д��
//0-----ֱ��д���п����еĵط�д����
//����ֵ:1��ʾ�ɹ�
****************************************************************/
uint32 sp_get_next_transdtl_addr(uint32 transdtl_addr)
{
/*
	//����Ҫ�����һҳ�絽��һҳ�ˣ�Ҳ����һҳɾ��
	if(master_unit.trans_addr + sizeof(sp_st_transdtl) > ADDR_TRANS_LAST)
	{
		//������һҳ
		SF_ErasePage(0);
		ret = SF_Write(0, 0, ptransdtl, sizeof(sp_st_transdtl), 0);
	}
	//����Ҫ��ҳ�Ļ��ͽ�������һҳɾ��
	next_page = 2;
	SF_ErasePage(next_page);
	//�Ƿ�Ҫ������Ҫ��������ҳ����ˮδ�ϴ�������??????
*/
	return 0;
}


uint8 sp_do_write_card(uint8* ptransdtl)
{
	return 0;
}

uint8 sp_update_left_transdtl_info(uint8* ptransdtl)
{
	return 0;
}


/*******************************************************
*** ������:		sp_write_transno_unit
*** ��������:	д��(��)��ˮ��	
*** ����flag: 	 true----> ����ˮ��;       false-----> ����ˮ��
*** ����punit: ������Ľṹ���ָ��
*** ����:		����
*** ʱ��:		2014-07-03
*********************************************************/
uint8 sp_write_transno_unit(bool flag, uint8* punit)
{
	uint8 ret = 0;
	uint16 pageno = 0;
	uint16 lastaddr = 0,startaddr = 0; 
	if(flag == true)
	{
		startaddr = ADDR_MASTER_TRANS_SEQNO;
	}
	else
	{
		startaddr = ADDR_SLAVE_TRANS_SEQNO;
	}
	pageno = startaddr/FLASH_PAGE_SIZE; 	//�õ�ҳ��
	lastaddr = sp_get_transno_lastaddr(pageno);//��ȡ��������Ե�ַ
	
	if(lastaddr+sizeof(sp_transno_unit) > FLASH_PAGE_SIZE)//�鿴�Ƿ�ᳬ������ַ
	{
		SF_ErasePage(pageno);		//������ҳ
		lastaddr = 0;		//��0��ʼд
	}
	//��lastaddr��ʼ����д
	ret = SF_Write(pageno, lastaddr, punit, sizeof(sp_transno_unit), 0);//ֱ��д
	if(SP_FLASH_FAIL == ret)
	{
		ret = SF_Write(pageno, lastaddr, punit, sizeof(sp_transno_unit), 1);//��������д	
	}
	return ret;
	
}


/*******************************************************
*** ������:		sp_read_transno_unit
*** ��������:	��ȡ������ˮ��	
*** ����flag: 	 true ����ˮ��;false ����ˮ��
*** ����punit: 	��С��Ԫ�Ľṹ��ָ��
*** ����:		����
*** ʱ��:		2014-07-03
*********************************************************/
uint8 sp_read_transno_unit(bool flag, uint8* ptrans_unit)
{
	uint16 pageno = 0;
	uint8 ret = 0;
	uint8 read_buf[FLASH_PAGE_SIZE] = {0};
	uint32 offset = 0;
	sp_transno_unit tmp_unt;
	uint8 tmp_crc[2] = {0};
	uint16 unit_len  = 0;
	unit_len = sizeof(sp_transno_unit);
	if(flag == true)
	{
		pageno = ADDR_MASTER_TRANS_SEQNO/FLASH_PAGE_SIZE;
	}
	else
	{
		pageno = ADDR_SLAVE_TRANS_SEQNO/FLASH_PAGE_SIZE;
	}
	ret = SF_Read(pageno, 0, read_buf, FLASH_PAGE_SIZE);
	if (ret != SP_FLASH_FAIL)
		return SP_FLASH_FAIL;
	for(offset = 0; offset < FLASH_PAGE_SIZE; offset += unit_len)
	{
		memcpy(&tmp_unt, read_buf+offset, unit_len);
		pb_protocol_crc((uint8*)&tmp_unt, unit_len-2, tmp_crc);
		if(memcmp(tmp_crc, tmp_unt.crc, 2) == 0)
		{
			memcpy(ptrans_unit, &tmp_unt, unit_len);
			return SP_FLASH_SUCCESS;
		}
	}
	return SP_FLASH_NOT_FOUNT;
	
}



/*******************************************************
*** ������:		sp_write_transdtl
*** ��������:		д������ˮ��flash
*** ����: 	������ˮ�ṹ��ָ��
*** ����:		����
*** ʱ��:		2014-07-03
*********************************************************/
uint8 sp_write_transdtl(uint8* ptransdtl)
{
	//1����ȡ������ˮ��
	uint8 ret = 0;
	uint16 pageno, write_cnt = 0, offset_addr = 0;
	uint32 next_addr = 0;
	sp_transno_unit master_unit,slave_unit;
	sp_st_transdtl tmp_trandtl, read_transdtl;
	uint8 read_buf[FLASH_PAGE_SIZE];
	ret = sp_read_transno_unit(true, (uint8*)&master_unit);
	if(ret)
		return ret;
	ret = sp_read_transno_unit(false, (uint8*)&slave_unit);
	if(ret)
		return ret;
	//2���Ա�������ˮ���Ƿ�һ��
	if(master_unit.trans_no != slave_unit.trans_no)
	{
		//������ˮ�Ŵ洢�������ݸ��ǵ�����ˮ����
		pageno = ADDR_SLAVE_TRANS_SEQNO/FLASH_PAGE_SIZE;
		SF_Read(pageno, 0, read_buf, FLASH_PAGE_SIZE);
		pageno = ADDR_MASTER_TRANS_SEQNO/FLASH_PAGE_SIZE;
		ret = SF_Write(pageno, 0, read_buf, FLASH_PAGE_SIZE, 1);//������д
		//���ɹ�����ѭ��д
		write_cnt = MAX_FLASH_WRITE_CNT;
		while(ret != SP_FLASH_SUCCESS)
		{
			ret = SF_Write(pageno, 0, read_buf, FLASH_PAGE_SIZE, 1);//������д
			if(write_cnt--<1)
			{
				return SP_FLASH_FAIL;//���ش���
			}
		}
		
	}
	//3�����ݴ���ˮ����ĵ�ַ��ȡ��ˮ����
	pageno = slave_unit.trans_addr/FLASH_PAGE_SIZE;
	offset_addr = slave_unit.trans_addr - pageno*FLASH_PAGE_SIZE;
	memset(&tmp_trandtl, 0, sizeof(tmp_trandtl));
	SF_Read(pageno, offset_addr, (uint8*)(&tmp_trandtl), sizeof(tmp_trandtl));
	//4���Ա���ˮ�����ˮ�źʹ���ˮ�Ŵ洢���е���ˮ���Ƿ�һ��
	if(tmp_trandtl.termseqno != slave_unit.trans_no)

	{
		return SP_FLASH_FAIL;
	}
	//5��д�µ�����ˮ��
	memcpy(&tmp_trandtl, ptransdtl, sizeof(sp_st_transdtl));
	////5.1����ȡ��һ����ˮ��ַ�Լ���ˮ��
	next_addr = sp_get_next_transdtl_addr(slave_unit.trans_addr);
	////5.2�����õ�ַ�Լ���ˮ�е���ˮ��д������ˮ�Ŵ洢��
	
	memset((&master_unit), 0, sizeof(master_unit));
	master_unit.trans_no = tmp_trandtl.termseqno;
	master_unit.trans_addr = next_addr;
	//6��д�µ�����ˮ��
	ret = sp_write_transno_unit(true, (uint8*)(&master_unit));
	write_cnt = MAX_FLASH_WRITE_CNT;
	while(ret != SP_FLASH_SUCCESS)
	{
		ret = sp_write_transno_unit(true, (uint8*)(&master_unit));
		if(write_cnt--<1)
		{
			return SP_FLASH_FAIL;
		}
	}
	//7��д�µ�����ˮ��
	ret = sp_write_transno_unit(false, (uint8*)(&master_unit));
	write_cnt = MAX_FLASH_WRITE_CNT;
	while(ret != SP_FLASH_SUCCESS)
	{
		ret = sp_write_transno_unit(false, (uint8*)(&master_unit));
		if(write_cnt--<1)
		{
			return SP_FLASH_FAIL;
		}
	}
	write_cnt = MAX_FLASH_WRITE_CNT;
	while(1)
	{
		//8�� д��ˮǰ55���ֽ�(��ȥ����tac�����ױ�־��������crc)
		pageno = master_unit.trans_addr/FLASH_PAGE_SIZE;
		offset_addr = master_unit.trans_addr - FLASH_PAGE_SIZE*pageno;
		SF_Write(pageno, offset_addr, ptransdtl, 55, 1);//ʹ�ò�����д�ķ�ʽ
		//9����ȡһ����ˮ�����Ƿ���ڴ��е�һ��
		SF_Read(pageno, offset_addr, (uint8*)(&read_transdtl), sizeof(sp_st_transdtl));
		if(read_transdtl.termseqno == tmp_trandtl.termseqno)
		{
			break;
		}
		if(write_cnt--<1)
		{
			return SP_FLASH_FAIL;
		}
	}
	//11��д��
	sp_do_write_card(ptransdtl);
	//12�������9���ֽ�
	sp_update_left_transdtl_info(ptransdtl);
	
	return 0;
}

//��ȡ��ˮ
uint8 sp_read_transdtl(uint8* ptransdtl)
{
	uint8 ret = 0;
	uint16 pageno = 0, offset_addr = 0;
	sp_transno_unit master_unit, slave_uint;
	//1����ȡ��ˮ��
	ret = sp_read_transno_unit(true, (uint8*)(&master_unit));
	if(ret != SP_FLASH_SUCCESS)
		return ret;
	ret = sp_read_transno_unit(false, (uint8*)(&slave_uint));
	if(ret != SP_FLASH_SUCCESS)
		return ret;
	if(slave_uint.trans_no != master_unit.trans_no)
	{
		return SP_FLASH_ERROR;
	}
	//2��������ˮ����ĵ�ַ��ȡ��ˮ
	pageno = slave_uint.trans_addr / FLASH_PAGE_SIZE;
	offset_addr = slave_uint.trans_addr - pageno*FLASH_PAGE_SIZE;
	SF_Read(pageno, offset_addr, ptransdtl, sizeof(sp_st_transdtl));
	return 0;
}

//дϵͳ��Ϣ
uint8 sp_write_sysinfo(uint8* p_ctx)
{
	uint16 pageno;
	pageno = ADDR_SYSINFO/FLASH_PAGE_SIZE;
	SF_ErasePage(pageno);
	SF_Write(pageno, 0, p_ctx, sizeof(sp_context), 0);
	return 0;
}

//��ȡϵͳ��Ϣ
uint8 sp_read_sysinfo(uint8* p_ctx)
{
	uint16 pageno;
	pageno = ADDR_SYSINFO/FLASH_PAGE_SIZE;
	SF_ErasePage(pageno);
	SF_Read(pageno, 0, p_ctx, sizeof(sp_context));
	return 0;
}









