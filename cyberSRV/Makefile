
vpath %.c = ./src
vpath %.h = ./inc
CC = gcc
CFLAGS = -g -I./inc  -L./lib
OPTION = -lsqlite3 -lpthread -ldl -ljson
#OPTION = -lpthread -ldl 

cyberServer:main.c func_process.c send.c req_dispose.c json_handle.c 
	$(CC) $(CFLAGS) -o $@ $^ $(OPTION)

.PHONY: clean
clean:
	$(RM) -f *.o *.sw? *.bak cyberServer  ksystem.db
