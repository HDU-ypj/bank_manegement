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
	Ctos_mes f_mes;
	Ctos_mes buf;
	Stoc_mes buf2;
	int msgid = get_msgid();
	while(1)
	{
		//读取消息队列
		read_msg(msgid,&buf,sizeof(buf),t_query);
		buf2.type = buf.pid;
		
		read_person_mes(buf.data.id_card,&f_mes);
		
		printf("%lld %s %s %.2f %hhd\n",
		f_mes.data.id_card,
		f_mes.data.id_num,
		f_mes.data.key,
		f_mes.data.money,
		f_mes.data.is_lock);

		sprintf(buf2.tips,"you have %.2f left!",f_mes.data.money);
		send_msg(msgid,&buf2);
	}
	return 0;
}
