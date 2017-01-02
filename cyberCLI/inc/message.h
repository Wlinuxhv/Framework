#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <dlist.h>

#define NAMESIZ     20
#define BUFLEN       2048

/*--------------------------------------------------------------------------- 
   �����ڵ�佻����4����Ϣ���� 
 ----------------------------------------------------------------------------*/
enum msg_type{
	regist_msg,
	menu_msg,
	order_msg,
	checkout_msg,
	prompt_msg
};

/*--------------------------------------------------------------------------- 
  �����ڵ㷢����Ϣ��ͨ�ýṹ
 ----------------------------------------------------------------------------*/
struct msg_t{
	char type; /* ʹ��msg_type ��ʾbuf�е���Ϣ���� */
	int tableid; /* ���� */
	char name[NAMESIZ];  
	int fd;
	char buf[BUFLEN]; /* ��ž������͵Ľṹ */
	struct list_head msg_list;
};

struct msg_t msg_head;

extern struct msg_t usr_msg;


#endif
