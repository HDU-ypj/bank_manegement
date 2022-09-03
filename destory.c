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
#include "head.h"


int main(int argc,const char* argv[])
{
	//获取消息队列
	int msgid = get_msgid();
	char path[30];
	while(1)
	{
		Ctos_mes buf;
		Stoc_mes buf2;

		//读取消息队列
		read_msg(msgid,&buf,sizeof(buf),t_destory);
		
		//删除文件来销毁
		sprintf(path,"./source/%lld.txt",buf.data.id_card);
		FILE* frp = fopen(path,"r");
		if(frp == NULL)
		{
			sprintf(buf2.tips,"idcard :%lld not exist!",buf.data.id_card);
		}
		else
		{
			char key[20];
			fscanf(frp,"%lld %s %s",
				&buf.data.id_card,
				buf.data.id_num,
				key);
			fclose(frp);
			if(strcmp(key,buf.data.key) != 0)
			{
				sprintf(buf2.tips,"password error!");
			}
			else
			{
				remove(path);
				sprintf(buf2.tips,"succeed!");
			}
		}
		
		//回消息
		buf2.type = buf.pid;
		send_msg(msgid,&buf2);
	}
	return 0;
}
