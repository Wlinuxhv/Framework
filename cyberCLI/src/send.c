/*----------------------------------------------------------------------------
 * Name:    send.c
 * Purpose: To send data function
 * Version: v1.0
 * Note(s): 
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/


#include <send.h>

int get_sockfd(void)
{
	return local_fd;
}

int send_msg(char *buf, int len, char type)
{
	int fd;

	if(len < 0){
		return -1;
	}

	fd = get_sockfd();
	if(fd <= 0){
		return -1;
	}

	if((NULL != buf) && (0 != len)){
		
		if(URGENT_DATA == type){
			Send(fd, buf, len, MSG_OOB);
			Printf("u send ok!");
			return 1;
		}
		else if(NORMAL_DATA == type){
			Send(fd, buf, len, 0);
			Printf("n send ok!");
			return 1;
		}
		else{
			printf("Unable to identify your message to be sent!\n");
			return -1;
		}
	}

	return -1;

}



