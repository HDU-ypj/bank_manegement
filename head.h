#ifndef HEAD_H
#define HEAD_H


enum Msg_type
{
	t_create=1,	//1
	t_destory,	//1
	t_login,	//1
	t_unlock,	//1
	t_change_sec,//
	t_query,	//
	t_save,		//
	t_transfer,	//
	t_withdraw,	//
};
typedef struct Mes_data
{
	long long id_card;	//银行卡id
	char id_num[20];		//身份证号
	char key[20];			//密码
	double money;			//余额
	char is_lock;			//是否锁定
}Mes_data;
typedef struct Ctos_mes
{
	long type;
	int pid;
	Mes_data data;
}Ctos_mes;

typedef struct Stoc_mes
{
	long type;
	char tips[30];
}Stoc_mes;

#define BUFF_CLEAR stdin->_IO_read_ptr = stdin->_IO_read_end	//清空缓存区
#define CTOS_LEN	sizeof(Ctos_mes)-sizeof(long)
#define STOC_LEN	sizeof(Stoc_mes)-sizeof(long)

void create_func(int msgid);
void destory_func(int msgid);
int login_func(int msgid,Ctos_mes* idcard);
void unlock_func(int msgid);

void change_sec_func(int msgid,Ctos_mes* idcard);
void query_func(int msgid,Ctos_mes* idcard);
void save_func(int msgid,Ctos_mes* idcard);
void transfer_func(int msgid,Ctos_mes* idcard);
void withdraw_func(int msgid,Ctos_mes* idcard);

int read_person_mes(long long id,Ctos_mes* mes);
int write_person_mes(long long id,Ctos_mes* mes);

char *fgets_t(char*str,int len);	//fgets的完善，将'\n''\0'合并为'\0'
int get_choise(void);
void press_anykey_continue(void);
int get_msgid(void);
int read_msg(int msgid,void* buf,size_t bufsize,enum Msg_type type);
void send_msg(int msgid,void* buf);
int is_quit(const char* str);
long long int get_idcard(void);
void write_idcard(long long int id);

#endif
