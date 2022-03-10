#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"func1.h"
#include<regex.h>
#include<time.h>

//初始化wz003.txt文件，结果存入参数指向的两个数组中，无返回值
void get_wz(int *position_1,int *position_2)
{
	FILE* wz = NULL;
	wz = fopen("wz003.txt", "r");
	char ch = fgetc(wz);
	ch = fgetc(wz);
	ch = fgetc(wz);								//去除文档开头标记(WZ)
	char sentence[20];
	char number[3];
	char pos[6];
	pos[5] = '\0';
	int k,j;
	while (fgets(sentence, 20, wz))
	{
		for (k = 0; sentence[k] != ','; k++)
		{
			number[k] = sentence[k];		//提取窗口号
		}
		number[k] = '\0';
		for (j=k+1; sentence[j] != ';'; j++)
		{
			pos[j-k-1] = sentence[j];				//提取位置
		}
		position_1[atoi(number)] = atoi(pos);			//存入数组
		position_2[atoi(number)] = atoi(pos);
	}
}

//对学号进行模糊查询，参数为账号数组和账号总数，无返回值
void schoolNumber_regex(students* accounts, int* num_of_accounts)
{
	int number = 0;//储存匹配成功的学号总数
	char reg_str[50];//储存正则表达式
	printf("请输入正则表达式，以^开头，以$结尾，.代表0个或1个字符，.{0,}代表0个或多个字符:");
	scanf("%s", reg_str);
	regex_t reg;
	int result_1 = regcomp(&reg, reg_str, REG_EXTENDED);//编译正则表达式
	//printf("%d\n", result_1);
	regmatch_t reg_struct[1];
	int k;
	int result_2 = 0;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (regexec(&reg, accounts[k].school_num, 1, reg_struct, 0) == 0)//进行匹配
		{
			if ((reg_struct[0].rm_so == 0) && (reg_struct[0].rm_eo == 10))//检测匹配的是否是整个字符串
			{
				result_2 = 1;
				number++;
				printf("匹配成功!学号为:%s  姓名:%s\n", accounts[k].school_num,accounts[k].name);
			}
		}
	}
	if (result_2 == 0)
		printf("未找到匹配的学号!\n");
	else
	{
		printf("一共匹配到%d个账户!\n", number);
	}
	regfree(&reg);
}

//对姓名模糊查询，参数为账号数组和账号总数，无返回值
void name_regex(students* accounts, int* num_of_accounts)
{
	int number=0;//一共匹配到几个
	char reg_str[20];//储存模糊查询串
	printf("请输入正则表达式，以^开头，以$结尾，.{0,2}代表0个或1个中文字符，.{0,}代表0个或多个中文字符:");
	scanf("%s", reg_str);
	regex_t reg;
	int result_1 = regcomp(&reg, reg_str, REG_EXTENDED);//编译正则表达式
	//printf("%d\n", result_1);
	regmatch_t reg_struct[1];
	int k;
	int result_2 = 0;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (regexec(&reg, accounts[k].name, 1, reg_struct, 0) == 0)//进行匹配
		{
			if ((reg_struct[0].rm_so == 0) && (reg_struct[0].rm_eo == strlen(accounts[k].name)))//检测匹配的是否是整个字符串
			{
				result_2 = 1;
				number++;
				printf("匹配成功!学号为:%s  姓名:%s\n", accounts[k].school_num, accounts[k].name);
			}
		}
	}
	if (result_2 == 0)
		printf("未找到匹配的学号!\n");
	else
	{
		printf("一共匹配到%d个账户!\n", number);
	}
	regfree(&reg);
}

//归并排序99个窗口的记录，按时间升序，参数为各窗口消费日志数组、储存排序结果数组、各窗口消费日志第一条位置和最后一条位置数组，返回结果数组元素总数
int sort_windows(each_window_record records[][60000],each_window_record *destination,int *position_1,int *position_2)
{
	clock_t start = clock();
	int k=0;//destination的下标
	int sub[100];//各窗口下标，[0]不使用
	int n;
	for (n = 1; n <= 99; n++)
	{
		sub[n] = position_1[n];//下标初始化
	}
	int min_window;
	int w;
	while (1)
	{
		min_window = 0;
		//比较99各窗口当前项，得到最小值所在的窗口min_window
		for (w = 1; w <= 99; w++)
		{
			if (sub[w] < position_2[w])
			{
				min_window = w;//找到第一个下标不越界且存有消息的窗口
				break;
			}
		}
		if (!min_window)
			break;//所有窗口下标都越界，代表归并完成

		for (; w <= 99; w++)
		{
			if (sub[w] < position_2[w])
			{
				if (strcmp(records[w][sub[w]%60000].date, records[min_window][sub[min_window]%60000].date) < 0)
					min_window = w;//如果w下标不越界，且w的日期小于min_window的日期，更新min_window
			}
		}

		destination[k] = records[min_window][sub[min_window]%60000];
		sub[min_window]++;//下标增1
		k++;
	}
	int check_code_all = 0;
	for (n = 0; n <= k; n++)
	{
		check_code_all += destination[n].check_code;
	}
	check_code_all = check_code_all % 167;
	printf("排序完成!所用时间%fms\n", (float)(clock() - start));
	printf("文件校验码为%d\n", check_code_all);
	return k;//返回总数量
}

