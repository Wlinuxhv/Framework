
#ifndef __SEND_H__
#define __SEND_H__

#include <overall.h>
#include <message.h>
#include <encapsulation_sock.h>

#define URGENT_DATA  'U'     //紧急数据(入座、读菜谱、催单、结账)
#define NORMAL_DATA  'N'     //客户信息、订单数据


extern int get_sockfd(void);

extern int send_msg(char *buf, int len, char type);




#endif
