#ifndef __ALLINCLUDE_H__
#define __ALLINCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "debug.h"
#include "klist.h"
#include "socket_encap.h"
#include <req_dispose.h>


//#include <encapsulate_dbase.h>

extern struct cli_info cli_head;

#define OK                                    1
#define FAIL                                  -1
#define LGIN 0
#define LGOUT  1
#define CONNECT  0
#define UNCONNECT  1

#define PORT                                10000
#define MAXNUMCLT                          100     //ͬʱ�����������ͻ������������ܴ�1024���ļ�������
#define NAMESIZ                           30       //���ֳ���
#define IDSIZ                           30       //���ֳ���
#define BUFLEN                             2048
#define BACKSIZE                          4096//ԭ����512
#define TYPESIZE                          20
#define  SQL_STRING_LEN             1024

/*�ͻ�����Ϣ*/
typedef struct cli_info{
	char utype[TYPESIZE];/*�ƻ���APP*/
	int uid; /*Ψһʶ����*/
	char uname[NAMESIZ];
	int	usr_fd;/*�������Ӻ���Accept���ص��ļ�������*/
	/*���Ը�����Ҫ��Ӹ������Ϣ*/
	struct list_head cli_list;
}cli_info_t;


/*�ͻ��˴����ӵ���Ϣ*/
typedef struct chat_link{
	int from;
	int to;
	struct list_head chat_list;
}chat_link_t;



#define SYS_ERR(err)  do{\
	perror(err);\
	exit(-1);\
}while(0)

#define SAFE_FREE(ptr) \
	do {	\
		if(ptr) { \
			free(ptr);	\
			ptr = NULL;	\
		}	\
	} while(0)

static inline void *Malloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	if(ret == NULL){
		SYS_ERR("malloc");
	}

	return ret;
}

static inline void *init_zero(void *data, int len)
{
	return memset(data, 0, len);
}

static inline int get_uname_by_fd(char *name, int fd)
{
	struct list_head *pos, *n;
	cli_info_t *tmp_usr;
	list_for_each_safe(pos, n, &cli_head.cli_list){
		tmp_usr = list_entry(pos,cli_info_t, cli_list);
		printf("name: %s \nuid: %d \ntype: %s\nusr_fd: %d\n",tmp_usr->uname,tmp_usr->uid,tmp_usr->utype,tmp_usr->usr_fd);
		if(tmp_usr->usr_fd==fd)
		{
		 	strncpy(name, tmp_usr->uname, NAMESIZ); 
			return OK;
		}
	}
	return FAIL;
}
extern void *func_reciev(void *args);
//extern void *background(void *args);
//extern void *func_process(void *args);






extern int fds[MAXNUMCLT];
extern struct cli_info AllCli[MAXNUMCLT];

extern pthread_mutex_t usr_mutex;
extern chat_link_t chat_link_head;
extern pthread_mutex_t chat_link_mutex;




#endif






