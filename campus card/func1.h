//定义重要结构体和卡片操作相关函数

#ifndef _FUNC1_H
#define _FUNC1_H

// 定义卡号链表
struct list_cards {
	char card_number[8];
	struct list_cards* next;
};

//定义账户结构
typedef struct
{
	char school_num[11];															//学号
	char name[10];																		//姓名
	int status;																				//1代表账户可用，0代表销户状态
	int effective_date;																//有效日期
	struct list_cards cards;															//卡号链表
	char card_now[8];																//当前可用的卡号,1111111代表尚未发卡，0000000代表挂失状态
	float money;																		//余额
	char latest_date[17];															//在用餐时间段最后一次消费的时间
	float money_20;																	//超过20要输密码
}students;

//定义操作日志
typedef struct
{
	char order[5];																		//指令
	char record[100];																	//具体内容
}record_of_operation;

//定义结构体，储存各窗口每天有多少交易数和营业额
typedef struct
{
	char date[9];														//形如20220228
	int num_of_deal;													//累计交易数
	float money;														//总交易金额
}window_record_of_day;

//定义结构体，储存各窗口消费记录
typedef struct
{
	char date[17];														//形如2022030113540000
	char card_number[8];											//3204659
	char money[10];													//17.50
	int result;																//1代表成功，0代表失败
	int check_code;													//校验码
}each_window_record;

//程序初始化函数，读入开户批量数据，参数为账号数组、账号总数、操作日志数组和日志总数，无返回值
extern void accounts_initialization(students* accounts, int* num_of_accounts,record_of_operation *records,int *num_of_records);

//批量发卡函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void cards_initialization(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//查询账户详细信息，参数为账号数组、账号总数和流水号，无返回值
extern void query(students* accounts, int* num_of_accounts, int* serial_number);

//手动添加账户函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void add_account(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records);

//手动发卡函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void give_card(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//充值函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void recharge(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//销户函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void close_account(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//手动挂失函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void loss_register(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//手动解挂函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void loss_unregister(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//手动补卡函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
extern void reissue(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//批量操作函数，参数为账号数组、账号总数、流水号、操作日志数组、日志数量、总消费日志数组、消费日志数量、各窗口消费日志数组、各窗口消费日志第一条消息位置数组和各窗口消费日志最后一条消息位置数组，返回值为操作成功数
extern int operation_initialization(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records_1, int* num_of_records_1, record_of_operation* records_2, int* num_of_records_2, each_window_record** window_record, int* window_position,int *window_position_1);

//打印操作日志，参数为操作日志数组和日志总数，无返回值
extern void print_card_log(record_of_operation* records, int* num_of_records);

//打印各窗口消费日志，参数为各窗口消费日志数组、各窗口消费日志第一条消息位置数组和各窗口消费日志最后一条消息位置数组，无返回值
extern void print_window_log(each_window_record window_record[][60000], int* position_1, int* position_2);

//写入各窗口消费记录
void write_window_log(char* date, char* card_number, char* money, int result, each_window_record window_record[][60000], int window, int* window_position, int* window_position_1,int check_code);

#endif
