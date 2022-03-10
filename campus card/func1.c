#include"func1.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>

//写入各窗口消费记录，参数为日期、卡号、金额、操作结果、各窗口消费日志数组、窗口号、各窗口消费日志最后一条消息位置数组和各窗口消费日志第一条消息位置数组，无返回值
void write_window_log(char* date, char *card_number,char *money,int result,each_window_record window_record[][60000], int window, int* window_position,int *window_position_1,int check_code)
{
	//printf("%d\n", window_position[window]);
	if (window_position[window] - 60000 > window_position_1[window])
		window_position_1[window]++;//发生回绕时，要更新第一条消息的位置
	strcpy(window_record[window][window_position[window]%60000].date,date );
	strcpy(window_record[window][window_position[window] % 60000].card_number, card_number);
	strcpy(window_record[window][window_position[window] % 60000].money, money);
	window_record[window][window_position[window] % 60000].check_code = check_code;
	window_record[window][window_position[window] % 60000].result = result;					//写入对应数据
	window_position[window]++;																							//对应窗口记录数增1
}

//写入卡片管理操作日志函数，参数为指令、要储存的字符串、操作日志数组和日志总数，无返回值
void write_card_log(char* order, char* record,record_of_operation *records,int *num_of_records)
{
	++(*num_of_records);
	strcpy(records[*num_of_records-1].order, order);
	strcpy(records[*num_of_records-1].record, record);
}

//程序初始化函数，读入开户批量数据，参数为账号数组、账号总数、操作日志数组和日志总数，无返回值
void accounts_initialization(students *accounts,int *num_of_accounts, record_of_operation* records, int* num_of_records)
{
	char sentence[25];											//sentence存储每一行，再对该行进行分离，需注意一个中文字符占16位
	char school_num[11];
	char name[10];
	char record_order[5] = "开户";
	char record_sentence[100];								//用于写入操作日志
	int n, m;															//用于分离学号字符串和姓名字符串
	int k = 0;															//用于对accounts结构体数组赋值
	FILE* kh = NULL;
	kh = fopen("kh001.txt", "r");
	char ch = fgetc(kh);
	ch = fgetc(kh);
	ch = fgetc(kh);													//去除第一行文档标识(KH)
	//逐行处理信息
	while (fgets(sentence, 25, kh))
	{
		m = 0;
		for (n = 0; n <= 9; n++)
		{
			school_num[n] = sentence[n];
		}
		school_num[n++] = NULL;
		strcpy(accounts[k].school_num, school_num);			//先提取学号
		while (sentence[n] != ';')
		{
			name[m++] = sentence[n++];
		}
		name[m] = NULL;
		strcpy(accounts[k].name, name);								//再提取姓名
		//printf检查数据是否读取正确
		//printf("%s ", accounts[k].school_num);
		//printf("%s\n", accounts[k].name);
		accounts[k].effective_date = 20220220;						//有效日期
		accounts[k].status = 1;												//账号状态
		accounts[k].money = 0.0;											//余额
		strcpy(accounts[k].latest_date, "0000000000000000");
		accounts[k].money_20 = 0.0;										//用餐时间段最后一次消费时间和累计金额
		k++;
		sprintf(record_sentence, "学号:%s 姓名:%s 成功", school_num, name);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	}
	*num_of_accounts = k;														//当前读取了多少个账户
	fclose(kh);
}

