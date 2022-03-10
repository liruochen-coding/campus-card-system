#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"func1.h"
#include<regex.h>
#include<time.h>

//��ʼ��wz003.txt�ļ�������������ָ������������У��޷���ֵ
void get_wz(int *position_1,int *position_2)
{
	FILE* wz = NULL;
	wz = fopen("wz003.txt", "r");
	char ch = fgetc(wz);
	ch = fgetc(wz);
	ch = fgetc(wz);								//ȥ���ĵ���ͷ���(WZ)
	char sentence[20];
	char number[3];
	char pos[6];
	pos[5] = '\0';
	int k,j;
	while (fgets(sentence, 20, wz))
	{
		for (k = 0; sentence[k] != ','; k++)
		{
			number[k] = sentence[k];		//��ȡ���ں�
		}
		number[k] = '\0';
		for (j=k+1; sentence[j] != ';'; j++)
		{
			pos[j-k-1] = sentence[j];				//��ȡλ��
		}
		position_1[atoi(number)] = atoi(pos);			//��������
		position_2[atoi(number)] = atoi(pos);
	}
}

//��ѧ�Ž���ģ����ѯ������Ϊ�˺�������˺��������޷���ֵ
void schoolNumber_regex(students* accounts, int* num_of_accounts)
{
	int number = 0;//����ƥ��ɹ���ѧ������
	char reg_str[50];//����������ʽ
	printf("������������ʽ����^��ͷ����$��β��.����0����1���ַ���.{0,}����0�������ַ�:");
	scanf("%s", reg_str);
	regex_t reg;
	int result_1 = regcomp(&reg, reg_str, REG_EXTENDED);//����������ʽ
	//printf("%d\n", result_1);
	regmatch_t reg_struct[1];
	int k;
	int result_2 = 0;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (regexec(&reg, accounts[k].school_num, 1, reg_struct, 0) == 0)//����ƥ��
		{
			if ((reg_struct[0].rm_so == 0) && (reg_struct[0].rm_eo == 10))//���ƥ����Ƿ��������ַ���
			{
				result_2 = 1;
				number++;
				printf("ƥ��ɹ�!ѧ��Ϊ:%s  ����:%s\n", accounts[k].school_num,accounts[k].name);
			}
		}
	}
	if (result_2 == 0)
		printf("δ�ҵ�ƥ���ѧ��!\n");
	else
	{
		printf("һ��ƥ�䵽%d���˻�!\n", number);
	}
	regfree(&reg);
}

//������ģ����ѯ������Ϊ�˺�������˺��������޷���ֵ
void name_regex(students* accounts, int* num_of_accounts)
{
	int number=0;//һ��ƥ�䵽����
	char reg_str[20];//����ģ����ѯ��
	printf("������������ʽ����^��ͷ����$��β��.{0,2}����0����1�������ַ���.{0,}����0�����������ַ�:");
	scanf("%s", reg_str);
	regex_t reg;
	int result_1 = regcomp(&reg, reg_str, REG_EXTENDED);//����������ʽ
	//printf("%d\n", result_1);
	regmatch_t reg_struct[1];
	int k;
	int result_2 = 0;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (regexec(&reg, accounts[k].name, 1, reg_struct, 0) == 0)//����ƥ��
		{
			if ((reg_struct[0].rm_so == 0) && (reg_struct[0].rm_eo == strlen(accounts[k].name)))//���ƥ����Ƿ��������ַ���
			{
				result_2 = 1;
				number++;
				printf("ƥ��ɹ�!ѧ��Ϊ:%s  ����:%s\n", accounts[k].school_num, accounts[k].name);
			}
		}
	}
	if (result_2 == 0)
		printf("δ�ҵ�ƥ���ѧ��!\n");
	else
	{
		printf("һ��ƥ�䵽%d���˻�!\n", number);
	}
	regfree(&reg);
}

