
#ifndef __SEND_H__
#define __SEND_H__

#include <socket_encap.h>

#define NO_JSON 0

extern int send_msg(char *buf, int len, char type, int fd);




#endif
