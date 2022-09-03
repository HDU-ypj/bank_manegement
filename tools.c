/**************************************************
>	作	  者：杭电羊皮卷
>   文件名称: tools.c
>	联系方式：weixin:QQ2997675141
>   创建日期：2022年09月01日
>   描    述：
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "head.h"

int get_choise(void)
{
	int choise;
	while(scanf("%d",&choise)!=1)
	{
		BUFF_CLEAR;
	}
	return choise;
}

void press_anykey_continue(void)
{
	printf("按任意键继续……");
	fflush(stdout);
	BUFF_CLEAR;
	getchar();
	BUFF_CLEAR;
	system("clear");
}

int get_msgid(void)
{
	int id = msgget(ftok(".",10086),0);
	if(0 > id)
	{
		perror("msgget");
		exit(-1);
	}
	return id;
}
//子进程调用接收数据
int read_msg(int msgid,void* buf,size_t bufsize,enum Msg_type type)
{
	int ret = msgrcv(msgid,buf,bufsize-sizeof(long),type,0);
	if(0 >= ret)
	{
		perror("msgrcv");	
		exit(-1);
	}
	return ret;
}
//子进程调用发回数据
void send_msg(int msgid,void* buf)
{
	if(msgsnd(msgid,buf,STOC_LEN,0))
	{
		perror("msgsnd");
		exit(-1);
	}
}

int is_quit(const char* str)
{
	int flag=0;
	if(0 == strcmp(str,"quit"))
	{
		flag = 1;
	}
	return flag;
}

char *fgets_t(char*str,int len)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;//清空缓存区
	fgets(str,len,stdin);
	stdin->_IO_read_ptr = stdin->_IO_read_end;//清空缓存区

	for(int i=1;i<len;i++)
	{
		if(str[i]=='\n')
		{
			str[i] = '\0';
		}
	}
	return str;
}

long long get_idcard(void)
{
	long long id = -1;
	FILE* frp = fopen("./source/idcard_max.txt","r");
	if(frp < 0)
	{
		perror("fopen");
		return -1;
	}
	fscanf(frp,"%lld",&id);
	fclose(frp);
	return id;
}

void write_idcard(long long id)
{
	FILE* fwp = fopen("./source/idcard_max.txt","w");
	if(fwp < 0)
	{
		perror("fopen");
		return;
	}
	fprintf(fwp,"%lld",id);
	fclose(fwp);
	return;
}

int read_person_mes(long long id,Ctos_mes* mes)
{
	char buff[30]={};
	
	sprintf(buff,"./source/%lld.txt",id);
	FILE* frp = fopen(buff,"r");
	if(frp == NULL)
	{
		printf("open fail\n");
		return 0;
	}
	
	fscanf(frp,"%lld %s %s %lf %hhd",
		&(mes->data.id_card),
		mes->data.id_num,
		mes->data.key,
		&(mes->data.money),
		&(mes->data.is_lock));

	fclose(frp);
	return 1;
}

int write_person_mes(long long id,Ctos_mes* mes)
{
	char buff[30]={};
	
	sprintf(buff,"./source/%lld.txt",id);
	FILE* fwp = fopen(buff,"w");
	if(fwp == NULL)
	{
		return 0;
	}
	
	fprintf(fwp,"%lld %s %s %.2f %hhd",
		mes->data.id_card,
		mes->data.id_num,
		mes->data.key,
		mes->data.money,
		mes->data.is_lock);
	fclose(fwp);
	/*printf("%lld %s %s %.2f %hhd\n",
		mes->data.id_card,
		mes->data.id_num,
		mes->data.key,
		mes->data.money,
		mes->data.is_lock);
	fclose(fwp);
	*/
	return 1;
}