//查询截止日期内指定账户的消费金额，参数为账号数组、账号总数、排序后的消费日志数组和日志总数，无返回值
void query_sorted_records(students *accounts,int *num_of_accounts,each_window_record* records, int number)
{
	char card[8];
	float money=0;
	printf("请输入账户:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("学号不存在!\n");									//遍历数组后没找到就返回
			return;
		}
	}
	int card_sub = k;
	printf("请输入开始日期:");
	char date_1[17];
	scanf("%s", date_1);
	printf("请输入截止日期:");
	char date_2[17];
	scanf("%s", date_2);
	k = 0;
	struct list_cards* node;
	clock_t start = clock();
	while (1)
	{
		if (k > number)
			break;//超过records总数，退出
		if (strcmp(records[k].date, date_1) < 0)
		{
			k++;
			continue;//日期未到起始点，跳过该消息
		}
		if (strcmp(records[k].date, date_2) > 0)
			break;//日期超过，退出
		if (records[k].result == 0)
		{
			k++;
			continue;//消费失败，跳过该记录
		}
		node = &(accounts[card_sub].cards);//取卡号链表
		while (node != NULL)//遍历该账户所有卡号查找消费记录
		{
			strcpy(card, node->card_number);
			if (strcmp(card, records[k].card_number) == 0)
			{
				printf("学号:%s  卡号:%s  日期:%s  消费金额:%s\n", accounts[card_sub].school_num, card, records[k].date, records[k].money);
				money += atof(records[k].money);
			}
			node = node->next;
		}
		k++;
	}
	printf("\n学号:%s  从%s截止至%s  共消费%.2f元!\n本次查询耗时%fms\n", school_number, date_1,date_2, money,(float)(clock()-start));
}

//qsort()辅助函数，定义日期升序
int cmpfunc(const void* a, const void* b)
{
	return strcmp((*(each_window_record*)a).date, (*(each_window_record*)b).date);
}

//冒泡排序
void bubble_sort(each_window_record* destination, int len) {
	int i, j;
	each_window_record temp;
	for (i = 0; i < len - 1; i++)
		for (j = 0; j < len - 1 - i; j++)
			if (memcmp(destination[j].date , destination[j + 1].date,16)>0) 
			{
				temp = destination[j];
				destination[j] = destination[j + 1];
				destination[j + 1] = temp;
			}
}
//希尔排序
void shell_sort(each_window_record* destination, int len)
{
	int gap, i, j;
	each_window_record temp;
	for (gap = len >> 1; gap > 0; gap = gap >> 1)
	{
		for (i = gap; i < len; i++) {
			temp = destination[i];
			for (j = i - gap; j >= 0 && memcmp(destination[j].date, temp.date, 16) > 0; j -= gap)
				destination[j + gap] = destination[j];
			destination[j + gap] = temp;
		}
	}
}

//用第二种方法排序99个窗口的记录，按时间升序，参数为各窗口消费日志数组、储存排序结果数组、各窗口消费日志第一条位置和最后一条位置数组，返回结果数组元素总数
int qsort_windows(each_window_record records[][60000], each_window_record* destination, int* position_1, int* position_2)
{
	clock_t start = clock();
	int k = 0;//destination的下标
	int sub[100];//各窗口下标，[0]不使用
	int n;
	for (n = 1; n <= 99; n++)
	{
		sub[n] = position_1[n];//下标初始化
	}
	for (n = 1; n <= 99; n++)//先将99个窗口记录全都放进一个数组中
	{
		while (sub[n] < position_2[n])
		{
			destination[k] = records[n][sub[n]%60000];
			k++;
			sub[n]++;
		}
	}
	int last = k;//记录destination最后的下标(从0计算)
	//qsort(destination, last + 1, sizeof(each_window_record), cmpfunc);
	//bubble_sort(destination, last + 1);//冒泡排序太慢了
	shell_sort(destination, last + 1);//希尔排序
	int check_code_all = 0;
	for (n = 0; n <= last; n++)
	{
		check_code_all += destination[n].check_code;
	}
	check_code_all = check_code_all % 167;
	printf("排序完成!所用时间%fms\n", (float)(clock() - start));
	printf("文件校验码为%d\n", check_code_all);
	return last;
}