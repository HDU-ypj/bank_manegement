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
	Ctos_mes buf;
	Ctos_mes old;
	Stoc_mes buf2;
	
	while(1)
	{
		//读取消息队列
		read_msg(msgid,&old,sizeof(buf),t_transfer);
		read_msg(msgid,&buf,sizeof(buf),t_transfer);
		buf2.type = buf.pid;
		
		read_person_mes(old.data.id_card,&old);//转账人数据
		if(old.data.money < buf.data.money)
		{
			sprintf(buf2.tips,"money not enough");
			send_msg(msgid,&buf2);
			continue;
		}
		old.data.money -= buf.data.money;
		write_person_mes(old.data.id_card,&old);//重新写入金额
		old.data.money = buf.data.money;
		
		if(!read_person_mes(buf.data.id_card,&buf))//收款人数据
		{
			sprintf(buf2.tips,"account B not exist!");
			send_msg(msgid,&buf2);
			continue;
		}
		buf.data.money += old.data.money;
		write_person_mes(buf.data.id_card,&buf);//重新写入金额
		
		
		write_person_mes(buf.data.id_card,&buf);
		sprintf(buf2.tips,"transfer success!");
		send_msg(msgid,&buf2);
	}
	return 0;
}
