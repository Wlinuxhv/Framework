#ifndef __RECIEV_H__
#define __RECIEV_H__

#include "socket_encap.h"

/* 从监听的描述符集合不断的接收数据*/
extern void recv_new_packets(fd_set rfds);  
/*新的连接请求*/
extern void recv_new_connect(int sockfd);
/*设置接收数据描述符到读集合*/
extern void recv_setfds(fd_set *fds, int *nfds);
extern int fd_getnext(int n);
/*回收不再使用的资源*/
extern void release_unusable(int fd);


#endif

