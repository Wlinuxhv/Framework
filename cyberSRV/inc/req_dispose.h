#ifndef __REQ_DISPOSE_H__
#define __REQ_DISPOSE_H__

#define JSONSIZE                          4096


#include "allinclude.h"
#include <socket_encap.h>
//#include "db_encap.h"
#include <json_handle.h>

#define copy_data(dst, fd, data) do{\
	memcpy(dst->json_buf, data, JSONSIZE);\
	dst->fd = fd;\
}while(0)

typedef struct req_data{
	int fd;
	char json_buf[JSONSIZE];
	void *private;
}req_data;

#if 1

extern int register_process(char *data, int fd);
extern void *cliservice_init(void *args); 
extern int create_chat_process(char *data,int fd);
extern void *chating_req_thd(void *args);

extern void *chating_thd(void *args);


#else


/*处理入座请求*/
extern int desk_process(char *data, int fd);
/*处理催单请求*/
extern int remind_process(int fd);
/*处理读菜谱请求*/
extern int menu_process(int fd);
/*处理结账请求*/
extern int count_process(int fd);
/*处理订单请求*/
extern int order_process(char *data, int fd);
extern int get_orderid(int *orderid, int fd);



/*(pending)待处理订单插入链表*/

extern void insert_pending(pend_t *pend);
extern void usrinfo_fill(req_data *trs_d, usr_t *userinfo);//填充用户结构体
extern int deskinfo_fill(desk_t *deski, usr_t *userinfo);//填充餐桌结构体

extern void *cliservice_init(void *args);//入座处理线程
extern void *build_menu(void *args);//菜谱处理线程
extern void *build_bill(void *args);//结账处理线程
extern void set_time(char *timer);   //获取系统时间


/*处理pending待处理json格式订单链表*/
extern void *func_process(void * args);
extern int get_pend_node(pend_t  *pend);//获取pending结点
extern int new_order_process(const char *json_str, int index, int deskid);/*新订单处理*/
extern void show_usr_order(int deskid);//显示订单到厨房端
/*根据订单类型判断怎么处理订单*/
extern int order_type_process(const char *json_str, int type, int num, int deskid);



/*厨师线程处理订单*/
extern int cook_order(int deskid);

#endif

#endif

