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
		Ctos_mes buf;	//接收消息队列缓存
		Ctos_mes dat;	//文件读取真实信息
		Stoc_mes buf2;	//发送消息队列缓存

		//读取消息队列
		read_msg(msgid,&buf,sizeof(buf),t_login);
		buf2.type = buf.pid;
		//读取文件信息
		if(!read_person_mes(buf.data.id_card,&dat))
		{
			sprintf(buf2.tips,"id :%lld not exit!",buf.data.id_card);
			send_msg(msgid,&buf2);
			continue;
		}
		
		//比对是否锁定
		if(dat.data.is_lock == 0)
		{
			sprintf(buf2.tips,"id :%lld is locked!",buf.data.id_card);
			send_msg(msgid,&buf2);
			continue;
		}
		//比对密码
		if(strcmp(dat.data.key,buf.data.key) == 0)
		{
			dat.data.is_lock = 3;
			write_person_mes(dat.data.id_card,&dat);
			sprintf(buf2.tips,"id :%lld landed succeed!",buf.data.id_card);
			send_msg(msgid,&buf2);
			continue;
		}
		
		dat.data.is_lock--;
		write_person_mes(dat.data.id_card,&dat);
		
		sprintf(buf2.tips,"you have %d chances!",dat.data.is_lock);
		send_msg(msgid,&buf2);
	}
	return 0;
}
