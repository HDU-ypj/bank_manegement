/**************************************************
>	作	  者：杭电羊皮卷
>   文件名称: client.c
>	联系方式：weixin:QQ2997675141
>   创建日期：2022年09月01日
>   描    述：
**************************************************/

#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"

static int msgid; 
static int interface_level=0;

void interface_main(void)
{
	printf("***********main interface***********\n");
	printf("		0、退出\n");
	printf("		1、登录\n");
	printf("		2、开户\n");
	printf("		3、注销\n");
	printf("		4、解锁\n");
	printf("please input your choise:");
}
void interface_second(void)
{
	printf("***********secondary interface***********\n");
	printf("		0、退出\n");
	printf("		1、存钱\n");
	printf("		2、取钱\n");
	printf("		3、转账\n");
	printf("		4、查询\n");
	printf("		5、更改密码\n");
	printf("please input your choise:");
}
void work(enum Msg_type type)
{	
	int flag=0;
	static Ctos_mes person_mes;
	person_mes.pid = getpid();
	
	switch(type)
	{
		case t_create:create_func(msgid);break;
		case t_destory:destory_func(msgid);break;
		case t_login:flag = login_func(msgid,&person_mes);break;
		case t_unlock:unlock_func(msgid);break;
		
		case t_change_sec:change_sec_func(msgid,&person_mes);break;
		case t_query:query_func(msgid,&person_mes);break;
		case t_save:save_func(msgid,&person_mes);break;
		case t_transfer:transfer_func(msgid,&person_mes);break;
		case t_withdraw:withdraw_func(msgid,&person_mes);break;
	}
	
	if(flag)
	{
		interface_level=1;
	}
}
int main(int argc,const char* argv[])
{
	//获取消息队列
	msgid = get_msgid();
	while(1)
	{
		//界面
		if(interface_level==0)
		{
			interface_main();
		}
		else
		{
			interface_second();
		}
		
		int choise = get_choise();
		choise = choise+interface_level*5;
		switch(choise)
		{
			case 1:work(t_login);break;
			case 2:work(t_create);break;
			case 3:work(t_destory);break;
			case 4:work(t_unlock);break;
			case 6:work(t_save);break;
			case 7:work(t_withdraw);break;
			case 8:work(t_transfer);break;
			case 9:work(t_query);break;
			case 10:work(t_change_sec);break;
			default:break;
		}
		if(choise == 5)	interface_level=0;
		if(choise == 0)	break;
		press_anykey_continue();
	}
	
	
	return 0;
}
