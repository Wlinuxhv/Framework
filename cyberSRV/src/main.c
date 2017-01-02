#include <allinclude.h>


#include <unistd.h>


/***************全局数据START**********************/
static pthread_t th_rcv, th_proc, th_upd;
struct cli_info AllCli[MAXNUMCLT];
cli_info_t cli_head;
chat_link_t chat_link_head;
pthread_mutex_t usr_mutex;
pthread_mutex_t chat_link_mutex;


/***************全局数据END*********************/



/**********************************************
  *On server! Create a TCP socket use to accept connect. 
  *********************************************/
int init_tcp_server(unsigned short port, int *fd)
{
	int on = 1;
	struct sockaddr_in local_addr;


	*fd = Socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(*fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);
	Bind(*fd, &local_addr, sizeof(local_addr));

	Listen(*fd, DEFAULT_BACKLOG);

	return *fd;
}



/*----------------------------------------------------------------------------
 * 初始化用于管理用户的相关数据链表
 *--------------------------------------------------------------------------*/
void init_list_head(void)
{
//	int i;

	INIT_LIST_HEAD(&cli_head.cli_list);
	INIT_LIST_HEAD(&chat_link_head.chat_list);

	
//	INIT_LIST_HEAD(&pnd_head.p_list);
//	for(i=0; i<DESKNUM; i++){
//		INIT_LIST_HEAD(&(desk[i].order.o_list));
//	}
}

/*----------------------------------------------------------------------------
 * 初始化用于管理用户的相关数据互斥量
 *--------------------------------------------------------------------------*/

void init_resous(void)
{
	pthread_mutex_init(&usr_mutex, 0);    //管理用户结点锁
	pthread_mutex_init(&chat_link_mutex, 0);  //管理消息结点锁
	return;
}

void read_alllist_info(void)
{
	struct list_head *pos, *n;
	cli_info_t *tmp_usr;
	chat_link_t*tmp_chat;
	int onlinecnt = 0;

	list_for_each_safe(pos, n, &cli_head.cli_list){
		tmp_usr = list_entry(pos,cli_info_t, cli_list);
		printf("name: %s \nuid: %d \ntype: %s\nusr_fd: %d\n",tmp_usr->uname,tmp_usr->uid,tmp_usr->utype,tmp_usr->usr_fd);
		printf("***********************\n");
		onlinecnt++;
	}
	if(onlinecnt==0)
		printf("\n******no one online*******\n");
//	list_for_each_safe(pos, n, &chat_link_head.chat_list){
//		tmp_chat = list_entry(pos,chat_link_t, chat_list);
//		
//		printf("from: %d \n to: %d \n",tmp_chat->from,tmp_chat->to);
//	}	
		
}

int main(void)
{
	int fd;
	int cnt= 0;
	unsigned short local_port = PORT;
	char buf[BUFLEN];
	pthread_attr_t attr;
	char hello[12]="hello test";
	init_zero(fds, sizeof(fds));            //初始化通信fd集合
	init_list_head();
	init_tcp_server(local_port, &fd);	 //初始化TCP网络套接字

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&th_rcv, &attr, func_reciev, &fd);
//	pthread_create(&th_proc, &attr, func_process,  &fd);
	printf("\n\n\t\t********************\n\t\t*CyberServer start!*\n\t\t********************\n\n\n");


	while(1){
		sleep(10);
		printf("The ---%d--online userinfo is:\n",cnt++);		
		read_alllist_info();
	}
}
