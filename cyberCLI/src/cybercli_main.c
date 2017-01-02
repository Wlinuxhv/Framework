/*----------------------------------------------------------------------------
 * Name:    waiter_main.c
 * Purpose: waiter program main functions
 * Version: v1.0
 * Note(s):
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/

#include <send_request.h>
#include <message.h>
#include <encapsulation_sock.h>
#include <rcv_feedback.h>

#define  USRID     321
#define USRNAME "Cyber_HW"
#define  DESKID    8
#define  UTYPE "Cyber"
#define LEN 1024
int local_fd = -1;
static pthread_t snd_th, rcv_th;

/*
  *餐厅的每个餐桌都配备点菜终端，终端机的编号和桌号匹配，客户可以根据终端机界面提示进行点餐；也可以支持客户通过智能手机安装本餐厅点餐软件进行。
  *此客户端模拟的是固定在餐桌上的终端机，所以终端机的桌号和用户名已经固定
  */
static inline int get_usr_info(char *usrinfo)
{	int ret=-1;
	ret = sprintf(usrinfo, "r{\"uid\":%d,\"uname\":\"%s\",\"utype\":\"%s\"}", USRID, USRNAME, UTYPE);
	printf("%s",usrinfo);
}

int main(void)
{
	char usr_buf[USRSIZE];
	char	send_buf[LEN];
	char	recv_buf[LEN];
	local_fd = init_tcp_client(PORT);
	int num = -1;
	get_usr_info(usr_buf);

	//设置分离属性
	//fix me ...

	//创建线程
//	pthread_create(&snd_th, NULL, send_request, usr_buf);
	pthread_create(&rcv_th, NULL, rcv_feedback, NULL);
	write(local_fd, usr_buf, strlen(usr_buf));
	while(1)
	{
		fgets(send_buf, LEN, stdin);
		num = write(local_fd, send_buf, strlen(send_buf));
		if(num < 0)
		{
			perror("write");
			exit(1);
		}
	}

	return 0;
}


