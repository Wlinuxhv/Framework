#ifndef __SOCK_ENCAP_H__
#define __SOCK_ENCAP_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "debug.h"
 #include "allinclude.h"

#define DEFAULT_BACKLOG 5

extern int init_tcp_server(unsigned short port, int *fd);
extern int init_tcp_client(in_addr_t addr, in_port_t port);

static inline int Socket(int domain, int type, int protocol)
{
	int ret;

	ret = socket(domain, type, protocol);
	if(ret < 0){
		perror("socket");
		exit(-1);
	}

	return ret;
}

static inline void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen)
{
	int ret;

	ret = bind(sockfd, (struct sockaddr *)addr, addrlen);
	if(ret != 0){
		perror("bind");
		exit(-1);
	}

	return;
}

static inline void Listen(int sockfd, int backlog)
{
	int ret;

	ret = listen(sockfd, backlog);
	if(ret != 0){
		perror("listen");
		exit(-1);
	}

	return;
}

static inline int Accept(int sockfd, struct sockaddr_in *addr, socklen_t *addrlen)
{
	int ret;

	ret = accept(sockfd, (struct sockaddr *)addr, addrlen);
	if(ret < 0 ){
		perror("accept");
		exit(-1);
	}

	return ret;
}

static inline int Connect(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen)
{
	int ret;

	ret = connect(sockfd, (struct sockaddr *)addr, addrlen);
	if(ret < 0 ){
		perror("connect");
		exit(-1);
	}

	return ret;
}

static inline ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
{
	ssize_t size;

	size = recv(sockfd, buf, len, flags);
	if(size < 0 ){
		perror("recv");
		exit(-1);
	}

	return size;
}

static inline void Send(int sockfd, const void *buf, size_t len, int flags)
{
	ssize_t size;

	size = send(sockfd, buf, len, flags);
	if(size != len){
		perror("send");
		exit(-1);
	}
	Printf("len=%d    size=%d\n", len, size);

	return;
}

static inline void Select(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout)
{
	int ret;

	ret = select(nfds, readfds, writefds, exceptfds, timeout);
	if(ret < 0 ){
		perror("select");
		exit(-1);
	}

	return;
}

#endif


