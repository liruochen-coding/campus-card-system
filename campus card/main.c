#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include"func1.h"
#include"func2.h"
#include"func3.h"

students accounts[10000];														//存放所有用户的信息
int num_of_accounts;																//当前有多少用户
int serial_number = 312345;													//校园卡流水号
record_of_operation records_1[60000];									//卡片管理操作日志
int num_of_records_1 = 0;														//卡片管理操作日志有几条
record_of_operation records_2[3000000];								//食堂消费总操作日志
int num_of_records_2 = 0;														//食堂消费总消费日志有几条
int window_position_1[100];													//各窗口第一条消费记录条数，[0]留空
int window_position_2[100];													//各窗口下一条消费记录条数，[0]留空
each_window_record window_records[100][60000];				//各窗口消费记录，[0]留空
each_window_record sorted_records[5940000];						//归并排序后的各窗口消费记录
window_record_of_day eachday_record[100][1000];				//各窗口每日记录，[0]留空

int main(void)
{
	char date[17] = "2021122711003163";//最新一次消费操作的日期
	int k;//case 17
	int n;//用于switch()
	int m = 1;
	printf("初始化各窗口每日消费记录\n");
	initial_each_day_record(eachday_record);//初始化各窗口每日消费记录
	printf("各窗口每日消费记录初始化成功\n");
	printf("初始化窗口消费初始位置文件\n");
	get_wz(window_position_1,window_position_2);//读取wz003.txt文件
	printf("窗口消费初始位置文件读取成功!\n");
	printf("初始化批量数据文件\n");
	accounts_initialization(accounts, &num_of_accounts,records_1,&num_of_records_1);//读入批量开户数据
	printf("读取批量开户数据成功！账户数量:%d\n",num_of_accounts);
	getchar();
	cards_initialization(accounts, &num_of_accounts, &serial_number,records_1,&num_of_records_1);//批量发卡
	printf("批量发卡成功!  当前流水号:%d\n",serial_number);
	getchar();
	//读取批量操作和消费文件
	int successful=operation_initialization(accounts, &num_of_accounts, &serial_number,records_1,&num_of_records_1,records_2,&num_of_records_2,window_records,window_position_2,window_position_1);
	printf("成功读取到%d条有效批量操作!\n", successful);

	//while(1)控制程序循环
	while (m) {
		getchar();
		system("cls");
		printf("\n\n\n\n\n");
		printf("-------------------------------------------------------------------------------------------------\n");
		printf("|					/// 校园卡管理系统 \\\\\\					|\n");
		printf("|                            最后一次消费时间:    %s				|\n", date);
		printf("|                             账号总数:%d    流水号:%d					|\n", num_of_accounts, serial_number);
		printf("|  卡片管理			食堂消费		汇总					|\n");
		printf("|  0 退出			10 消费			12 学号模糊查询				|\n");
		printf("|  1 开户			11 各窗口消费日志	13 姓名模糊查询				|\n");
		printf("|  2 发卡						14 查询累计消费金额（多路归并排序）	|\n");
		printf("|  3 挂失						15 查询累计消费金额（希尔排序）		|\n");
		printf("|  4 解挂											|\n");
		printf("|  5 补卡											|\n");
		printf("|  6 充值											|\n");
		printf("|  7 销户											|\n");
		printf("|  8 账户详细信息查询										|\n");
		printf("|  9 操作日志											|\n");
		printf("-------------------------------------------------------------------------------------------------\n");
		scanf("%d", &n);
		switch (n) {
		case 0:
			m = 0;
			break;
		case 1:
			printf("开户操作\n");
			fflush(stdin);
			add_account(accounts,&num_of_accounts,&serial_number,records_1,&num_of_records_1);													//调用手动开户函数
			break;
		case 2:
			printf("发卡操作\n");
			give_card(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);														//调用手动发卡函数
			break;
		case 3:
			printf("挂失操作\n");
			loss_register(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 4:
			printf("解挂操作\n");
			loss_unregister(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 5:
			printf("补卡操作\n");
			reissue(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 6:
			printf("充值操作\n");
			recharge(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 7:
			printf("销户操作\n");
			close_account(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 8:
			printf("查询账户详细情况\n");
			query(accounts, &num_of_accounts, &serial_number);
			break;
		case 9:
			print_card_log(records_1, &num_of_records_1);
			break;
		case 10:
			printf("食堂消费\n");
			consume(accounts, &num_of_accounts,records_2,&num_of_records_2,window_records,window_position_2,window_position_1,eachday_record,date);
			break;
			/*  总消费日志，功能不启用
		case 11:
			//print_card_log(records_2, &num_of_records_2);
			break;
			*/
		case 11:
			printf("打印窗口消费日志\n");
			print_window_log(window_records, window_position_1, window_position_2);
			getchar();
			break;
		case 12:
			printf("对学号模糊查询\n");
			schoolNumber_regex(accounts, &num_of_accounts);
			getchar();
			break;
		case 14:
			printf("查询累计消费金额（多路归并排序）\n");
			k=sort_windows(window_records, sorted_records, window_position_1, window_position_2);
			query_sorted_records(accounts, &num_of_accounts, sorted_records, k);
			getchar();
			break;
		case 13:
			printf("对姓名模糊查询\n");
			name_regex(accounts, &num_of_accounts);
			getchar();
			break;
		case 15:
			printf("查询累计消费金额（希尔排序）\n");
			k=qsort_windows(window_records, sorted_records, window_position_1, window_position_2);
			query_sorted_records(accounts, &num_of_accounts, sorted_records, k);
			getchar();
			break;
			/*  查看各窗口消费日志起止位置，不启用
		case 17:
			for (k = 1; k <= 99; k++)
			{
				printf("%d  %d  %d\n", k, window_position_1[k], window_position_2[k]);
			}
			getchar();
			break;
			*/
		default:
			printf("无效操作！\n");
			getchar();
		}
	}
	//while(1)结束

	return 0;
}
