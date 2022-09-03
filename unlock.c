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
		Ctos_mes mes;
		Stoc_mes buf2;

		//读取消息队列
		read_msg(msgid,&buf,sizeof(buf),t_unlock);
		buf2.type = buf.pid;
		
		//比对是否存在帐号
		if(0 == read_person_mes(buf.data.id_card,&mes))
		{
			sprintf(buf2.tips,"idcard :%lld not exist!",buf.data.id_card);
			send_msg(msgid,&buf2);
			continue;
		}
		
		//身份证号对比
		if(strcmp(buf.data.id_num,mes.data.id_num) != 0)
		{
			sprintf(buf2.tips,"account not exist!");
			send_msg(msgid,&buf2);
			continue;
		}
		
		mes.data.is_lock = 3;
		write_person_mes(mes.data.id_card,&mes);
		sprintf(buf2.tips,"unlock succeed!");
		send_msg(msgid,&buf2);
	}
	return 0;
}
