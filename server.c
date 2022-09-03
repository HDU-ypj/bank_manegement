/**************************************************
>	作	  者：杭电羊皮卷
>   文件名称: server.c
>	联系方式：weixin:QQ2997675141
>   创建日期：2022年09月01日
>   描    述：
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "head.h"

const char* function_map[]=
{
	"create.exe",	//创建账户（1）
	"destory.exe",	//销毁账户（1）
	"login.exe",	//登录（1）
	"unlock.exe",	//解锁（1）
	"change_sec.exe",//改密码（2）
	"query.exe",	//查询余额（2）
	"save.exe",		//存钱（2）
	"transfer.exe",	//转账（2）
	"withdraw.exe",	//取钱（2）
};

static pid_t childbuf[sizeof(function_map)/sizeof(function_map[0])];
static int msgid;

void release(void)
{
	//取消子进程
	for(int i=0; i<sizeof(function_map)/sizeof(function_map[0]); i++)
	{
		kill(childbuf[i],SIGINT);
	}
	//取消消息队列
	msgctl(msgid,IPC_RMID,NULL);
}
void sigint(int num)
{
	release();
	exit(0);
}
int main(int argc,const char* argv[])
{
	signal(SIGINT,sigint);
	//创建消息队列
	msgid = msgget(ftok(".",10086),IPC_CREAT|0644);
	if(0 > msgid)
	{
		perror("msgget");
		return -1;
	}
	for(int i=0; i<sizeof(function_map)/sizeof(function_map[0]); i++)
	{
		pid_t pid = vfork();
		if(pid == 0)
		{
			execl(function_map[i],function_map[i],NULL);
			perror("execlp");
			return -1;
		}
		else
		{
			childbuf[i] = pid;
		}
	}
	while(1)
	{
		char buf[20];
		printf("press quit to quit!\n");
		fgets_t(buf,20);
		if(is_quit(buf))
		{
			break;
		}
	}
	release();
	return 0;
}
