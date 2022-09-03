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
	char temp[20];
	while(1)
	{
		Ctos_mes buf;
		Stoc_mes buf2;

		read_msg(msgid,&buf,sizeof(buf),t_save);
		buf2.type = buf.pid;
		
		Ctos_mes f_mes;
		read_person_mes(buf.data.id_card,&f_mes);
		
		//改qian
		f_mes.data.money+=buf.data.money;
		write_person_mes(buf.data.id_card,&f_mes);
		sprintf(buf2.tips,"save succeed!");
		send_msg(msgid,&buf2);
	}
	return 0;
}
