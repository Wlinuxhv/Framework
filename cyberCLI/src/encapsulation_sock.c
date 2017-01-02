/*----------------------------------------------------------------------------
 * Name:    encapsulation_sock.c
 * Purpose: Init TCP socket of client
 * Version: v1.0
 * Note(s): 
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/

#include <encapsulation_sock.h>



/*----------------------------------------------------------------------------
  On client using input port and address create a TCP sender socket use to connect. 
 *----------------------------------------------------------------------------*/
int init_tcp_client(int port)
{
	int fd;
	int on = 1;
	struct sockaddr_in peer_addr;


	fd = Socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addr.sin_family = AF_INET;
	peer_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	peer_addr.sin_port = htons(port);
	Connect(fd, &peer_addr, sizeof(peer_addr));

	return fd;
}

