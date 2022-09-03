#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"

//身份证号是否合法
static int is_illegal(char* id_num)
{
	return 1;
}

static Ctos_mes mes;
static Stoc_mes rec;

//开户
void create_func(int msgid)
{
	char buf[20];
	
	//身份证号
	while(1)
	{
		printf("请输入您的身份证号：");
		fgets_t(buf,20);
		if(is_illegal(buf))
		{
			break;
		}
		printf("身份证号非法\n");
	}
	strcpy(mes.data.id_num,buf);
	
	//银行卡号（自动生成）
	mes.data.id_card = get_idcard();
	write_idcard(mes.data.id_card+1);
	
	//初始密码
	strcpy(mes.data.key,"123");
	
	//初始账户余额
	printf("请输入您的初始账户余额：");
	while(1 != scanf("%lf",&mes.data.money))
	{
		printf("格式错误，请重新输入！\n");
		BUFF_CLEAR;
	}
	
	//是否锁定
	mes.data.is_lock = 3;
	
	//消息队列类型号
	mes.type = t_create;
	
	//进程号
	mes.pid = getpid();
	
	//发送消息
	msgsnd(msgid,&mes,CTOS_LEN,0);
	//等待接收
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	printf("%s\n",rec.tips);
}

//销户
void destory_func(int msgid)
{
	char strbuf[20];
	Stoc_mes receive;
	Ctos_mes mes;
	
	//消息队列类型号
	mes.type = t_destory;
	
	//进程号
	mes.pid = getpid();
	
	//银行卡帐号
	printf("请输入需要销户的银行卡帐号：\n");
	while(1 != scanf("%lld",&mes.data.id_card))
	{
		BUFF_CLEAR;
		printf("输入有误，请重新输入!\n");
	}
	
	//输入密码确认销户
	printf("请输入需要销户的帐号密码：\n");
	fgets_t(strbuf,20);
	strcpy(mes.data.key,strbuf);
	
	//发送消息，让他干活
	msgsnd(msgid,&mes,CTOS_LEN,0);
	//等待接收
	msgrcv(msgid,&receive,STOC_LEN,getpid(),0);
	
	printf("%s\n",receive.tips);
}

//客户端解锁
void unlock_func(int msgid)
{
	char buf[20];
	
	//银行卡号
	printf("请输入您的银行卡号：");
	while(1 != scanf("%lld",&mes.data.id_card))
	{
		BUFF_CLEAR;
		printf("格式错误请重新输入：");
	}
	
	//身份证号
	printf("请输入您的身份证号：");
	fgets_t(mes.data.id_num,20);
	
	//消息队列类型号
	mes.type = t_unlock;
	
	//进程号
	mes.pid = getpid();
	
	//发送消息
	msgsnd(msgid,&mes,CTOS_LEN,0);
	//等待接收
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	
	printf("%s\n",rec.tips);
}

//客户端登录
int login_func(int msgid,Ctos_mes* person_mes)
{
	char buf[20];
	
	//银行卡号解锁
	printf("请输入您的银行卡号：");
	while(1 != scanf("%lld",&mes.data.id_card))
	{
		BUFF_CLEAR;
		printf("格式错误请重新输入：");
	}
	
	while(1)
	{
		//输入密码进行登录
		printf("请输入您的登录密码(q退出)：");
		fgets_t(buf,20);
		
		if(strcmp("q",buf)==0)
		{
			break;
		}
		strcpy(mes.data.key,buf);
	
		//消息队列类型号
		mes.type = t_login;
	
		//进程号
		mes.pid = getpid();
	
		//发送消息
		msgsnd(msgid,&mes,CTOS_LEN,0);
		//等待接收
		msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
		printf("%s\n",rec.tips);
		if(NULL != strstr(rec.tips,"succeed"))
		{
			(person_mes->data).id_card = mes.data.id_card;
			return 1;
		}
	}
	return 0;
}

void change_sec_func(int msgid,Ctos_mes* idcard)
{
	char strbuf[20];
	
	idcard->type = t_change_sec;
	
	printf("请输入原密码：\n");
	fgets_t(idcard->data.key,20);
	msgsnd(msgid,idcard,CTOS_LEN,0);
	
	printf("请输入新密码：\n");
	fgets_t(idcard->data.key,20);
	msgsnd(msgid,idcard,CTOS_LEN,0);
	
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	printf("%s\n",rec.tips);
}

void query_func(int msgid,Ctos_mes* idcard)
{
	idcard->type = t_query;
	
	msgsnd(msgid,idcard,CTOS_LEN,0);
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	printf("%s\n",rec.tips);
}
void save_func(int msgid,Ctos_mes* idcard)
{
	double money=0;
	printf("请输入需要存入的金额：");
	while(1 != scanf("%lf",&money))
	{
		BUFF_CLEAR;
		printf("格式错误请重新输入：");
	}
	idcard->type = t_save;
	idcard->data.money=money;
	
	msgsnd(msgid,idcard,CTOS_LEN,0);
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	printf("%s\n",rec.tips);
}
void transfer_func(int msgid,Ctos_mes* idcard)
{
	printf("请输入需要转账的帐号：");
	while(1 != scanf("%lld",&mes.data.id_card))
	{
		BUFF_CLEAR;
		printf("格式错误请重新输入：");
	}
	printf("请输入需要转账的金额：");
	while(1 != scanf("%lf",&mes.data.money))
	{
		BUFF_CLEAR;
		printf("格式错误请重新输入：");
	}
	mes.type = t_transfer;
	idcard->type = t_transfer;
	//原来的帐号
	msgsnd(msgid,idcard,CTOS_LEN,0);
	//转账帐号和金额
	msgsnd(msgid,&mes,CTOS_LEN,0);
	
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	printf("%s\n",rec.tips);
}
void withdraw_func(int msgid,Ctos_mes* idcard)
{
	double money=0;
	printf("请输入需要取出的金额：");
	while(1 != scanf("%lf",&money))
	{
		BUFF_CLEAR;
		printf("格式错误请重新输入：");
	}
	idcard->type = t_withdraw;
	idcard->data.money=money;
	
	msgsnd(msgid,idcard,CTOS_LEN,0);
	msgrcv(msgid,&rec,STOC_LEN,getpid(),0);
	printf("%s\n",rec.tips);
}