//�鲢����99�����ڵļ�¼����ʱ�����򣬲���Ϊ������������־���顢�������������顢������������־��һ��λ�ú����һ��λ�����飬���ؽ������Ԫ������
int sort_windows(each_window_record records[][60000],each_window_record *destination,int *position_1,int *position_2)
{
	clock_t start = clock();
	int k=0;//destination���±�
	int sub[100];//�������±꣬[0]��ʹ��
	int n;
	for (n = 1; n <= 99; n++)
	{
		sub[n] = position_1[n];//�±��ʼ��
	}
	int min_window;
	int w;
	while (1)
	{
		min_window = 0;
		//�Ƚ�99�����ڵ�ǰ��õ���Сֵ���ڵĴ���min_window
		for (w = 1; w <= 99; w++)
		{
			if (sub[w] < position_2[w])
			{
				min_window = w;//�ҵ���һ���±겻Խ���Ҵ�����Ϣ�Ĵ���
				break;
			}
		}
		if (!min_window)
			break;//���д����±궼Խ�磬����鲢���

		for (; w <= 99; w++)
		{
			if (sub[w] < position_2[w])
			{
				if (strcmp(records[w][sub[w]%60000].date, records[min_window][sub[min_window]%60000].date) < 0)
					min_window = w;//���w�±겻Խ�磬��w������С��min_window�����ڣ�����min_window
			}
		}

		destination[k] = records[min_window][sub[min_window]%60000];
		sub[min_window]++;//�±���1
		k++;
	}
	int check_code_all = 0;
	for (n = 0; n <= k; n++)
	{
		check_code_all += destination[n].check_code;
	}
	check_code_all = check_code_all % 167;
	printf("�������!����ʱ��%fms\n", (float)(clock() - start));
	printf("�ļ�У����Ϊ%d\n", check_code_all);
	return k;//����������
}

//��ѯ��ֹ������ָ���˻������ѽ�����Ϊ�˺����顢�˺�������������������־�������־�������޷���ֵ
void query_sorted_records(students *accounts,int *num_of_accounts,each_window_record* records, int number)
{
	char card[8];
	float money=0;
	printf("�������˻�:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!\n");									//���������û�ҵ��ͷ���
			return;
		}
	}
	int card_sub = k;
	printf("�����뿪ʼ����:");
	char date_1[17];
	scanf("%s", date_1);
	printf("�������ֹ����:");
	char date_2[17];
	scanf("%s", date_2);
	k = 0;
	struct list_cards* node;
	clock_t start = clock();
	while (1)
	{
		if (k > number)
			break;//����records�������˳�
		if (strcmp(records[k].date, date_1) < 0)
		{
			k++;
			continue;//����δ����ʼ�㣬��������Ϣ
		}
		if (strcmp(records[k].date, date_2) > 0)
			break;//���ڳ������˳�
		if (records[k].result == 0)
		{
			k++;
			continue;//����ʧ�ܣ������ü�¼
		}
		node = &(accounts[card_sub].cards);//ȡ��������
		while (node != NULL)//�������˻����п��Ų������Ѽ�¼
		{
			strcpy(card, node->card_number);
			if (strcmp(card, records[k].card_number) == 0)
			{
				printf("ѧ��:%s  ����:%s  ����:%s  ���ѽ��:%s\n", accounts[card_sub].school_num, card, records[k].date, records[k].money);
				money += atof(records[k].money);
			}
			node = node->next;
		}
		k++;
	}
	printf("\nѧ��:%s  ��%s��ֹ��%s  ������%.2fԪ!\n���β�ѯ��ʱ%fms\n", school_number, date_1,date_2, money,(float)(clock()-start));
}

//qsort()����������������������
int cmpfunc(const void* a, const void* b)
{
	return strcmp((*(each_window_record*)a).date, (*(each_window_record*)b).date);
}

//ð������
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
//ϣ������
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

//�õڶ��ַ�������99�����ڵļ�¼����ʱ�����򣬲���Ϊ������������־���顢�������������顢������������־��һ��λ�ú����һ��λ�����飬���ؽ������Ԫ������
int qsort_windows(each_window_record records[][60000], each_window_record* destination, int* position_1, int* position_2)
{
	clock_t start = clock();
	int k = 0;//destination���±�
	int sub[100];//�������±꣬[0]��ʹ��
	int n;
	for (n = 1; n <= 99; n++)
	{
		sub[n] = position_1[n];//�±��ʼ��
	}
	for (n = 1; n <= 99; n++)//�Ƚ�99�����ڼ�¼ȫ���Ž�һ��������
	{
		while (sub[n] < position_2[n])
		{
			destination[k] = records[n][sub[n]%60000];
			k++;
			sub[n]++;
		}
	}
	int last = k;//��¼destination�����±�(��0����)
	//qsort(destination, last + 1, sizeof(each_window_record), cmpfunc);
	//bubble_sort(destination, last + 1);//ð������̫����
	shell_sort(destination, last + 1);//ϣ������
	int check_code_all = 0;
	for (n = 0; n <= last; n++)
	{
		check_code_all += destination[n].check_code;
	}
	check_code_all = check_code_all % 167;
	printf("�������!����ʱ��%fms\n", (float)(clock() - start));
	printf("�ļ�У����Ϊ%d\n", check_code_all);
	return last;
}