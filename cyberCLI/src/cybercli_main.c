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
  *������ÿ���������䱸����նˣ��ն˻��ı�ź�����ƥ�䣬�ͻ����Ը����ն˻�������ʾ���е�ͣ�Ҳ����֧�ֿͻ�ͨ�������ֻ���װ���������������С�
  *�˿ͻ���ģ����ǹ̶��ڲ����ϵ��ն˻��������ն˻������ź��û����Ѿ��̶�
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

	//���÷�������
	//fix me ...

	//�����߳�
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


