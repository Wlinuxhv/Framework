#include "req_dispose.h"

#if 0
int get_pend_node(pend_t  *pend)
{
	pend_t *tmp_pnd;

	sem_wait(&pnd_non_empty);
	pthread_mutex_lock(&pnd_mutex);
	Printf("get node");

	if(list_empty(&pnd_head.p_list)){
		sem_post(&pnd_non_full); 
		pthread_mutex_unlock(&pnd_mutex);
		
		return FAIL;
	}
	else{
		tmp_pnd = list_entry(pnd_head.p_list.prev, pend_t, p_list);
		memcpy(pend, tmp_pnd, sizeof(pend_t));
		
		list_del_init(&(tmp_pnd->p_list));
		free(tmp_pnd);
		
		sem_post(&pnd_non_full); 
		pthread_mutex_unlock(&pnd_mutex);
		return OK;
	}


}

void set_time(char *timer)
{
	/*获取系统时间*/
}

int new_order_process(const char *json_str, int index, int deskid)
{
	int ret = -1;
	char buf[BACKSIZE];
	order_t *order;

	order = (order_t *)Malloc(sizeof(order_t));
	data_json_get_orderid(json_str, order, index);//获取订单中的菜品编号和数量
	ret = db_order_create(order);   //从库存表从得到原材料充足的菜品
	set_time(order->booktime);
	order->flags = 'n';
	order->all_price = (order->dishprice)*(order->dishnum);
	order->uc_id = deskid;//客户桌号作为用户账单号

	if(ret == OK){
		pthread_mutex_lock(&(desk[deskid].o_mutx));
		list_add(&(order->o_list), &(desk[deskid].order.o_list));//将客户所点菜品插入对应的桌号结构体，待厨师处理
		desk[deskid].usr->uo_state = 'd';   //标志客户已经下单
		pthread_mutex_unlock(&(desk[deskid].o_mutx));
		return OK;
	}
	else{
		sprintf(buf, "{\"result\":\"0\", \"dishid\":\"%d\"}", order->dishid);
		send_msg(buf, BACKSIZE, 0, desk[deskid].use_fd);
		SAFE_FREE(order);
		return FAIL;
	}

}

void show_usr_order(int deskid)
{
	struct list_head *pos, *n;
	struct order_t *tmp_ord;

	if(!list_empty(&(desk[deskid].order.o_list))){
		printf("There are new orders of customer deskid is %d!\n"
			"Please input deskid process order\n", deskid);
		printf("\ndishname\t\tdishnum\n");
		list_for_each_safe(pos, n, &(desk[deskid].order.o_list)){
			tmp_ord = list_entry(pos, order_t, o_list);
			printf("%s\t\t%d\n", tmp_ord->dishname, tmp_ord->dishnum);
		}
	}
}

 /*
  * 0: 表示新订单
  * 1：增加订单
  * 2：删除订单
  * 3：查询订单
  * 其他值： 订单编号
  */
int order_type_process(const char *json_str, int type, int num, int deskid)
{
	int ret = -1;
	int index;

	switch(type){
		case 0:
			for(index=0; index<num; index++)
				ret = new_order_process(json_str, index, deskid);
			show_usr_order(deskid);
			break;
		case 1:
			//ret = add_order_process();
			break;
		case 2:
			//ret = del_order_process();
			break;
		case 3:
			//ret = query_order_process();
			break;
		default:
			printf("Unable to identify your order type!\n");
			break;
	}

	return ret;
}
#endif


/*------------------------------------------------------------------------------
//函数名称 :   int register_process(char *data, int fd)
//功能描述 :   记录用户存储用户对于的文件描述符
//输入参数 :   
// 输出参数 :   
//创  建  者:   WangLin
* 创建时间  :   2016.08.24  14:41:42
------------------------------------------------------------------------------*/
int register_process(char *data, int fd)
{
	pthread_t tid;
	char buf[BACKSIZE];
	req_data *trs_data;
	pthread_attr_t attr;

	trs_data = (req_data *)Malloc(sizeof(req_data));
	init_zero(trs_data, sizeof(req_data));
	copy_data(trs_data, fd, data);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*创建分离线程，初始化餐桌结构体*/
	if(pthread_create(&tid, &attr, cliservice_init, trs_data) < 0){
		sprintf(buf, "服务繁忙请稍后再试!\n");
		send_msg(buf, BACKSIZE, 0, fd);
		free(trs_data);
		return FAIL;
	}
	else{
		usleep(5000);
		Printf("create a pthread process desk init!\n");
		return OK;
	}

}

