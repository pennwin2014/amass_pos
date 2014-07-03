#include "flash_store.h"

/**************************************************************
******SF_Write
//最后一个参数
//1-----先擦除原来的数据再写，肯定可以写上
//0-----直接写，有可能有的地方写不上
//返回值:1表示成功
****************************************************************/
uint32 sp_get_next_transdtl_addr(uint32 transdtl_addr)
{
/*
	//假如要从最后一页跨到第一页了，也将第一页删除
	if(master_unit.trans_addr + sizeof(sp_st_transdtl) > ADDR_TRANS_LAST)
	{
		//擦除第一页
		SF_ErasePage(0);
		ret = SF_Write(0, 0, ptransdtl, sizeof(sp_st_transdtl), 0);
	}
	//假如要跨页的话就将后面那一页删除
	next_page = 2;
	SF_ErasePage(next_page);
	//是否要考虑正要被擦除的页的流水未上传的问题??????
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
*** 函数名:		sp_write_transno_unit
*** 函数功能:	写主(从)流水号	
*** 参数flag: 	 true----> 主流水号;       false-----> 从流水号
*** 参数punit: 待存入的结构体的指针
*** 作者:		汪鹏
*** 时间:		2014-07-03
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
	pageno = startaddr/FLASH_PAGE_SIZE; 	//得到页号
	lastaddr = sp_get_transno_lastaddr(pageno);//获取到的是相对地址
	
	if(lastaddr+sizeof(sp_transno_unit) > FLASH_PAGE_SIZE)//查看是否会超过最大地址
	{
		SF_ErasePage(pageno);		//擦除该页
		lastaddr = 0;		//从0开始写
	}
	//从lastaddr开始往下写
	ret = SF_Write(pageno, lastaddr, punit, sizeof(sp_transno_unit), 0);//直接写
	if(SP_FLASH_FAIL == ret)
	{
		ret = SF_Write(pageno, lastaddr, punit, sizeof(sp_transno_unit), 1);//擦除后再写	
	}
	return ret;
	
}


/*******************************************************
*** 函数名:		sp_read_transno_unit
*** 函数功能:	读取主从流水号	
*** 参数flag: 	 true 主流水号;false 从流水号
*** 参数punit: 	最小单元的结构体指针
*** 作者:		汪鹏
*** 时间:		2014-07-03
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
*** 函数名:		sp_write_transdtl
*** 函数功能:		写交易流水到flash
*** 参数: 	交易流水结构体指针
*** 作者:		汪鹏
*** 时间:		2014-07-03
*********************************************************/
uint8 sp_write_transdtl(uint8* ptransdtl)
{
	//1、读取主从流水号
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
	//2、对比主从流水号是否一致
	if(master_unit.trans_no != slave_unit.trans_no)
	{
		//将从流水号存储区的内容覆盖到主流水号中
		pageno = ADDR_SLAVE_TRANS_SEQNO/FLASH_PAGE_SIZE;
		SF_Read(pageno, 0, read_buf, FLASH_PAGE_SIZE);
		pageno = ADDR_MASTER_TRANS_SEQNO/FLASH_PAGE_SIZE;
		ret = SF_Write(pageno, 0, read_buf, FLASH_PAGE_SIZE, 1);//擦出后写
		//不成功就死循环写
		write_cnt = MAX_FLASH_WRITE_CNT;
		while(ret != SP_FLASH_SUCCESS)
		{
			ret = SF_Write(pageno, 0, read_buf, FLASH_PAGE_SIZE, 1);//擦出后写
			if(write_cnt--<1)
			{
				return SP_FLASH_FAIL;//返回错误
			}
		}
		
	}
	//3、根据从流水号里的地址读取流水数据
	pageno = slave_unit.trans_addr/FLASH_PAGE_SIZE;
	offset_addr = slave_unit.trans_addr - pageno*FLASH_PAGE_SIZE;
	memset(&tmp_trandtl, 0, sizeof(tmp_trandtl));
	SF_Read(pageno, offset_addr, (uint8*)(&tmp_trandtl), sizeof(tmp_trandtl));
	//4、对比流水里的流水号和从流水号存储区中的流水号是否一致
	if(tmp_trandtl.termseqno != slave_unit.trans_no)

	{
		return SP_FLASH_FAIL;
	}
	//5、写新的主流水号
	memcpy(&tmp_trandtl, ptransdtl, sizeof(sp_st_transdtl));
	////5.1、获取下一个流水地址以及流水号
	next_addr = sp_get_next_transdtl_addr(slave_unit.trans_addr);
	////5.2、将该地址以及流水中的流水号写入主流水号存储区
	
	memset((&master_unit), 0, sizeof(master_unit));
	master_unit.trans_no = tmp_trandtl.termseqno;
	master_unit.trans_addr = next_addr;
	//6、写新的主流水号
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
	//7、写新的主流水号
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
		//8、 写流水前55个字节(除去最后的tac，交易标志，保留，crc)
		pageno = master_unit.trans_addr/FLASH_PAGE_SIZE;
		offset_addr = master_unit.trans_addr - FLASH_PAGE_SIZE*pageno;
		SF_Write(pageno, offset_addr, ptransdtl, 55, 1);//使用擦出后写的方式
		//9、读取一遍流水内容是否和内存中的一致
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
	//11、写卡
	sp_do_write_card(ptransdtl);
	//12更新最后9个字节
	sp_update_left_transdtl_info(ptransdtl);
	
	return 0;
}

//读取流水
uint8 sp_read_transdtl(uint8* ptransdtl)
{
	uint8 ret = 0;
	uint16 pageno = 0, offset_addr = 0;
	sp_transno_unit master_unit, slave_uint;
	//1、读取流水号
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
	//2、根据流水号里的地址读取流水
	pageno = slave_uint.trans_addr / FLASH_PAGE_SIZE;
	offset_addr = slave_uint.trans_addr - pageno*FLASH_PAGE_SIZE;
	SF_Read(pageno, offset_addr, ptransdtl, sizeof(sp_st_transdtl));
	return 0;
}

//写系统信息
uint8 sp_write_sysinfo(uint8* p_ctx)
{
	uint16 pageno;
	pageno = ADDR_SYSINFO/FLASH_PAGE_SIZE;
	SF_ErasePage(pageno);
	SF_Write(pageno, 0, p_ctx, sizeof(sp_context), 0);
	return 0;
}

//读取系统信息
uint8 sp_read_sysinfo(uint8* p_ctx)
{
	uint16 pageno;
	pageno = ADDR_SYSINFO/FLASH_PAGE_SIZE;
	SF_ErasePage(pageno);
	SF_Read(pageno, 0, p_ctx, sizeof(sp_context));
	return 0;
}