//批量发卡函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void cards_initialization(students *accounts,int *num_of_accounts,int *serial_number, record_of_operation* records, int* num_of_records)
{
	char record_order[5] = "发卡";
	char record_sentence[100];												//用于写入操作日志
	int k;																					//控制循环
	int card_number;																//卡号
	char array_card_number[8];												//卡号字符串形式
	int first_six;																		//前六位
	char array_first_six[7];														//前六位的字符串形式
	int last;																				//末尾校验码
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		first_six = ++( * serial_number);
		itoa(first_six, array_first_six, 10);
		last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
		last = last % 10;
		last = 9 - last;																//计算校验码
		card_number = first_six * 10 + last;							//计算卡号
		itoa(card_number, array_card_number, 10);
		strcpy(accounts[k].cards.card_number, array_card_number);
		accounts[k].cards.next = NULL;
		strcpy(accounts[k].card_now, array_card_number);
		//printf("%s   %s\n", accounts[k].school_num, accounts[k].card_now);
		sprintf(record_sentence, "学号:%s 姓名:%s 卡号:%s 成功", accounts[k].school_num, accounts[k].name, array_card_number);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	}
	//printf测试数据是否正常储存
	/*
	for (k = 0; k <= num_of_accounts - 1; k++)
	{
		printf("school_num %s name %s status %d date %d card number %s money %f\n", accounts[k].school_num, accounts[k].name, accounts[k].status, accounts[k].effective_date, accounts[k].card_now,accounts[k].money);
	}
	*/
}

//查询账户详细信息，参数为账号数组、账号总数和流水号，无返回值
void query(students* accounts, int* num_of_accounts, int* serial_number)
{
	char school_number[11];
	printf("请输入学号:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			getchar();
			return;
		}
	}
	if (accounts[k].status == 0)
	{
		printf("学号已销户!");									//遍历数组后没找到就返回
		getchar();
		return;
	}
	printf("学号：%s\n", accounts[k].school_num);
	printf("卡号：%s\n", accounts[k].card_now);
	printf("姓名：%s\n", accounts[k].name);
	printf("余额：%.2f\n", accounts[k].money);
	getchar();
}



//手动添加账户函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void add_account(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "开户";
	char record_sentence[100];											//用于操作日志
	char input_num[20];
	char school_number[11];												//学号字符串
	char name[9];																//姓名字符串
	fflush(stdin);
	printf("请输入账户学号：");
	scanf("%s", input_num);
	if (strlen(input_num) != 10)											//判断输入位数是否正常
	{
		printf("学号位数错误!");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	strcpy(school_number, input_num);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)					//查找学号是否已存在
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
		{
			if (accounts[k].status == 1)
			{
				printf("学号已存在!\n");											//测试学号2020902103
				sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
				write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
				getchar();
			}
			else
			{
				printf("账号恢复成功!\n");
				accounts[k].status = 1;
				getchar();
			}
			return;
		}
	}
	printf("请输入账户姓名：");
	scanf("%s", name);
	strcpy(accounts[k].school_num, school_number);			//账户初始化
	strcpy(accounts[k].name, name);
	strcpy(accounts[k].card_now, "1111111");
	accounts[k].cards.next = NULL;
	accounts[k].status = 1;
	accounts[k].effective_date = 20220220;
	accounts[k].money = 0.0;
	(* num_of_accounts)++;
	printf("开户成功!  学号:%s  姓名:%s", accounts[k].school_num, accounts[k].name);
	sprintf(record_sentence, "学号: %s 姓名: %s 结果: %d", school_number, name, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	getchar();
}

