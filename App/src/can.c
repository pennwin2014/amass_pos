#include "can.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <can.h>




int tcp_send_data(char* send_buf, uint32 send_len, char* recv_buf, uint32* recv_len)//发送函数
{
	int cfd;
	int recbytes,writebytes;
	struct sockaddr_in s_add;
	unsigned short portnum=8888; 

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == cfd)
	{
	    LOG((LOG_DEBUG,"socket fail ! \r\n"));
	    return -1;
	}

	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;
	s_add.sin_addr.s_addr= inet_addr("192.168.1.83");
	s_add.sin_port=htons(portnum);

	//设置超时时间
	struct timeval tv;
	tv.tv_sec = 5;  //5秒超时
	tv.tv_usec = 0;  
	setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	//alarm(2);
	if(connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr))<0)
	{
	    //LOG((LOG_DEBUG,"connect fail !\r\n"));
		//alarm(0);
	    return -1;
	}
	//alarm(0);
	//LOG((LOG_DEBUG,"connect ok !"));
	
    if((writebytes = write(cfd,send_buf,send_len))<0)
    {
		LOG((LOG_DEBUG,"write fail !\r\n"));
	}
	//LOG((LOG_DEBUG,"write ok !"));
/*
	if((recbytes = read(cfd,recv_buf,1024)) < 0)
	{
		LOG((LOG_DEBUG,"read data fail !recbytes=%d\r\n",recbytes));
		close(cfd);
	    return -1;
	}
*/
	char tmp_recv_buf[2048] = {0};
	// recv 6 byte
	if((recbytes = read(cfd,tmp_recv_buf,6)) < 0)
	{
		LOG((LOG_DEBUG,"read data fail !recbytes=%d\r\n",recbytes));
		close(cfd);
	    return -1;
	}
	memcpy(recv_buf, tmp_recv_buf, 6);//贴上前6个字节
	if(memcmp(tmp_recv_buf,"SLV",3) != 0)
	{
		LOG((LOG_DEBUG,"read SLV fail !\r\n"));
		close(cfd);
	    return -1;
	}
	int16 read_len = 0;
	memcpy(&read_len, tmp_recv_buf+4, 2);
	//LOG((LOG_DEBUG,"read_len = %d",read_len));
	int16 total_read_len = 0;
	int16 left_read_len = read_len;
	memset(tmp_recv_buf, 0, 2048);
	while(1)
	{
		recbytes = read(cfd, tmp_recv_buf+total_read_len, left_read_len);
		if(recbytes<0)
		{
			LOG((LOG_DEBUG,"read data fail !recbytes=%d\r\n",recbytes));
			close(cfd);
	    	return -1;
		}
		total_read_len += recbytes;
		left_read_len -= recbytes;
		//LOG((LOG_DEBUG,"已接收 %d",total_read_len));
		if(left_read_len <= 0)
		{
			//LOG((LOG_DEBUG,"接收完毕!!!"));
			break;
		}
		if(recbytes == 0)
		{
			LOG((LOG_DEBUG,"recbytes == 0"));
			//说明接收到了结束符
			break;
		}
	}
	memcpy(recv_buf+6, tmp_recv_buf, total_read_len);//贴上剩余数据
	(*recv_len) = 6+total_read_len;
	//LOG((LOG_DEBUG,"接收到数据打印:"));
	//p16dump_hex(LOG_DEBUG,recv_buf,(*recv_len));

	close(cfd);
	return 0;
}



