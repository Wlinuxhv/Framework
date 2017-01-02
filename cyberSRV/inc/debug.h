#ifndef __ENCAP_PRINT_H__
#define __ENCAP_PRINT_H__

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>


#define __DEBUG__ 0
	#if __DEBUG__

	#define Printf(fmt,...)  \
	do{\
		printf(fmt,##__VA_ARGS__);\
		printf("\n");\
	}while(0)   

	//printf("%s %d %s::",__FILE__, __LINE__, __func__); \


	#define SLEEP(nsec) sleep(nsec)

	#else

	#define Printf(fmt,...)
	#define SLEEP(nsec)

	#endif

#endif

