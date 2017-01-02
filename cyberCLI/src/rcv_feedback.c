/*----------------------------------------------------------------------------
 * Name:    rcv_feedback.c
 * Purpose: Receive and the feedback information from server
 * Version: v1.0
 * Note(s): 
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/


#include <rcv_feedback.h>


void *rcv_feedback(void *args)
{
	int fd;
	char buf[JSONSIZE];

	fd = get_sockfd();

	while(1){
		if(Recv(fd, buf, JSONSIZE, 0) == 0){
			printf("\n\n\t\t***************\n\t\t*Server unusual!*\n\t\t***************\n\n\n");
			pthread_exit(NULL);
		}
		else{
			printf("\n%s\n", buf);
		}
	}


}