int create_chat_process(char *data,int fd)
{
	pthread_t tid;
	char buf[BACKSIZE];
	req_data *trs_data;
	pthread_attr_t attr;

	trs_data = (req_data *)Malloc(sizeof(req_data));
	init_zero(trs_data, sizeof(req_data));
	copy_data(trs_data, fd, data);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if(pthread_create(&tid, &attr, chating_req_thd, trs_data) < 0){
		sprintf(buf, "Read the menu data failed!\n");
		send_msg(buf, BACKSIZE, 0, fd);
		return FAIL;
	}
	else{
		usleep(5000);
		return OK;
	}
}



int chating_process(char *data,int fd)
{
	pthread_t tid;
	char buf[BACKSIZE];
	req_data *trs_data;
	pthread_attr_t attr;

	trs_data = (req_data *)Malloc(sizeof(req_data));
	init_zero(trs_data, sizeof(req_data));
	copy_data(trs_data, fd, data);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if(pthread_create(&tid, &attr, chating_thd, trs_data) < 0){
		sprintf(buf, "Read the menu data failed!\n");
		send_msg(buf, BACKSIZE, 0, fd);
		return FAIL;
	}
	else{
		usleep(5000);
		return OK;
	}
    
}


/* ----------------------------------------------------------------------- */

/**************************
*判断是否在线
***************************/
int IS_login(cli_info_t *clinode)
{
	struct list_head *pos, *n;
	struct cli_info *tmp_usr;
	if(NULL == clinode)
		return LGOUT;
	
	list_for_each_safe(pos, n,&cli_head.cli_list)
	{
		tmp_usr = list_entry(pos, cli_info_t, cli_list);
		
		printf("clinode->uid = %d\n",clinode->uid);
		printf("tmp_usr->uid = %d\n",tmp_usr->uid);		
		if(tmp_usr->uid==clinode->uid){
			return LGIN;
		}
	}
	return LGOUT;
}


int IS_connect(chat_link_t *pchact_info)
{
	struct list_head *pos, *n;
	chat_link_t *tmp_chat;
	int cdn;
	if(NULL == pchact_info)
		return CONNECT;
	printf("hello---^_^--in\n");
	/*从用户链表里找到该用户的桌号，将此桌号作为该客户的订单号*/
	list_for_each_safe(pos, n,&chat_link_head.chat_list){
		tmp_chat = list_entry(pos,chat_link_t, chat_list);
		//there is a big bug fix me
		printf("pchact_info->from = %d!!\n",pchact_info->from);
		printf("tmp_chat->from = %d!!\n",tmp_chat->from);
		printf("pchact_info->to = %d!!\n",pchact_info->to);
		printf("tmp_chat->to = %d!!\n",tmp_chat->to);
		if((pchact_info->from==tmp_chat->from)||
			(pchact_info->to==tmp_chat->to)||
			(pchact_info->to==tmp_chat->from)||
			(pchact_info->from==tmp_chat->to))
		{
		    return CONNECT;
		}
	}

	return UNCONNECT;    
}



int usrinfo_fill(req_data *trs_d, cli_info_t *cli_info)
{
	int ret = -1;
	data_json_to_userinfo(trs_d->json_buf+1, trs_d->fd, cli_info);	
	ret = IS_login(cli_info);
	return ret;
}

int  chat_fill(req_data *trs_d, chat_link_t *pchact_info)
{
	printf("pchact_info:%s", trs_d->json_buf);

	int ret = IS_connect(pchact_info);

	if(ret==UNCONNECT)
	{
		data_json_chat_create(trs_d->json_buf+1,pchact_info);
		return UNCONNECT;
	}else{
		return CONNECT;
	}
}

