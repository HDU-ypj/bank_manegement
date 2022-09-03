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

		//读取消息队列
		read_msg(msgid,&buf,sizeof(buf),t_create);
		
		//创建账户
		sprintf(temp,"./source/%lld.txt",buf.data.id_card);
		FILE* fwp = fopen(temp,"w");
		fprintf(fwp,"%lld %s %s %.2f %hhd",
			buf.data.id_card,
			buf.data.id_num,
			buf.data.key,
			buf.data.money,
			buf.data.is_lock);
		fclose(fwp);
		
		//回消息
		buf2.type = buf.pid;
		sprintf(buf2.tips,"account :%lld create!",buf.data.id_card);
		send_msg(msgid,&buf2);
	}
	return 0;
}