//手动发卡函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void give_card(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "发卡";
	char record_sentence[100];
	char school_number[11];
	printf("请输入学号:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
			getchar();
			return;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		printf("该账户已销户!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	if (atoi(accounts[k].card_now) != 1111111)
	{
		printf("该账户已存在校园卡!如需补卡请使用补卡功能\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	int card_number;																//卡号
	char array_card_number[8];												//卡号字符串形式
	char array_first_six[7];														//前六位的字符串形式
	int last;																				//末尾校验码
	(* serial_number)++;															//流水号增1
	itoa(*serial_number, array_first_six, 10);
	last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
	last = last % 10;
	last = 9 - last;																	//计算校验码
	card_number = *serial_number * 10 + last;						//计算卡号
	itoa(card_number, array_card_number, 10);
	strcpy(accounts[k].card_now, array_card_number);
	strcpy(accounts[k].cards.card_number, array_card_number);				//储存卡号
	printf("发卡成功!   卡号为:");
	puts(accounts[k].card_now);
	sprintf(record_sentence, "学号: %s 卡号: %s 结果: %d", school_number, array_card_number, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	getchar();
}

//充值函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void recharge(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "充值";
	char record_sentence[100];
	char school_number[11];
	printf("请输入学号:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
			getchar();
			return;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		printf("该账户已销户!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否已挂失
	if (strcmp(accounts[k].card_now, "0000000") == 0)
	{
		printf("该账户校园卡已经挂失!操作失败\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否发过卡
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("该账户尚未发卡!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//充值
	printf("当前余额:  %f\n", accounts[k].money);
	printf("请输入充值金额:  ");
	float money_to_recharge = 0.0;
	scanf("%f", &money_to_recharge);
	if (money_to_recharge <= 0.0)
	{
		printf("充值金额不能为负数!操作失败!");
		sprintf(record_sentence, "学号: %s 金额: %f 结果: %d", school_number, money_to_recharge,0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	if (accounts[k].money + money_to_recharge > 999.99)
	{
		printf("金额超过上限!操作失败!");
		sprintf(record_sentence, "学号: %s 金额: %f 结果: %d", school_number, money_to_recharge, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
	}
	else
	{
		accounts[k].money = accounts[k].money + money_to_recharge;
		printf("充值成功!当前账户余额:  %f", accounts[k].money);
		sprintf(record_sentence, "学号: %s 金额: %f 结果: %d", school_number, money_to_recharge, 1);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
	}
}

//销户函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void close_account(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "销户";
	char record_sentence[100];
	char school_number[11];
	printf("请输入学号:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
			getchar();
			return;
		}
	}
	if (accounts[k].status == 0)
	{
		printf("该账户已销户!操作失败\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	accounts[k].status = 0;										//销户标记
	printf("操作成功!");
	sprintf(record_sentence, "学号: %s 结果: %d", school_number, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	getchar();
}

//手动挂失函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void loss_register(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "挂失";
	char record_sentence[100];
	printf("请输入学号:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
			getchar();
			return;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		printf("该账户已销户!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否已挂失
	if (strcmp(accounts[k].card_now, "0000000") == 0)
	{
		printf("该账户校园卡已经挂失!操作失败\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否发过卡
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("该账户尚未发卡!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	strcpy(accounts[k].card_now, "0000000");					//挂失操作
	printf("挂失操作完成!");
	sprintf(record_sentence, "学号: %s 结果: %d", school_number, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	getchar();
}

//手动解挂函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void loss_unregister(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "解挂";
	char record_sentence[100];
	printf("请输入学号:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
			getchar();
			return;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		printf("该账户已销户!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否发过卡
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("该账户尚未发卡!操作失败!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否为挂失状态
	if (strcmp(accounts[k].card_now, "0000000") != 0)
	{
		printf("该账户校园卡未挂失!操作失败!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	struct list_cards* p = &(accounts[k].cards);
	while (p->next != NULL)
	{
		p = p->next;											//找到卡号链表最后一项
	}
	strcpy(accounts[k].card_now, p->card_number);
	printf("解挂完成!当前卡号为:  %s\n", accounts[k].card_now);
	sprintf(record_sentence, "学号: %s 卡号: %s 结果: %d", school_number, accounts[k].card_now,1);
	write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	getchar();
}

//手动补卡函数，参数为账号数组、账号总数、流水号、操作日志数组和日志总数，无返回值
void reissue(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "补卡";
	char record_sentence[100];
	printf("请输入学号:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!");									//遍历数组后没找到就返回
			sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
			getchar();
			return;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		printf("该账户已销户!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否发过卡
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("该账户尚未发卡!操作失败!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}
	//判断是否为挂失状态
	if (strcmp(accounts[k].card_now, "0000000") != 0)
	{
		printf("该账户校园卡未挂失!操作失败!\n");
		sprintf(record_sentence, "学号: %s 结果: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
		getchar();
		return;
	}

	int card_number;																//卡号
	char array_card_number[8];												//卡号字符串形式
	char array_first_six[7];														//前六位的字符串形式
	int last;																				//末尾校验码
	(* serial_number)++;															//流水号增1
	itoa(*serial_number, array_first_six, 10);
	last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
	last = last % 10;
	last = 9 - last;																	//计算校验码
	card_number = *serial_number * 10 + last;						//计算卡号
	itoa(card_number, array_card_number, 10);
	strcpy(accounts[k].card_now, array_card_number);
	struct list_cards* p = &(accounts[k].cards);
	//卡号链表末尾新增一项
	while (p->next != NULL)
	{
		p = p->next;											//找到卡号链表最后一项
	}
	p->next = malloc(sizeof(struct list_cards));
	strcpy(p->next->card_number, array_card_number);
	p->next->next = NULL;
	printf("补卡操作成功!新卡号为:  %s", accounts[k].card_now);
	sprintf(record_sentence, "学号: %s 卡号: %s 结果: %d", school_number, accounts[k].card_now,1);
	write_card_log(record_order, record_sentence, records, num_of_records);//写入操作日志
	getchar();
}

//批量挂失函数，用于cz_func()，参数为时间、学号、账号数组，账号总数和流水号，无返回值
int batch_loss_register(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			//printf("学号不存在!\n");									//遍历数组后没找到就返回
			return 0;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		//printf("该账户已销户!\n");
		return 0;
	}
	//判断是否已挂失
	if (memcmp(accounts[k].card_now, "0000000",6) == 0)
	{
		//printf("该账户校园卡已经挂失!操作失败\n");
		return 0;
	}
	//判断是否发过卡
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("该账户尚未发卡!操作失败!\n");
		return 0;
	}
	strcpy(accounts[k].card_now, "0000000");
	//printf("挂失成功!卡号： %s 时间： %s\n", school_number, date);
	return 1;
}

//批量解挂函数，用于cz_func()，参数为时间、学号、账号数组，账号总数和流水号，无返回值
int batch_loss_unregister(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			//printf("学号不存在!\n");									//遍历数组后没找到就返回
			return 0;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		//printf("该账户已销户!\n");
		return 0;
	}
	//判断是否已挂失
	if (memcmp(accounts[k].card_now, "0000000",6) != 0)
	{
		//printf("该账户校园卡尚未挂失!操作失败\n");
		return 0;
	}
	//判断是否发过卡
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("该账户尚未发卡!操作失败!\n");
		return 0;
	}
	struct list_cards* p = &(accounts[k].cards);
	while (p->next != NULL)
	{
		p = p->next;											//找到卡号链表最后一项
	}
	strcpy(accounts[k].card_now, p->card_number);
	//printf("解挂成功!学号:  %s  日期：%s\n", school_number, date);
	return 1;
}

//批量补卡函数，用于cz_func()，参数为时间、学号、账号数组，账号总数和流水号，无返回值
int batch_reissue(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			//printf("学号不存在!\n");									//遍历数组后没找到就返回
			return 0;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		//printf("该账户已销户!\n");
		return 0;
	}
	//判断是否发过卡
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("该账户尚未发卡!操作失败!\n");
		return 0;
	}
	//判断是否已挂失
	if (memcmp(accounts[k].card_now, "0000000",6) != 0)
	{
		//printf("该账户校园卡尚未挂失!操作失败\n");
		return 0;
	}
	int card_number;																//卡号
	char array_card_number[8];												//卡号字符串形式
	char array_first_six[7];														//前六位的字符串形式
	int last;																				//末尾校验码
	(* serial_number)++;														//流水号增1
	itoa(*serial_number, array_first_six, 10);
	last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
	last = last % 10;
	last = 9 - last;																	//计算校验码
	card_number = *serial_number * 10 + last;						//计算卡号
	itoa(card_number, array_card_number, 10);
	strcpy(accounts[k].card_now, array_card_number);
	struct list_cards* p = &(accounts[k].cards);
	//卡号链表末尾新增一项
	while (p->next != NULL)
	{
		p = p->next;											//找到卡号链表最后一项
	}
	p->next = malloc(sizeof(struct list_cards));
	strcpy(p->next->card_number, array_card_number);
	p->next->next = NULL;
	//printf("补卡成功!学号:  %s  日期：%s\n", school_number, date);
	return 1;
}

//批量充值函数，用于cz_func()，参数为时间、学号、充值金额、账号数组，账号总数和流水号，无返回值
int batch_recharge(char* date, char* school_number, char* money, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			//printf("学号不存在!\n");									//遍历数组后没找到就返回
			return 0;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		//printf("该账户已销户!\n");
		return 0;
	}
	//判断是否已挂失
	if (memcmp(accounts[k].card_now, "0000000",6) == 0)
	{
		//printf("该账户校园卡已经挂失!操作失败\n");
		return 0;
	}
	//判断是否发过卡
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("该账户尚未发卡!\n");
		return 0;
	}
	//充值
	float money_to_recharge = atoi(money);
	if (money_to_recharge <= 0.0)
	{
		//printf("充值金额不能为负数!操作失败!\n");
		return 0;
	}
	if (accounts[k].money + money_to_recharge > 999.99)
	{
		//printf("金额超过上限!操作失败!\n");
		return 0;
	}
	else
	{
		accounts[k].money = accounts[k].money + money_to_recharge;
		//printf("充值成功!学号:  %s  日期：%s  余额：%f\n", school_number, date, accounts[k].money);
		return 1;
	}
}

//批量销户函数，用于cz_func()，参数为时间、学号、账号数组，账号总数和流水号，无返回值
int batch_close_account(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if(memcmp(school_number,accounts[k].school_num,10)==0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			//printf("学号不存在!\n");									//遍历数组后没找到就返回
			return 0;
		}
	}
	//判断是否销户
	if (accounts[k].status == 0)
	{
		//printf("该账户已销户!操作失败\n");
		return 0;
	}
	accounts[k].status = 0;										//销户标记
	//printf("销户成功!学号:  %s  日期：%s\n", school_number, date);
	return 1;
}

//读取xf014.txt，记录每个窗口第一条消息的偏移量，结果储存在参数position数组中，xf是待读取的文件
int xf_position(int* position, FILE* xf)
{
	int num_of_win = 0;
	char i = fgetc(xf);
	//char sentence[50];
	while (i != EOF)
	{
		if (i == 'W')
		{
			num_of_win++;
			//printf("读到W%d   %d   \n ", num_of_win, ftell(xf));
			if (num_of_win <= 9)
			{
				position[num_of_win] = ftell(xf) + 3;						//position[0]不使用
				fseek(xf, position[num_of_win], SEEK_SET);
			}
			else
			{
				position[num_of_win] = ftell(xf) + 4;
				fseek(xf, position[num_of_win], SEEK_SET);
			}
			//fgets(sentence, 50, xf);
			//puts(sentence);
		}
		i = fgetc(xf);
	}
	return num_of_win;
}

//按照不同指令执行对应操作，用于operation_initialization()，参数为指令、日期、学号、金额、读取到的一行信息、账户数组、账户总数、流水号、操作日志数组和日志总数，成功返回1，失败返回0
int cz_func(char *order,char*date,char*school_number,char *money,char *sentence,students *accounts,int *num_of_accounts,int*serial_number,record_of_operation *records, int *num_of_records)
{
	char record_sentence[100];							//用于写入操作日志
	int k;
	int successful = 0;
	//按不同操作，提取重要信息
	if (strcmp(order, "挂失") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_loss_register(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("挂失  date %s number %s\n", date, school_number);
	}
	if (strcmp(order, "解挂") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_loss_unregister(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("解挂  date %s number %s\n", date, school_number);
	}
	if (strcmp(order, "补卡") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_reissue(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("补卡  date %s number %s\n", date, school_number);
	}
	if (strcmp(order, "充值") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		for (k = 33; k <= 35; k++)
		{
			money[k - 33] = sentence[k];
		}
		successful += batch_recharge(date, school_number, money, accounts, num_of_accounts, serial_number);
		//printf("充值  date %s number %s money %s\n", date, school_number,money);
		sprintf(record_sentence, "日期: %s 学号: %s 金额: %s 结果: %d", date, school_number, money,successful);
		write_card_log(order, record_sentence, records, num_of_records);//写入操作日志
		return successful;																				//充值操作的操作日志多一个金额项，要单独操作
	}
	if (strcmp(order, "销户") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_close_account(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("销户  date %s number %s\n", date, school_number);
	}
	sprintf(record_sentence, "日期: %s 学号: %s 结果: %d", date, school_number, successful);
	write_card_log(order, record_sentence, records, num_of_records);//写入操作日志
	return successful;
}

//打印操作日志，参数为操作日志数组和日志总数，无返回值
void print_card_log(record_of_operation* records, int* num_of_records)
{
	system("cls");
	int k;
	int n = 0;
	while (getchar() != '0')
	{
		for (k = n; k <= n + 150; k++)
		{
			if (k > *num_of_records - 1)
			{
				printf("已到达记录结尾!\n");
				return;
			}
			printf("%s  %s\n", records[k].order, records[k].record);
		}
		printf("按下0退出，回车键翻页\n");
		n += 150;
	}
	getchar();
}

//打印各窗口消费日志，参数为各窗口消费日志数组、各窗口消费日志第一条消息位置数组和各窗口消费日志最后一条消息位置数组，无返回值
void print_window_log(each_window_record window_record[][60000], int* position_1, int* position_2)
{
	printf("请输入窗口号:");
	int n;
	scanf("%d", &n);
	system("cls");
	int k;
	int m = 0;
	while (getchar() != '0')
	{
		for (k = position_1[n] + m; k <= position_1[n] + m + 150; k++)
		{
			if (k > position_2[n] - 1)
			{
				printf("已到达记录结尾!\n");
				return;
			}
			printf("No:%d    ", k % 60000);
			printf("日期:%s  ", window_record[n][k % 60000].date);
			printf("卡号:%s  ", window_record[n][k % 60000].card_number);
			printf("金额:%s  ", window_record[n][k % 60000].money);
			printf("结果:%d  ", window_record[n][k % 60000].result);
			printf("校验码:%d\n", window_record[n][k % 60000].check_code);
		}
		printf("按下0退出，回车键翻页\n");
		m += 150;
	}
}

//批量消费函数，用于operation_initialization()，参数为日期下限、日期上限、xf014.txt文件、批量消费文件中的窗口总数、各窗口第一条消息位置在xf文件中的偏移量、账号数组、账号总数、总消费日志数组、消费日志总数、各窗口消费日志数组、各窗口消费日志最后一条消息位置数组和各窗口消费日志第一条消息位置数组
int batch_consume(long long int down, long long int up, FILE* xf, int* num_of_window, int* position,students *accounts,int *num_of_accounts,record_of_operation *records,int *num_of_records,each_window_record **window_record,int *window_position,int *window_position_1)
{
	int check_code;//校验码
	int successful = 0;											//记录操作成功数
	char record_order[5] = "消费";
	char record_sentence[100];
	int k,m,n;
	char sentence[50];
	char date[17];
	char card_number[8];
	card_number[7] = '\0';
	char money[10];
	date[16] = '\0';
	int j = 1;											//用于辅助跳出循环
	for (k = 1; k <= *num_of_window; k++)
	{
		printf("读取批量消费数据中\n");
		fseek(xf, position[k], SEEK_SET);
		while (1)
		{
			j = 1;
			fgets(sentence, 50, xf);//sentence储存每一行
			if (sentence[0] == 'E')//判断文件是否已经读完
				break;
			if (sentence[0] == 'W')//判断该窗口消息是否已读完
				break;
			//从sentence里提取时间
			date[0] = sentence[8];
			date[1] = sentence[9];
			date[2] = sentence[10];
			date[3] = sentence[11];
			date[4] = sentence[12];
			date[5] = sentence[13];
			date[6] = sentence[14];
			date[7] = sentence[15];
			//sentence[16]是逗号
			date[8] = sentence[17];
			date[9] = sentence[18];
			date[10] = sentence[19];
			date[11] = sentence[20];
			date[12] = sentence[21];
			date[13] = sentence[22];
			date[14] = sentence[23];
			date[15] = sentence[24];
			//判断时间是否在范围内
			if (!((atoll(date) >= down) && (atoll(date) < up)))
				break;
			position[k] = ftell(xf);//更新第一条待读取消息位置
			//从sentence里面提取卡号
			card_number[0] = sentence[0];
			card_number[1] = sentence[1];
			card_number[2] = sentence[2];
			card_number[3] = sentence[3];
			card_number[4] = sentence[4];
			card_number[5] = sentence[5];
			card_number[6] = sentence[6];
			//从sentence里面提取金额
			for (m = 26; sentence[m] != ';';m++)
			{
				money[m - 26] = sentence[m];
			}
			money[m - 26] = '\0';
			//扣减余额
			//先找卡号对应账户
			for (n = 0; n <= *num_of_accounts - 1; n++)
			{
				if (memcmp(card_number,accounts[n].card_now,7)==0)
					break;																//找到账户就跳出循环
				if (n == *num_of_accounts - 1)
				{
					//printf("卡号不存在或不可用!\n");									//遍历数组后没找到就返回
					//sprintf(record_sentence, "日期: %s 窗口: %d 卡号: %s 金额: %s 结果: %d", date, k, card_number,money,0);
					//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
					//write_window_log(date, card_number, money, 0, window_record, k, window_position);
					j = 0;
				}
			}
			if (j == 0)
				continue;
			//检查账户是否异常
			if (accounts[n].status == 0)
			{
				//printf("该账号已销户!\n");
				//sprintf(record_sentence, "日期: %s 窗口: %d 卡号: %s 金额: %s 结果: %d", date, k,card_number, money,0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;//销户无法消费
			}
			if ((atoi(accounts[n].card_now) == 0) || (atoi(accounts[n].card_now) == 1111111))
			{
				//printf("卡号不存在或不可用!\n");
				//sprintf(record_sentence, "日期: %s 窗口: %d 卡号: %s 金额: %s 结果: %d", date, k,card_number, money,0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;
			}
			if (atof(money) <= 0.0)
			{
				//printf("消费金额不能为0或负数!\n");
				//sprintf(record_sentence, "日期: %s 窗口: %d 卡号: %s 金额: %s 结果: %d", date, k, card_number, money, 0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;
			}
			if (accounts[n].money - atof(money) < 0.0)
			{
				//printf("余额不足!\n");
				//sprintf(record_sentence, "日期: %s 窗口: %d 卡号: %s 金额: %s 结果: %d", date, k, card_number, money,0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;
			}
			accounts[n].money = (accounts[n].money*pow(10,5) - atof(money)* pow(10, 5))/ pow(10, 5);
			successful++;
			//printf("消费成功! 消费金额: %s 窗口号:W%d 卡号: %s 余额: %f\n", money, k, card_number, accounts[n].money);
			//sprintf(record_sentence, "日期: %s 窗口: %d 卡号: %s 金额: %s 结果: %d", date, k, card_number, money,1);
			//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
			check_code = ((date[10] - '0') + (card_number[6] - '0') + (int)(atof(money))+k)%10;
			write_window_log(date, card_number, money, 1, window_record, k, window_position,window_position_1,check_code);
		}
	}
	return successful;
}

//批量操作函数，参数为账号数组、账号总数、流水号、操作日志数组、日志数量、总消费日志数组、消费日志数量、各窗口消费日志数组、各窗口消费日志第一条消息位置数组和各窗口消费日志最后一条消息位置数组，返回值为操作成功数
int operation_initialization(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records_1, int* num_of_records_1, record_of_operation* records_2, int* num_of_records_2,each_window_record **window_record, int* window_position,int *window_position_1)
{
	//打开两个文件
	FILE* xf = NULL;
	FILE* cz = NULL;
	cz = fopen("cz002.txt", "r");
	xf = fopen("xf014.txt", "r");
	//初始化xf014.txt各窗口第一条消息位置
	int position[100];												//position[0]不使用
	printf("初始化xf014.txt文件中...\n");
	int num_of_window = xf_position(position, xf);
	printf("一共读取到%d个窗口批量消费数据\n", num_of_window);
	getchar();
	char ch = fgetc(cz);
	ch = fgetc(cz);
	ch = fgetc(cz);													//去除第一行文档标识(CZ)
	//逐行处理信息
	char sentence[50];											//储存行
	char order[5];													//操作指令
	order[4] = '\0';
	char school_number[11];									//学号
	school_number[10] = '\0';
	char date[17];													//日期
	date[16] = '\0';
	char money[4];												//如果操作是充值，还要得知金额
	money[3] = '\0';
	int k;																	//用于提取信息
	int successful = 0;											//记录成功的批量操作数
	int result;															//当前操作成功为1，失败为0
	int successful_2 = 0;										//记录成功的批量消费数
	printf("读取批量操作文件中...\n");
	while (fgets(sentence, 50, cz))							
	{
		//提取时间
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//提取指令,sentence[17 18 19 20]指明操作
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021092300000000)
		{
			//对上述时间段进行消费
			successful_2+=batch_consume(2021092300000000, 2021101400000000, xf, &num_of_window, position,accounts,num_of_accounts,records_2,num_of_records_2,window_record,window_position, window_position_1);
			//处理多读取的一行
			successful+=cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number,records_1,num_of_records_1);
			break;//跳出循环，进入下一时间段
		}
		//按照不同指令进行操作
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1,num_of_records_1);
	}
	printf("读取批量操作文件中...\n");
	while (fgets(sentence, 50, cz))
	{
		//提取时间
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//提取指令,sentence[17 18 19 20]指明操作
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021101407000000)
		{
			//对上述时间段进行消费
			successful_2 += batch_consume(2021101407000000, 2021110400000000, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			//处理多读取的一行
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			break;//跳出循环，进入下一时间段
		}
		//按照不同指令进行操作
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("读取批量操作文件中...\n");
	while (fgets(sentence, 50, cz))
	{
		//提取时间
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//提取指令,sentence[17 18 19 20]指明操作
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021110407000000)
		{
			//对上述时间段进行消费
			successful_2 += batch_consume(2021110407000000, 2021112500000000, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			//处理多读取的一行
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			break;//跳出循环，进入下一时间段
		}
		//按照不同指令进行操作
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("读取批量操作文件中...\n");
	while (fgets(sentence, 50, cz))
	{
		//提取时间
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//提取指令,sentence[17 18 19 20]指明操作
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021112507000000)
		{
			//对上述时间段进行消费
			successful_2 += batch_consume(2021112507000000, 2021121600000000, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			//处理多读取的一行
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			break;//跳出循环，进入下一时间段
		}
		//按照不同指令进行操作
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("读取批量操作文件中...\n");
	while (fgets(sentence, 50, cz))
	{
		//提取时间
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//提取指令,sentence[17 18 19 20]指明操作
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) == 2021121606440770)
		{
			//处理最后一行
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			//对上述时间段进行消费
			successful_2 += batch_consume(2021121607000000, 9999999999999999, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			break;//跳出循环
		}
		//按照不同指令进行操作
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("共读取到%d条有效批量消费信息\n", successful_2);
	fclose(cz);
	fclose(xf);
	return successful;
}