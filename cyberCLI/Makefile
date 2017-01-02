
vpath %.c = ./src
vpath %.h = ./inc
CC = gcc
CFLAGS = -g -I./inc  -L./lib
#OPTION = -lsqlite3 -lpthread -ldl
OPTION = -lpthread -ldl

#cybercli:cybercli_main.c encapsulation_sock.c  send.c  send_request.c rcv_feedback.c
cybercli:cybercli_main.c encapsulation_sock.c rcv_feedback.c send.c 
	$(CC) $(CFLAGS) -o $@ $^ $(OPTION)

.PHONY: clean
clean:
	$(RM) -f *.o cybercli *.sw? *.bak
