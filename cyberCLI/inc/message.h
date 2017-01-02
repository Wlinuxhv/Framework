#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <dlist.h>

#define NAMESIZ     20
#define BUFLEN       2048

/*--------------------------------------------------------------------------- 
   两个节点间交互的4种消息类型 
 ----------------------------------------------------------------------------*/
enum msg_type{
	regist_msg,
	menu_msg,
	order_msg,
	checkout_msg,
	prompt_msg
};

/*--------------------------------------------------------------------------- 
  两个节点发送消息的通用结构
 ----------------------------------------------------------------------------*/
struct msg_t{
	char type; /* 使用msg_type 表示buf中的消息类型 */
	int tableid; /* 桌号 */
	char name[NAMESIZ];  
	int fd;
	char buf[BUFLEN]; /* 存放具体类型的结构 */
	struct list_head msg_list;
};

struct msg_t msg_head;

extern struct msg_t usr_msg;


#endif
