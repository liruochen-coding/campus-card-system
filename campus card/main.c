#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include"func1.h"
#include"func2.h"
#include"func3.h"

students accounts[10000];														//��������û�����Ϣ
int num_of_accounts;																//��ǰ�ж����û�
int serial_number = 312345;													//У԰����ˮ��
record_of_operation records_1[60000];									//��Ƭ���������־
int num_of_records_1 = 0;														//��Ƭ���������־�м���
record_of_operation records_2[3000000];								//ʳ�������ܲ�����־
int num_of_records_2 = 0;														//ʳ��������������־�м���
int window_position_1[100];													//�����ڵ�һ�����Ѽ�¼������[0]����
int window_position_2[100];													//��������һ�����Ѽ�¼������[0]����
each_window_record window_records[100][60000];				//���������Ѽ�¼��[0]����
each_window_record sorted_records[5940000];						//�鲢�����ĸ��������Ѽ�¼
window_record_of_day eachday_record[100][1000];				//������ÿ�ռ�¼��[0]����

int main(void)
{
	char date[17] = "2021122711003163";//����һ�����Ѳ���������
	int k;//case 17
	int n;//����switch()
	int m = 1;
	printf("��ʼ��������ÿ�����Ѽ�¼\n");
	initial_each_day_record(eachday_record);//��ʼ��������ÿ�����Ѽ�¼
	printf("������ÿ�����Ѽ�¼��ʼ���ɹ�\n");
	printf("��ʼ���������ѳ�ʼλ���ļ�\n");
	get_wz(window_position_1,window_position_2);//��ȡwz003.txt�ļ�
	printf("�������ѳ�ʼλ���ļ���ȡ�ɹ�!\n");
	printf("��ʼ�����������ļ�\n");
	accounts_initialization(accounts, &num_of_accounts,records_1,&num_of_records_1);//����������������
	printf("��ȡ�����������ݳɹ����˻�����:%d\n",num_of_accounts);
	getchar();
	cards_initialization(accounts, &num_of_accounts, &serial_number,records_1,&num_of_records_1);//��������
	printf("���������ɹ�!  ��ǰ��ˮ��:%d\n",serial_number);
	getchar();
	//��ȡ���������������ļ�
	int successful=operation_initialization(accounts, &num_of_accounts, &serial_number,records_1,&num_of_records_1,records_2,&num_of_records_2,window_records,window_position_2,window_position_1);
	printf("�ɹ���ȡ��%d����Ч��������!\n", successful);

	//while(1)���Ƴ���ѭ��
	while (m) {
		getchar();
		system("cls");
		printf("\n\n\n\n\n");
		printf("-------------------------------------------------------------------------------------------------\n");
		printf("|					/// У԰������ϵͳ \\\\\\					|\n");
		printf("|                            ���һ������ʱ��:    %s				|\n", date);
		printf("|                             �˺�����:%d    ��ˮ��:%d					|\n", num_of_accounts, serial_number);
		printf("|  ��Ƭ����			ʳ������		����					|\n");
		printf("|  0 �˳�			10 ����			12 ѧ��ģ����ѯ				|\n");
		printf("|  1 ����			11 ������������־	13 ����ģ����ѯ				|\n");
		printf("|  2 ����						14 ��ѯ�ۼ����ѽ���·�鲢����	|\n");
		printf("|  3 ��ʧ						15 ��ѯ�ۼ����ѽ�ϣ������		|\n");
		printf("|  4 ���											|\n");
		printf("|  5 ����											|\n");
		printf("|  6 ��ֵ											|\n");
		printf("|  7 ����											|\n");
		printf("|  8 �˻���ϸ��Ϣ��ѯ										|\n");
		printf("|  9 ������־											|\n");
		printf("-------------------------------------------------------------------------------------------------\n");
		scanf("%d", &n);
		switch (n) {
		case 0:
			m = 0;
			break;
		case 1:
			printf("��������\n");
			fflush(stdin);
			add_account(accounts,&num_of_accounts,&serial_number,records_1,&num_of_records_1);													//�����ֶ���������
			break;
		case 2:
			printf("��������\n");
			give_card(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);														//�����ֶ���������
			break;
		case 3:
			printf("��ʧ����\n");
			loss_register(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 4:
			printf("��Ҳ���\n");
			loss_unregister(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 5:
			printf("��������\n");
			reissue(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 6:
			printf("��ֵ����\n");
			recharge(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 7:
			printf("��������\n");
			close_account(accounts, &num_of_accounts, &serial_number, records_1, &num_of_records_1);
			break;
		case 8:
			printf("��ѯ�˻���ϸ���\n");
			query(accounts, &num_of_accounts, &serial_number);
			break;
		case 9:
			print_card_log(records_1, &num_of_records_1);
			break;
		case 10:
			printf("ʳ������\n");
			consume(accounts, &num_of_accounts,records_2,&num_of_records_2,window_records,window_position_2,window_position_1,eachday_record,date);
			break;
			/*  ��������־�����ܲ�����
		case 11:
			//print_card_log(records_2, &num_of_records_2);
			break;
			*/
		case 11:
			printf("��ӡ����������־\n");
			print_window_log(window_records, window_position_1, window_position_2);
			getchar();
			break;
		case 12:
			printf("��ѧ��ģ����ѯ\n");
			schoolNumber_regex(accounts, &num_of_accounts);
			getchar();
			break;
		case 14:
			printf("��ѯ�ۼ����ѽ���·�鲢����\n");
			k=sort_windows(window_records, sorted_records, window_position_1, window_position_2);
			query_sorted_records(accounts, &num_of_accounts, sorted_records, k);
			getchar();
			break;
		case 13:
			printf("������ģ����ѯ\n");
			name_regex(accounts, &num_of_accounts);
			getchar();
			break;
		case 15:
			printf("��ѯ�ۼ����ѽ�ϣ������\n");
			k=qsort_windows(window_records, sorted_records, window_position_1, window_position_2);
			query_sorted_records(accounts, &num_of_accounts, sorted_records, k);
			getchar();
			break;
			/*  �鿴������������־��ֹλ�ã�������
		case 17:
			for (k = 1; k <= 99; k++)
			{
				printf("%d  %d  %d\n", k, window_position_1[k], window_position_2[k]);
			}
			getchar();
			break;
			*/
		default:
			printf("��Ч������\n");
			getchar();
		}
	}
	//while(1)����

	return 0;
}
