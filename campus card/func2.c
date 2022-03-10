//食堂消费

#include"func1.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>

//写入窗口每日消费记录，参数为各窗口每日消费记录数组、日期、金额和窗口号，无返回值
void write_eachday_record(window_record_of_day records[][1000], char* date, char* money,int window)
{
	char day[9];															//参数date是16位的完整时间，要截断为8为日期
	day[8] = '\0';
	int k;
	for (k = 0; k <= 7; k++)
	{
		day[k] = date[k];
	}
	for (k = 0; k <= 999; k++)
	{
		if (strcmp(records[window][k].date, "00000000" )==0)
			break;
		if (strcmp(records[window][k].date, day) == 0)						//记录中已有该日期
		{
			records[window][k].num_of_deal++;
			records[window][k].money += atof(money);
			return;
		}
	}
	strcpy(records[window][k].date, day);										//新建该日期的记录
	records[window][k].num_of_deal++;
	records[window][k].money += atof(money);
}

//打印窗口今日消费数量和总金额，参数为各窗口每日消费记录数组、日期和窗口号，无返回值
void print_day_of_window(window_record_of_day records[][1000], char *date,int window)
{
	char day[9];															//参数date是16位的完整时间，要截断为8为日期
	day[8] = '\0';
	int k;
	for (k = 0; k <= 7; k++)
	{
		day[k] = date[k];
	}
	for (k = 0; k <= 999; k++)
	{
		if (strcmp(records[window][k].date, day) == 0)
		{
			printf("窗口W%d今日总消费笔数:%d  总金额:%f\n", window,records[window][k].num_of_deal, records[window][k].money);
			break;
		}
	}
}

//手动消费函数，参数为账号数组、账号数量、总消费日志数组、消费日志总数、各窗口消费日志数组、各窗口消费日志最后一条消息位置数组、各窗口每日消费概况数组和系统最后一次消费日期，成功返回1，失败返回0
int consume(students* accounts,int *num_of_accounts,record_of_operation *records,int *num_of_records,each_window_record **window_record,int *position,int *position_1, window_record_of_day **record_of_day,char *latest_date)
{
	printf("请输入窗口号：");
	int number;
	scanf("%d", &number);
	printf("请输入操作时间：");
	char date[17];
	scanf("%s", date);
	if (atoll(date) < atoll(latest_date))
	{
		printf("时间设置错误!时间不能前于:%s\n",latest_date);
		getchar();
		return 0;
	}
	strcpy(latest_date, date);//更新操作时间
	char record_order[5] = "消费";
	char record_sentence[100];
	printf("请输入卡号：");
	char card_number[8];
	scanf("%s", card_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(card_number,accounts[k].card_now,7)==0)
			break;																//找到账户就跳出循环
		if (k == *num_of_accounts - 1)
		{
			printf("卡号不存在或不可用!\n");									//遍历数组后没找到就返回
			sprintf(record_sentence, "时间: %s 窗口: %d 卡号: %s 结果: %d", date, number, card_number, 0);
			//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
			//write_window_log(date, card_number, " - 1", 0, window_record, number, position);
			getchar();
			return 0;
		}
	}
	if (accounts[k].status == 0)
	{
		printf("该账号已销户!\n");
		getchar();
		return 0;
	}
	//检查是否是异常卡号
	if ((atoi(accounts[k].card_now) == 0)|| (atoi(accounts[k].card_now) == 1111111))
	{
		printf("卡号不存在或不可用!\n");
		sprintf(record_sentence, "时间: %s 窗口: %d 卡号: %s 结果: %d", date, number, card_number, 0);
		//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
		//write_window_log(date, card_number, -1, 0, window_record, number, position);
		getchar();
		return 0;
	}
	printf("当前余额为%f 请输入消费金额：", accounts[k].money);
	char money[8];
	scanf("%s", money);
	if (atof(money) <= 0)
	{
		printf("消费金额不能为0或负数!");
		sprintf(record_sentence, "时间: %s 窗口: %d 卡号: %s 金额: %f 结果: %d", date,number,card_number, atof(money),0);
		//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
		//write_window_log(date, card_number, money, 0, window_record, number, position);
		getchar();
		return 0;
	}
	if (accounts[k].money - atof(money) <0.0)
	{
		printf("余额不足!");
		sprintf(record_sentence, "时间: %s 窗口: %d 卡号: %s 金额: %f 结果: %d", date,number,card_number, atof(money), 0);
		//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
		//write_window_log(date, card_number, money, 0, window_record, number, position);
		getchar();
		return 0;
	}
	//检测当前用餐时间段累计金额是否超过20
	char password[5];
	char hour[3];
	hour[0] = date[8];
	hour[1] = date[9];
	hour[2] = '\0';
	int i_hour = atoi(hour);
	if (!(((i_hour >= 7) && (i_hour <= 9)) || ((i_hour >= 11) && (i_hour <= 13)) || ((i_hour >= 17) && (i_hour <= 19))))
		;//如果消费时间不在用餐时间段内，不用判断
	else
	{
		if (atoll(date) - atoll(accounts[k].latest_date) > 2000000)//与上一次消费时间相差超过两个小时，不继续累计
			accounts[k].money_20 = 0.0;
		accounts[k].money_20 += atof(money);//增加累计余额
		if (accounts[k].money_20 > 20.0)//超过20输入密码并归零
		{
			printf("请输入密码:");
			scanf("%s", password);
			if (strcmp(password, "8888") != 0)
			{
				printf("密码输入错误!\n");
				getchar();
				return 0;
			}
			accounts[k].money_20 = 0.0;//超过20，归零
		}
		strcpy(accounts[k].latest_date, date);//更新时间
	}

	accounts[k].money = (accounts[k].money * pow(10, 5) - atof(money) * pow(10, 5)) / pow(10, 5);
	printf("消费成功!\n");
	printf("当前窗口：W%d    消费时间：%s    卡号：%s    消费后余额：%.2f\n", number, date, accounts[k].card_now, accounts[k].money);
	sprintf(record_sentence, "时间: %s 窗口: %d 卡号: %s 金额: %.2f 结果: %d", date,number,card_number, atof(money), 1);
	//write_card_log(record_order, record_sentence, records, num_of_records);//写入总操作日志
	int check_code = ((date[10] - '0') + (card_number[6] - '0') + (int)(atof(money))+number) % 10;//校验码
	write_window_log(date, card_number, money, 1, window_record, number,position,position_1,check_code);
	write_eachday_record(record_of_day, date, money, number);
	print_day_of_window(record_of_day, date, number);//消费成功后，打印该窗口今日总笔数和金额
	getchar();
	return 1;
}

//初始化各窗口每日记录，参数为各窗口每日消费记录数组，无返回值
void initial_each_day_record(window_record_of_day records[][1000])
{
	int n, m;
	for (n = 1; n <= 99; n++)
	{
		for (m = 0; m <= 999; m++)
		{
			strcpy(records[n][m].date, "00000000");
			records[n][m].money = 0.0;
			records[n][m].num_of_deal = 0;
		}
	}
}
