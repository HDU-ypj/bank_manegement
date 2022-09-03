/**************************************************
>	作	  者：杭电羊皮卷
>   文件名称: create.c
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


int main(int argc,const char* argv[])
{
	//获取消息队列
	int msgid = get_msgid();
	char old[20];
	while(1)
	{
		Ctos_mes buf;
		Stoc_mes buf2;

		//读取消息队列
		read_msg(msgid,&buf,sizeof(buf),t_change_sec);
		strcpy(old,buf.data.key);
		//printf("old get\n");
		read_msg(msgid,&buf,sizeof(buf),t_change_sec);
		buf2.type = buf.pid;
		//printf("new get\n");
		Ctos_mes f_mes;
		read_person_mes(buf.data.id_card,&f_mes);
		if(strcmp(f_mes.data.key,old) == 0)
		{
			sprintf(buf2.tips,"key wrong!");
			send_msg(msgid,&buf2);
			continue;
		}
		
		//改密码
		strcpy(f_mes.data.key,buf.data.key);
		write_person_mes(buf.data.id_card,&f_mes);
		sprintf(buf2.tips,"change success");
		send_msg(msgid,&buf2);
	}
	return 0;
}