/*------------------------------------------------------------------------------
将连接客户的节点数据插入内核链表
------------------------------------------------------------------------------*/
void usrinfo_2list(cli_info_t *userinfo)
{
	pthread_mutex_lock(&usr_mutex);
	list_add(&userinfo->cli_list,&cli_head.cli_list);
	pthread_mutex_unlock(&usr_mutex);
}

/*------------------------------------------------------------------------------
将连接聊天节点数据插入内核链表
------------------------------------------------------------------------------*/
void chat_info_2list(chat_link_t *chatinfo)
{
	pthread_mutex_lock(&chat_link_mutex);
	list_add(&chatinfo->chat_list,&chat_link_head.chat_list);
	pthread_mutex_unlock(&chat_link_mutex);
}

/*-------------------------------------------------------------------------
  *由uid获得文件描述符
  *-----------------------------------------------------------------------*/

int uid_get_fd(chat_link_t *pchact_info,int cur_fd)
{
	struct list_head *pos, *n;
	cli_info_t *tmp_cli;
	if(NULL == pchact_info)
		return CONNECT;
	/*从用户链表里找到该用户的桌号，将此桌号作为该客户的订单号*/
	list_for_each_safe(pos, n,&cli_head.cli_list){
		tmp_cli = list_entry(pos,cli_info_t,cli_list);
		//there is a big bug fix me
		printf("uid_get_fd from = %d!!\n",pchact_info->from);
		printf("uid_get_fd to = %d!!\n",pchact_info->to);
		if((pchact_info->to==tmp_cli->uid)&&(tmp_cli->usr_fd!=cur_fd))
		{
		    return tmp_cli->usr_fd;
		}
	}
	return -1;
}


/*-------------------------------------------------------------------------
  *线程快速响应客户端请求
  *-----------------------------------------------------------------------*/

