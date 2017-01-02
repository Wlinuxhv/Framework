
#ifndef __OVERALL_H__
#define __OVERALL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include <encapsulation_print.h>


#define PORT 10000

#define  USRSIZE     1024
#define JSONSIZE    4096
#define SENDS  8

/*需要服务器快速响应的各种请求*/
#define TABLE     "t"                               //入座
#define MENU      "m"                             //读菜谱
#define ORDER    "o"                             //订单
#define REMIN     "r"                              //催单
#define COUNT    "c"                              //结账

#define CLEAR_BUF do{while(getchar() != '\n');}while(0)

#define SYS_ERR(err)  do{\
	perror(err);\
	exit(-1);\
}while(0)

#define HELP \
		printf("\n\tWelcome use smart order-solft\n\n\n");\
		printf("1.book order\n2.reminder\n3.checkout\n\nPls choice:\n")

static inline void *Malloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	if(ret == NULL){
		perror("malloc");
		exit(-1);
	}

	return ret;
}

extern int local_fd;





#endif
