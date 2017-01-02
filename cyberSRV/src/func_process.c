/*----------------------------------------------------------------------------
 * Name:    func_process.c
 * Purpose: Receive customer send data and put it in the queue
 * Version: v1.0
 * Note(s): 
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/
#include "allinclude.h"
#include "recv.h"

//extern int register_process(char *data, int fd);



int fds[MAXNUMCLT];

int fd_getnext(int n)
{
	return fds[n];
}

/*------------------------------------------------------------------------------
//函数名称 :   void release_unusable(int fd)
//功能描述 :   释放文件描述符，释放数据资源
//输入参数 :   
// 输出参数 :   
//创  建  者:   WangLin
* 创建时间  :   2016.08.24  10:11:06
------------------------------------------------------------------------------*/
void release_unusable(int fd)
{
	int i;
	struct list_head *pos,*pos_chat, *n,*n_chat;
	cli_info_t *tmp_usr;
	chat_link_t *tmp_chat;
	for(i=0; i<MAXNUMCLT; i++){
		if(fd == fds[i]){
			fds[i] = 0;
			break;
		}
	}

	list_for_each_safe(pos, n, &cli_head.cli_list){
		tmp_usr = list_entry(pos,cli_info_t, cli_list);
		list_del_init(pos);
		if(tmp_usr->usr_fd==fd)
		{
			/*释放与其在一起聊天的资源*/
			list_for_each_safe(pos_chat, n_chat, &chat_link_head.chat_list){
				tmp_chat = list_entry(pos_chat,chat_link_t, chat_list);
				if((tmp_chat->from==tmp_usr->uid)||(tmp_chat->to==tmp_usr->uid))
					SAFE_FREE(tmp_chat);
			}
			/*释放自己的在线数据*/	
			SAFE_FREE(tmp_usr);
		}
	}
	close(fd);
}

/*----------------------------------------------------------------------------
  set all clinet's fd into fdset
 *----------------------------------------------------------------------------*/
void recv_setfds(fd_set *fds, int *nfds)
{
	int i = 0;
	int fd = 0;
	
	while(i < MAXNUMCLT){
		if(fd_getnext(i))
			fd = fd_getnext(i);
		i++;

		FD_SET(fd, fds);
		if(*nfds <= fd){
			*nfds = fd + 1;
		}
	}

	return;
}

/*----------------------------------------------------------------------------
  accept a new connect
 *----------------------------------------------------------------------------*/
void recv_new_connect(int sockfd)
{
	int i;
	int newfd;
	newfd = Accept(sockfd, NULL, NULL);
	Printf("A new connect:%d!\n", newfd);

	for(i=0; i<MAXNUMCLT; i++){
		if(fds[i] == 0){
			fds[i] = newfd;
			break;
		}
	}

	return;
}

/*----------------------------------------------------------------------------
接收数据包并解析出理
 *----------------------------------------------------------------------------*/
void recv_new_packets(fd_set rfds)
{
	int ret = -1;
	int i = 0;
	int fd = 0;
	char json_buf[JSONSIZE];
	char uname[NAMESIZ];

	while(0 != (fd = fd_getnext(i))){
		i++;

		if(FD_ISSET(fd, &rfds)){
			init_zero(json_buf,sizeof(json_buf));
			
			if(Recv(fd, json_buf, JSONSIZE, 0) == 0){	
				get_uname_by_fd(uname, fd);
				printf("client<%s> logout!\n", uname);
				printf("client  logout!\n");

				release_unusable(fd);
			}else{
				switch(json_buf[0]){
				case 'r':
					register_process(json_buf, fd);
					init_zero(json_buf,sizeof(json_buf));
					break;
//				case 'c':/*测试不用*/
//					create_chat_process(json_buf,fd);
//					break;
				case 'l':/*聊天*/
					chating_process(json_buf,fd);
					break;
				}
			}
		}
	}

	return;
}


/*----------------------------------------------------------------------------
  A thread entry,
  receive local port  from main thread to create a socket[void *args],
  then wait a connect or receive pakcets. 
 *----------------------------------------------------------------------------*/
void *func_reciev(void *args)
{
	int fd = *(int *)args;
	int nfds;
	fd_set readfds;

	while(1){
		nfds = fd + 1;
	
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		recv_setfds(&readfds, &nfds);
		
		Select(nfds, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(fd, &readfds)){//socket有连接了需要去建立连接
			recv_new_connect(fd);
			continue;
		}

		recv_new_packets(readfds);
	}

	return;

}