void *cliservice_init(void *args)
{
	req_data *trs_data = (req_data *)args;
	cli_info_t *pcli_info;
	char buf[BACKSIZE];
	int ret = -1;

	pcli_info = (struct cli_info *)Malloc(sizeof(struct cli_info));
	init_zero(pcli_info, sizeof(struct cli_info));
	init_zero(buf, sizeof(buf));

	/*将JSON数据存入节点*/
	ret = usrinfo_fill(trs_data, pcli_info);

	if(ret==LGOUT)
	{
		usrinfo_2list(pcli_info);
		printf("the usrinfo is storaged \nclient:%d  connected with server!\ncur_fd:%d\n",pcli_info->uid,trs_data->fd);
		sprintf(buf, "connect the server\n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
		
	}else{
		printf("the userinfo had been  storaged this client:%d already login!\n",pcli_info->uid);
		sprintf(buf, "you already login !\n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
		SAFE_FREE(pcli_info);
	}
	
	SAFE_FREE(trs_data);
	
	pthread_exit(NULL);
}

/*****************************
服务两个人聊天的线程
架构不太合理,每次发送
消息都开线程然后死掉
*****************************/
void *chating_req_thd(void *args)
{

	req_data *trs_data = (req_data *)args;
	chat_link_t *pchact_info;
	char buf[BACKSIZE];
	int ret = -1;
	
	pchact_info = (chat_link_t *)Malloc(sizeof(chat_link_t));
	init_zero(pchact_info, sizeof(chat_link_t));
	init_zero(buf, sizeof(buf));
	/*将JSON数据存入节点*/
	ret = chat_fill(trs_data, pchact_info);
	if(ret){
		chat_info_2list(pchact_info);
		printf("chat_link  storaged\n");
		sprintf(buf, "connect you \n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
	}else{
		SAFE_FREE(pchact_info);
		printf("chat_link already storaged\n");
		sprintf(buf, "you had been connect\n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
	}
	
	SAFE_FREE(trs_data);

	pthread_exit(NULL);
}


	
/*****************************
服务两个人聊天的线程
架构不太合理,每次发送
消息都开线程然后死掉
*****************************/
void *chating_thd(void *args)
{

	req_data *trs_data = (req_data *)args;
	chat_link_t *pchact_info;
	char buf[BACKSIZE];
	int ret = -1;
	int tofd = -1;
	
	pchact_info = (chat_link_t *)Malloc(sizeof(chat_link_t));
	init_zero(pchact_info, sizeof(chat_link_t));
	init_zero(buf, sizeof(buf));
	/*将JSON数据存入节点*/
	 data_json_chat_get(trs_data->json_buf+1, pchact_info,buf);
	/*检查是否在线*/
	tofd = uid_get_fd(pchact_info,trs_data->fd);
	/* 在线就将数据发过去 */
	printf("will send # #%s# # to %d \nthe tofd is %d\n",buf,pchact_info->to,tofd);
	if(tofd > 0){
//		chat_info_2list(pchact_info);
//		printf("shit what happened here\n");
		/*服务器直接转发数据不做过多的解析*/
		send_msg(trs_data->json_buf+1, BACKSIZE, 0, tofd);
	}else{
		printf("chat_link already storaged\n");
//		sprintf(buf," 对方不在线\n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
		SAFE_FREE(pchact_info);
	}
	
	SAFE_FREE(trs_data);

	pthread_exit(NULL);
}



#if 0

void *build_menu(void *args)//菜谱处理线程
{
	int fd = *(int *)args;
	char buf[JSONSIZE] = {"init!\n"};
	char uname[NAMESIZ];

	data_json_menu_create(INVENTORY_TABLE, buf);
	get_uname_by_fd(uname, fd, desk);
Printf("menu fd:%d", fd);
	printf("Barack %s to send the latest recipes to customers!\n", uname);
	send_msg(buf, JSONSIZE, 0, fd);
}

void *build_bill(void *args)//结账处理线程
{
	int fd = *(int *)args;
	char json_buf[JSONSIZE];
	char uname[NAMESIZ];

	//init_zero(json_buf, JSONSIZE);
	//data_json_checkout_create(COUNT_TABLE, json_buf);
	init_zero(json_buf, JSONSIZE);
	sprintf(json_buf, "{\"result\":\"1\"}");
	send_msg(json_buf, JSONSIZE, 0, fd);

	get_uname_by_fd(uname, fd, desk);
	printf("Invoicing customer Mr %s!\n", uname);
}

/*----------------------------------------------------------------------
 从pending 队列中取出json格式订单数据处理
----------------------------------------------------------------------*/
void *func_process(void * args)
{
	char buf[JSONSIZE];
	int ret = -1;
	int type = -1;
	pend_t  pend;
	int total_num;
	int deskid;

	while(1){
		ret = get_pend_node(&pend);
		Printf("pend.deskid = %d pend.fd = %d pend.json_buf = %s ", pend.uc_id, pend.fd, pend.json_buf);
		if(ret == OK){
			data_json_order_analysis(pend, &type, &total_num, &deskid);
			
			if(order_type_process(pend.json_buf, type, total_num, deskid) == OK){
				sprintf(buf, "{\"result\":\"%d\", \"orderid\":\"%d\"}", OK, pend.uc_id);
				Printf("order process ok!\n");
				send_msg(buf, JSONSIZE, 0, pend.fd);
			}
		}
		else{
			printf("get pending node failed!\n");
			continue;
		}
	}
}

int cook_order(int deskid)
{
	struct list_head *pos, *n;
	order_t *tmp_ord;

	if(list_empty(&(desk[deskid].order.o_list))){
		printf("Ramadhin for %d customers didn't order!\n", deskid);
		return FAIL;
	}
	
	pthread_mutex_lock(&(desk[deskid].o_mutx));
	list_for_each_safe(pos, n, &(desk[deskid].order.o_list)){
		list_del_init(pos);
		tmp_ord = list_entry(pos, order_t, o_list);
		set_time(tmp_ord->donetime);
		db_order_insert_counttbl(tmp_ord, 0);
		SAFE_FREE(tmp_ord);
	}
	pthread_mutex_unlock(&(desk[deskid].o_mutx));

	desk[deskid].usr->uo_state = 'o';  //该客户的订单烹饪完成
	printf("Ramadhin for %d customer orders to finish!\n", deskid);

	return OK;
}

#endif

