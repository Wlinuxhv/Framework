#include "json_handle.h"

#if 0 
int json_get_int(struct json_object *obj, struct json_object *tmp_obj, const char *name)
{
	tmp_obj = json_object_object_get(obj, name);

	return json_object_get_int(tmp_obj);
}

const char *json_get_string(struct json_object *obj, struct json_object *tmp_obj, const char *name)
{
	tmp_obj = json_object_object_get(obj, name);

	return json_object_get_string(tmp_obj);
}
#endif

int data_json_to_userinfo(char *json_str, int fd,struct  cli_info *userinfo)
{
	
	struct json_object *usr_obj;
	struct json_object  *tmp_obj;

	/*将json字符串解析成json 对象*/
	usr_obj = json_tokener_parse(json_str);

	/*获取用户名,类型,ID*/
	JSON_GET_STRING(usr_obj, tmp_obj, "uname", &(userinfo->uname));
	JSON_GET_STRING(usr_obj, tmp_obj, "utype", &(userinfo->utype));
	JSON_GET_INT(usr_obj, tmp_obj, "uid", &(userinfo->uid));
	/*存储链接对于的fd*/
	userinfo->usr_fd = fd;

	/*释放json对象资源*/
	json_object_put(usr_obj);
	json_object_put(tmp_obj);
	
	return 1;
	
}


int data_json_chat_create(char *json_str,chat_link_t *ch_link)
{
	struct json_object *usr_obj;
	struct json_object  *tmp_obj;
	
	usr_obj = json_tokener_parse(json_str);/*将json字符串解析成json 对象*/
	/*获取要通信的from to*/
	JSON_GET_INT(usr_obj, tmp_obj, "from", &(ch_link->from));
	JSON_GET_INT(usr_obj, tmp_obj, "to", &(ch_link->to));
	/*释放json对象资源*/
	json_object_put(usr_obj);
	json_object_put(tmp_obj);
	
	return 1;
}

int data_json_chat_get(char *json_str,chat_link_t *ch_link,char *msgbuf)
{
	struct json_object *usr_obj;
	struct json_object	*tmp_obj;

	usr_obj = json_tokener_parse(json_str);/*将json字符串解析成json 对象*/
	/*获取要通信的from to*/
	printf("json_str: %s\n",json_str);

	JSON_GET_INT(usr_obj, tmp_obj, "from", &(ch_link->from));
	JSON_GET_INT(usr_obj, tmp_obj, "to", &(ch_link->to));
	JSON_GET_STRING(usr_obj, tmp_obj,"msg",&msgbuf);

	/*释放json对象资源*/
	json_object_put(usr_obj);
	json_object_put(tmp_obj);

	return 1;
}




#if 0
void data_json_array_create(char *json_buf, const char *arr_name, char **db_result, int nrow, int ncolumn)
{
	int i, j;
	int index;
	struct json_object *my_array, *my_object, *new_obj;
	
	new_obj = json_object_new_object();
	my_array = json_object_new_array(); /*创建个空json对象值数组类型*/

	index = ncolumn;
	for(i=0; i<nrow; i++){
		my_object = json_object_new_object();
			
		for(j=0; j<ncolumn; j++){
			json_object_object_add(my_object, db_result[j], \
				json_object_new_string(db_result[index]));
			++index;
		}
			
		json_object_array_add(my_array, my_object);
	}
	json_object_object_add(new_obj, arr_name, my_array);
	strcpy(json_buf, json_object_to_json_string(new_obj));

	json_object_put(my_object);
	json_object_put(new_obj);
	json_object_put(my_array);

}

void data_json_menu_create(const char *name_tbl, char *json_buf)
{
	char **db_result;
	int nrow, ncolumn;
	char sql_buf[SQL_STRING_LEN];
	 
	sprintf(sql_buf, "select * from  %s", name_tbl);
	db_get_table(sql_buf, &db_result, &nrow, &ncolumn);

	data_json_array_create(json_buf, "carte", db_result, nrow, ncolumn);
	Printf("json_buf:%s\n", json_buf);

	return;
}

void data_json_get_orderid(const char *json_str, order_t *order, int index)
{
	struct json_object *ord_obj;
	struct json_object *tmp_obj;
	struct json_object *arr_obj;
	
	/*将json字符串解析成json 对象*/
	ord_obj = json_tokener_parse(json_str);

	JSON_GET_ARRAY(ord_obj, &arr_obj, "carte", index);   //获取第i个成员
	JSON_GET_INT(arr_obj, tmp_obj, "id", &(order->dishid));
	JSON_GET_INT(arr_obj, tmp_obj, "number", &(order->dishnum));

	json_object_put(ord_obj);
	json_object_put(tmp_obj);
	json_object_put(arr_obj);

}

void data_json_order_analysis(pend_t  pend,int *type, int *total_num, int *deskid)
{
	struct json_object *ord_obj;
	struct json_object *tmp_obj;
	
	/*将json字符串解析成json 对象*/
	ord_obj = json_tokener_parse(pend.json_buf);

	/*获取订单类型*/
	JSON_GET_INT(ord_obj, tmp_obj, "orderid", type);
	/*获取桌号*/
	JSON_GET_INT(ord_obj, tmp_obj, "deskid", deskid);
	/*获取订单数量长度*/
	JSON_GET_INT(ord_obj, tmp_obj, "total number", total_num); 

	json_object_put(ord_obj);
	json_object_put(tmp_obj);
}

void data_json_obj_create(char *json_buf, const char *arr_name, char **db_result)
{
	struct json_object *obj;
	
	/*将json字符串解析成json 对象*/
	obj = json_tokener_parse(json_buf);

	json_object_object_add(obj, arr_name, \
				json_object_new_string(db_result[1]));

	init_zero(json_buf, JSONSIZE);
	strcpy(json_buf, json_object_to_json_string(obj));

	json_object_put(obj);

}

/*data_json_array_create()
 * data_json_obj_create()
 *以上两个函数封装有待改进
 *其中data_json_obj_create()函数里需要对json_buf字符进行判断
 *(是否只需要新建json对象?
 *还是把json_buf里json对象和新建的json对象合并?)
 */
void data_json_checkout_create(const char *name_tbl, char *json_buf)
{
	char **db_result = NULL;
	int nrow, ncolumn;
	char sql_buf[SQL_STRING_LEN];
	 
	sprintf(sql_buf, \
		"select dishid, dishname, dishprice, dishnum, all_price from  %s", name_tbl);
	db_get_table(sql_buf, &db_result, &nrow, &ncolumn);

	if(db_result[ncolumn] != NULL){
		/*创建json格式的账单对象数组成员*/
		data_json_array_create(json_buf, "carte", db_result, nrow, ncolumn);
		Printf("json_buf:%s\n", json_buf);
	}

	sprintf(sql_buf, \
		"select sum(all_price) from  %s", name_tbl);
	db_get_table(sql_buf, &db_result, &nrow, &ncolumn);

	if(db_result[ncolumn] != NULL){
		/*创建json格式的账单对象值成员*/
		data_json_obj_create(json_buf, "total", db_result);
		Printf("json_buf:%s\n", json_buf);
	}
}
#endif
