//ʳ������

#include"func1.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>

//д�봰��ÿ�����Ѽ�¼������Ϊ������ÿ�����Ѽ�¼���顢���ڡ����ʹ��ںţ��޷���ֵ
void write_eachday_record(window_record_of_day records[][1000], char* date, char* money,int window)
{
	char day[9];															//����date��16λ������ʱ�䣬Ҫ�ض�Ϊ8Ϊ����
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
		if (strcmp(records[window][k].date, day) == 0)						//��¼�����и�����
		{
			records[window][k].num_of_deal++;
			records[window][k].money += atof(money);
			return;
		}
	}
	strcpy(records[window][k].date, day);										//�½������ڵļ�¼
	records[window][k].num_of_deal++;
	records[window][k].money += atof(money);
}

//��ӡ���ڽ��������������ܽ�����Ϊ������ÿ�����Ѽ�¼���顢���ںʹ��ںţ��޷���ֵ
void print_day_of_window(window_record_of_day records[][1000], char *date,int window)
{
	char day[9];															//����date��16λ������ʱ�䣬Ҫ�ض�Ϊ8Ϊ����
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
			printf("����W%d���������ѱ���:%d  �ܽ��:%f\n", window,records[window][k].num_of_deal, records[window][k].money);
			break;
		}
	}
}

//�ֶ����Ѻ���������Ϊ�˺����顢�˺���������������־���顢������־������������������־���顢������������־���һ����Ϣλ�����顢������ÿ�����Ѹſ������ϵͳ���һ���������ڣ��ɹ�����1��ʧ�ܷ���0
int consume(students* accounts,int *num_of_accounts,record_of_operation *records,int *num_of_records,each_window_record **window_record,int *position,int *position_1, window_record_of_day **record_of_day,char *latest_date)
{
	printf("�����봰�ںţ�");
	int number;
	scanf("%d", &number);
	printf("���������ʱ�䣺");
	char date[17];
	scanf("%s", date);
	if (atoll(date) < atoll(latest_date))
	{
		printf("ʱ�����ô���!ʱ�䲻��ǰ��:%s\n",latest_date);
		getchar();
		return 0;
	}
	strcpy(latest_date, date);//���²���ʱ��
	char record_order[5] = "����";
	char record_sentence[100];
	printf("�����뿨�ţ�");
	char card_number[8];
	scanf("%s", card_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(card_number,accounts[k].card_now,7)==0)
			break;																//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("���Ų����ڻ򲻿���!\n");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ʱ��: %s ����: %d ����: %s ���: %d", date, number, card_number, 0);
			//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
			//write_window_log(date, card_number, " - 1", 0, window_record, number, position);
			getchar();
			return 0;
		}
	}
	if (accounts[k].status == 0)
	{
		printf("���˺�������!\n");
		getchar();
		return 0;
	}
	//����Ƿ����쳣����
	if ((atoi(accounts[k].card_now) == 0)|| (atoi(accounts[k].card_now) == 1111111))
	{
		printf("���Ų����ڻ򲻿���!\n");
		sprintf(record_sentence, "ʱ��: %s ����: %d ����: %s ���: %d", date, number, card_number, 0);
		//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
		//write_window_log(date, card_number, -1, 0, window_record, number, position);
		getchar();
		return 0;
	}
	printf("��ǰ���Ϊ%f ���������ѽ�", accounts[k].money);
	char money[8];
	scanf("%s", money);
	if (atof(money) <= 0)
	{
		printf("���ѽ���Ϊ0����!");
		sprintf(record_sentence, "ʱ��: %s ����: %d ����: %s ���: %f ���: %d", date,number,card_number, atof(money),0);
		//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
		//write_window_log(date, card_number, money, 0, window_record, number, position);
		getchar();
		return 0;
	}
	if (accounts[k].money - atof(money) <0.0)
	{
		printf("����!");
		sprintf(record_sentence, "ʱ��: %s ����: %d ����: %s ���: %f ���: %d", date,number,card_number, atof(money), 0);
		//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
		//write_window_log(date, card_number, money, 0, window_record, number, position);
		getchar();
		return 0;
	}
	//��⵱ǰ�ò�ʱ����ۼƽ���Ƿ񳬹�20
	char password[5];
	char hour[3];
	hour[0] = date[8];
	hour[1] = date[9];
	hour[2] = '\0';
	int i_hour = atoi(hour);
	if (!(((i_hour >= 7) && (i_hour <= 9)) || ((i_hour >= 11) && (i_hour <= 13)) || ((i_hour >= 17) && (i_hour <= 19))))
		;//�������ʱ�䲻���ò�ʱ����ڣ������ж�
	else
	{
		if (atoll(date) - atoll(accounts[k].latest_date) > 2000000)//����һ������ʱ����������Сʱ���������ۼ�
			accounts[k].money_20 = 0.0;
		accounts[k].money_20 += atof(money);//�����ۼ����
		if (accounts[k].money_20 > 20.0)//����20�������벢����
		{
			printf("����������:");
			scanf("%s", password);
			if (strcmp(password, "8888") != 0)
			{
				printf("�����������!\n");
				getchar();
				return 0;
			}
			accounts[k].money_20 = 0.0;//����20������
		}
		strcpy(accounts[k].latest_date, date);//����ʱ��
	}

	accounts[k].money = (accounts[k].money * pow(10, 5) - atof(money) * pow(10, 5)) / pow(10, 5);
	printf("���ѳɹ�!\n");
	printf("��ǰ���ڣ�W%d    ����ʱ�䣺%s    ���ţ�%s    ���Ѻ���%.2f\n", number, date, accounts[k].card_now, accounts[k].money);
	sprintf(record_sentence, "ʱ��: %s ����: %d ����: %s ���: %.2f ���: %d", date,number,card_number, atof(money), 1);
	//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
	int check_code = ((date[10] - '0') + (card_number[6] - '0') + (int)(atof(money))+number) % 10;//У����
	write_window_log(date, card_number, money, 1, window_record, number,position,position_1,check_code);
	write_eachday_record(record_of_day, date, money, number);
	print_day_of_window(record_of_day, date, number);//���ѳɹ��󣬴�ӡ�ô��ڽ����ܱ����ͽ��
	getchar();
	return 1;
}

//��ʼ��������ÿ�ռ�¼������Ϊ������ÿ�����Ѽ�¼���飬�޷���ֵ
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
