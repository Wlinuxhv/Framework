
#ifndef __SEND_REQUEST_H__
#define __SEND_REQUEST_H__

//#include <message.h>
#include <send.h>

#define ENCAP_REQ(dst, src, flags)  \
		sprintf(dst, "%c%s", flags[0], src);

extern int get_orderid(void *buf);
extern void *send_request(void *args);







#endif
