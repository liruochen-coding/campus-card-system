#include"func1.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>

//д����������Ѽ�¼������Ϊ���ڡ����š������������������������־���顢���ںš�������������־���һ����Ϣλ������͸�����������־��һ����Ϣλ�����飬�޷���ֵ
void write_window_log(char* date, char *card_number,char *money,int result,each_window_record window_record[][60000], int window, int* window_position,int *window_position_1,int check_code)
{
	//printf("%d\n", window_position[window]);
	if (window_position[window] - 60000 > window_position_1[window])
		window_position_1[window]++;//��������ʱ��Ҫ���µ�һ����Ϣ��λ��
	strcpy(window_record[window][window_position[window]%60000].date,date );
	strcpy(window_record[window][window_position[window] % 60000].card_number, card_number);
	strcpy(window_record[window][window_position[window] % 60000].money, money);
	window_record[window][window_position[window] % 60000].check_code = check_code;
	window_record[window][window_position[window] % 60000].result = result;					//д���Ӧ����
	window_position[window]++;																							//��Ӧ���ڼ�¼����1
}

//д�뿨Ƭ���������־����������Ϊָ�Ҫ������ַ�����������־�������־�������޷���ֵ
void write_card_log(char* order, char* record,record_of_operation *records,int *num_of_records)
{
	++(*num_of_records);
	strcpy(records[*num_of_records-1].order, order);
	strcpy(records[*num_of_records-1].record, record);
}

//�����ʼ�����������뿪���������ݣ�����Ϊ�˺����顢�˺�������������־�������־�������޷���ֵ
void accounts_initialization(students *accounts,int *num_of_accounts, record_of_operation* records, int* num_of_records)
{
	char sentence[25];											//sentence�洢ÿһ�У��ٶԸ��н��з��룬��ע��һ�������ַ�ռ16λ
	char school_num[11];
	char name[10];
	char record_order[5] = "����";
	char record_sentence[100];								//����д�������־
	int n, m;															//���ڷ���ѧ���ַ����������ַ���
	int k = 0;															//���ڶ�accounts�ṹ�����鸳ֵ
	FILE* kh = NULL;
	kh = fopen("kh001.txt", "r");
	char ch = fgetc(kh);
	ch = fgetc(kh);
	ch = fgetc(kh);													//ȥ����һ���ĵ���ʶ(KH)
	//���д�����Ϣ
	while (fgets(sentence, 25, kh))
	{
		m = 0;
		for (n = 0; n <= 9; n++)
		{
			school_num[n] = sentence[n];
		}
		school_num[n++] = NULL;
		strcpy(accounts[k].school_num, school_num);			//����ȡѧ��
		while (sentence[n] != ';')
		{
			name[m++] = sentence[n++];
		}
		name[m] = NULL;
		strcpy(accounts[k].name, name);								//����ȡ����
		//printf��������Ƿ��ȡ��ȷ
		//printf("%s ", accounts[k].school_num);
		//printf("%s\n", accounts[k].name);
		accounts[k].effective_date = 20220220;						//��Ч����
		accounts[k].status = 1;												//�˺�״̬
		accounts[k].money = 0.0;											//���
		strcpy(accounts[k].latest_date, "0000000000000000");
		accounts[k].money_20 = 0.0;										//�ò�ʱ������һ������ʱ����ۼƽ��
		k++;
		sprintf(record_sentence, "ѧ��:%s ����:%s �ɹ�", school_num, name);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	}
	*num_of_accounts = k;														//��ǰ��ȡ�˶��ٸ��˻�
	fclose(kh);
}

//������������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void cards_initialization(students *accounts,int *num_of_accounts,int *serial_number, record_of_operation* records, int* num_of_records)
{
	char record_order[5] = "����";
	char record_sentence[100];												//����д�������־
	int k;																					//����ѭ��
	int card_number;																//����
	char array_card_number[8];												//�����ַ�����ʽ
	int first_six;																		//ǰ��λ
	char array_first_six[7];														//ǰ��λ���ַ�����ʽ
	int last;																				//ĩβУ����
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		first_six = ++( * serial_number);
		itoa(first_six, array_first_six, 10);
		last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
		last = last % 10;
		last = 9 - last;																//����У����
		card_number = first_six * 10 + last;							//���㿨��
		itoa(card_number, array_card_number, 10);
		strcpy(accounts[k].cards.card_number, array_card_number);
		accounts[k].cards.next = NULL;
		strcpy(accounts[k].card_now, array_card_number);
		//printf("%s   %s\n", accounts[k].school_num, accounts[k].card_now);
		sprintf(record_sentence, "ѧ��:%s ����:%s ����:%s �ɹ�", accounts[k].school_num, accounts[k].name, array_card_number);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	}
	//printf���������Ƿ���������
	/*
	for (k = 0; k <= num_of_accounts - 1; k++)
	{
		printf("school_num %s name %s status %d date %d card number %s money %f\n", accounts[k].school_num, accounts[k].name, accounts[k].status, accounts[k].effective_date, accounts[k].card_now,accounts[k].money);
	}
	*/
}

//��ѯ�˻���ϸ��Ϣ������Ϊ�˺����顢�˺���������ˮ�ţ��޷���ֵ
void query(students* accounts, int* num_of_accounts, int* serial_number)
{
	char school_number[11];
	printf("������ѧ��:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			getchar();
			return;
		}
	}
	if (accounts[k].status == 0)
	{
		printf("ѧ��������!");									//���������û�ҵ��ͷ���
		getchar();
		return;
	}
	printf("ѧ�ţ�%s\n", accounts[k].school_num);
	printf("���ţ�%s\n", accounts[k].card_now);
	printf("������%s\n", accounts[k].name);
	printf("��%.2f\n", accounts[k].money);
	getchar();
}



//�ֶ�����˻�����������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void add_account(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "����";
	char record_sentence[100];											//���ڲ�����־
	char input_num[20];
	char school_number[11];												//ѧ���ַ���
	char name[9];																//�����ַ���
	fflush(stdin);
	printf("�������˻�ѧ�ţ�");
	scanf("%s", input_num);
	if (strlen(input_num) != 10)											//�ж�����λ���Ƿ�����
	{
		printf("ѧ��λ������!");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	strcpy(school_number, input_num);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)					//����ѧ���Ƿ��Ѵ���
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
		{
			if (accounts[k].status == 1)
			{
				printf("ѧ���Ѵ���!\n");											//����ѧ��2020902103
				sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
				write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
				getchar();
			}
			else
			{
				printf("�˺Żָ��ɹ�!\n");
				accounts[k].status = 1;
				getchar();
			}
			return;
		}
	}
	printf("�������˻�������");
	scanf("%s", name);
	strcpy(accounts[k].school_num, school_number);			//�˻���ʼ��
	strcpy(accounts[k].name, name);
	strcpy(accounts[k].card_now, "1111111");
	accounts[k].cards.next = NULL;
	accounts[k].status = 1;
	accounts[k].effective_date = 20220220;
	accounts[k].money = 0.0;
	(* num_of_accounts)++;
	printf("�����ɹ�!  ѧ��:%s  ����:%s", accounts[k].school_num, accounts[k].name);
	sprintf(record_sentence, "ѧ��: %s ����: %s ���: %d", school_number, name, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	getchar();
}

//�ֶ���������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void give_card(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "����";
	char record_sentence[100];
	char school_number[11];
	printf("������ѧ��:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
			getchar();
			return;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		printf("���˻�������!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	if (atoi(accounts[k].card_now) != 1111111)
	{
		printf("���˻��Ѵ���У԰��!���貹����ʹ�ò�������\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	int card_number;																//����
	char array_card_number[8];												//�����ַ�����ʽ
	char array_first_six[7];														//ǰ��λ���ַ�����ʽ
	int last;																				//ĩβУ����
	(* serial_number)++;															//��ˮ����1
	itoa(*serial_number, array_first_six, 10);
	last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
	last = last % 10;
	last = 9 - last;																	//����У����
	card_number = *serial_number * 10 + last;						//���㿨��
	itoa(card_number, array_card_number, 10);
	strcpy(accounts[k].card_now, array_card_number);
	strcpy(accounts[k].cards.card_number, array_card_number);				//���濨��
	printf("�����ɹ�!   ����Ϊ:");
	puts(accounts[k].card_now);
	sprintf(record_sentence, "ѧ��: %s ����: %s ���: %d", school_number, array_card_number, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	getchar();
}

//��ֵ����������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void recharge(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "��ֵ";
	char record_sentence[100];
	char school_number[11];
	printf("������ѧ��:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
			getchar();
			return;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		printf("���˻�������!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ��ѹ�ʧ
	if (strcmp(accounts[k].card_now, "0000000") == 0)
	{
		printf("���˻�У԰���Ѿ���ʧ!����ʧ��\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ񷢹���
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("���˻���δ����!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//��ֵ
	printf("��ǰ���:  %f\n", accounts[k].money);
	printf("�������ֵ���:  ");
	float money_to_recharge = 0.0;
	scanf("%f", &money_to_recharge);
	if (money_to_recharge <= 0.0)
	{
		printf("��ֵ����Ϊ����!����ʧ��!");
		sprintf(record_sentence, "ѧ��: %s ���: %f ���: %d", school_number, money_to_recharge,0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	if (accounts[k].money + money_to_recharge > 999.99)
	{
		printf("��������!����ʧ��!");
		sprintf(record_sentence, "ѧ��: %s ���: %f ���: %d", school_number, money_to_recharge, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
	}
	else
	{
		accounts[k].money = accounts[k].money + money_to_recharge;
		printf("��ֵ�ɹ�!��ǰ�˻����:  %f", accounts[k].money);
		sprintf(record_sentence, "ѧ��: %s ���: %f ���: %d", school_number, money_to_recharge, 1);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
	}
}

//��������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void close_account(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "����";
	char record_sentence[100];
	char school_number[11];
	printf("������ѧ��:");
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
			getchar();
			return;
		}
	}
	if (accounts[k].status == 0)
	{
		printf("���˻�������!����ʧ��\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	accounts[k].status = 0;										//�������
	printf("�����ɹ�!");
	sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	getchar();
}

//�ֶ���ʧ����������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void loss_register(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "��ʧ";
	char record_sentence[100];
	printf("������ѧ��:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
			getchar();
			return;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		printf("���˻�������!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ��ѹ�ʧ
	if (strcmp(accounts[k].card_now, "0000000") == 0)
	{
		printf("���˻�У԰���Ѿ���ʧ!����ʧ��\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ񷢹���
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("���˻���δ����!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	strcpy(accounts[k].card_now, "0000000");					//��ʧ����
	printf("��ʧ�������!");
	sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 1);
	write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	getchar();
}

//�ֶ���Һ���������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void loss_unregister(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "���";
	char record_sentence[100];
	printf("������ѧ��:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
			getchar();
			return;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		printf("���˻�������!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ񷢹���
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("���˻���δ����!����ʧ��!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ�Ϊ��ʧ״̬
	if (strcmp(accounts[k].card_now, "0000000") != 0)
	{
		printf("���˻�У԰��δ��ʧ!����ʧ��!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	struct list_cards* p = &(accounts[k].cards);
	while (p->next != NULL)
	{
		p = p->next;											//�ҵ������������һ��
	}
	strcpy(accounts[k].card_now, p->card_number);
	printf("������!��ǰ����Ϊ:  %s\n", accounts[k].card_now);
	sprintf(record_sentence, "ѧ��: %s ����: %s ���: %d", school_number, accounts[k].card_now,1);
	write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	getchar();
}

//�ֶ���������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
void reissue(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records)
{
	char record_order[5] = "����";
	char record_sentence[100];
	printf("������ѧ��:");
	char school_number[11];
	scanf("%s", school_number);
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			printf("ѧ�Ų�����!");									//���������û�ҵ��ͷ���
			sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
			write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
			getchar();
			return;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		printf("���˻�������!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ񷢹���
	if (strcmp(accounts[k].card_now, "1111111") == 0)
	{
		printf("���˻���δ����!����ʧ��!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}
	//�ж��Ƿ�Ϊ��ʧ״̬
	if (strcmp(accounts[k].card_now, "0000000") != 0)
	{
		printf("���˻�У԰��δ��ʧ!����ʧ��!\n");
		sprintf(record_sentence, "ѧ��: %s ���: %d", school_number, 0);
		write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
		getchar();
		return;
	}

	int card_number;																//����
	char array_card_number[8];												//�����ַ�����ʽ
	char array_first_six[7];														//ǰ��λ���ַ�����ʽ
	int last;																				//ĩβУ����
	(* serial_number)++;															//��ˮ����1
	itoa(*serial_number, array_first_six, 10);
	last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
	last = last % 10;
	last = 9 - last;																	//����У����
	card_number = *serial_number * 10 + last;						//���㿨��
	itoa(card_number, array_card_number, 10);
	strcpy(accounts[k].card_now, array_card_number);
	struct list_cards* p = &(accounts[k].cards);
	//��������ĩβ����һ��
	while (p->next != NULL)
	{
		p = p->next;											//�ҵ������������һ��
	}
	p->next = malloc(sizeof(struct list_cards));
	strcpy(p->next->card_number, array_card_number);
	p->next->next = NULL;
	printf("���������ɹ�!�¿���Ϊ:  %s", accounts[k].card_now);
	sprintf(record_sentence, "ѧ��: %s ����: %s ���: %d", school_number, accounts[k].card_now,1);
	write_card_log(record_order, record_sentence, records, num_of_records);//д�������־
	getchar();
}

//������ʧ����������cz_func()������Ϊʱ�䡢ѧ�š��˺����飬�˺���������ˮ�ţ��޷���ֵ
int batch_loss_register(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			//printf("ѧ�Ų�����!\n");									//���������û�ҵ��ͷ���
			return 0;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		//printf("���˻�������!\n");
		return 0;
	}
	//�ж��Ƿ��ѹ�ʧ
	if (memcmp(accounts[k].card_now, "0000000",6) == 0)
	{
		//printf("���˻�У԰���Ѿ���ʧ!����ʧ��\n");
		return 0;
	}
	//�ж��Ƿ񷢹���
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("���˻���δ����!����ʧ��!\n");
		return 0;
	}
	strcpy(accounts[k].card_now, "0000000");
	//printf("��ʧ�ɹ�!���ţ� %s ʱ�䣺 %s\n", school_number, date);
	return 1;
}

//������Һ���������cz_func()������Ϊʱ�䡢ѧ�š��˺����飬�˺���������ˮ�ţ��޷���ֵ
int batch_loss_unregister(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			//printf("ѧ�Ų�����!\n");									//���������û�ҵ��ͷ���
			return 0;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		//printf("���˻�������!\n");
		return 0;
	}
	//�ж��Ƿ��ѹ�ʧ
	if (memcmp(accounts[k].card_now, "0000000",6) != 0)
	{
		//printf("���˻�У԰����δ��ʧ!����ʧ��\n");
		return 0;
	}
	//�ж��Ƿ񷢹���
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("���˻���δ����!����ʧ��!\n");
		return 0;
	}
	struct list_cards* p = &(accounts[k].cards);
	while (p->next != NULL)
	{
		p = p->next;											//�ҵ������������һ��
	}
	strcpy(accounts[k].card_now, p->card_number);
	//printf("��ҳɹ�!ѧ��:  %s  ���ڣ�%s\n", school_number, date);
	return 1;
}

//������������������cz_func()������Ϊʱ�䡢ѧ�š��˺����飬�˺���������ˮ�ţ��޷���ֵ
int batch_reissue(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			//printf("ѧ�Ų�����!\n");									//���������û�ҵ��ͷ���
			return 0;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		//printf("���˻�������!\n");
		return 0;
	}
	//�ж��Ƿ񷢹���
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("���˻���δ����!����ʧ��!\n");
		return 0;
	}
	//�ж��Ƿ��ѹ�ʧ
	if (memcmp(accounts[k].card_now, "0000000",6) != 0)
	{
		//printf("���˻�У԰����δ��ʧ!����ʧ��\n");
		return 0;
	}
	int card_number;																//����
	char array_card_number[8];												//�����ַ�����ʽ
	char array_first_six[7];														//ǰ��λ���ַ�����ʽ
	int last;																				//ĩβУ����
	(* serial_number)++;														//��ˮ����1
	itoa(*serial_number, array_first_six, 10);
	last = (array_first_six[0] - '0') + (array_first_six[1] - '0') + (array_first_six[2] - '0') + (array_first_six[3] - '0') + (array_first_six[4] - '0') + (array_first_six[5] - '0');
	last = last % 10;
	last = 9 - last;																	//����У����
	card_number = *serial_number * 10 + last;						//���㿨��
	itoa(card_number, array_card_number, 10);
	strcpy(accounts[k].card_now, array_card_number);
	struct list_cards* p = &(accounts[k].cards);
	//��������ĩβ����һ��
	while (p->next != NULL)
	{
		p = p->next;											//�ҵ������������һ��
	}
	p->next = malloc(sizeof(struct list_cards));
	strcpy(p->next->card_number, array_card_number);
	p->next->next = NULL;
	//printf("�����ɹ�!ѧ��:  %s  ���ڣ�%s\n", school_number, date);
	return 1;
}

//������ֵ����������cz_func()������Ϊʱ�䡢ѧ�š���ֵ���˺����飬�˺���������ˮ�ţ��޷���ֵ
int batch_recharge(char* date, char* school_number, char* money, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if (memcmp(school_number, accounts[k].school_num, 10) == 0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			//printf("ѧ�Ų�����!\n");									//���������û�ҵ��ͷ���
			return 0;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		//printf("���˻�������!\n");
		return 0;
	}
	//�ж��Ƿ��ѹ�ʧ
	if (memcmp(accounts[k].card_now, "0000000",6) == 0)
	{
		//printf("���˻�У԰���Ѿ���ʧ!����ʧ��\n");
		return 0;
	}
	//�ж��Ƿ񷢹���
	if (memcmp(accounts[k].card_now, "1111111",6) == 0)
	{
		//printf("���˻���δ����!\n");
		return 0;
	}
	//��ֵ
	float money_to_recharge = atoi(money);
	if (money_to_recharge <= 0.0)
	{
		//printf("��ֵ����Ϊ����!����ʧ��!\n");
		return 0;
	}
	if (accounts[k].money + money_to_recharge > 999.99)
	{
		//printf("��������!����ʧ��!\n");
		return 0;
	}
	else
	{
		accounts[k].money = accounts[k].money + money_to_recharge;
		//printf("��ֵ�ɹ�!ѧ��:  %s  ���ڣ�%s  ��%f\n", school_number, date, accounts[k].money);
		return 1;
	}
}

//������������������cz_func()������Ϊʱ�䡢ѧ�š��˺����飬�˺���������ˮ�ţ��޷���ֵ
int batch_close_account(char* date, char* school_number, students* accounts, int* num_of_accounts, int* serial_number)
{
	int k;
	for (k = 0; k <= *num_of_accounts - 1; k++)
	{
		if(memcmp(school_number,accounts[k].school_num,10)==0)
			break;															//�ҵ��˻�������ѭ��
		if (k == *num_of_accounts - 1)
		{
			//printf("ѧ�Ų�����!\n");									//���������û�ҵ��ͷ���
			return 0;
		}
	}
	//�ж��Ƿ�����
	if (accounts[k].status == 0)
	{
		//printf("���˻�������!����ʧ��\n");
		return 0;
	}
	accounts[k].status = 0;										//�������
	//printf("�����ɹ�!ѧ��:  %s  ���ڣ�%s\n", school_number, date);
	return 1;
}

//��ȡxf014.txt����¼ÿ�����ڵ�һ����Ϣ��ƫ��������������ڲ���position�����У�xf�Ǵ���ȡ���ļ�
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
			//printf("����W%d   %d   \n ", num_of_win, ftell(xf));
			if (num_of_win <= 9)
			{
				position[num_of_win] = ftell(xf) + 3;						//position[0]��ʹ��
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

//���ղ�ָͬ��ִ�ж�Ӧ����������operation_initialization()������Ϊָ����ڡ�ѧ�š�����ȡ����һ����Ϣ���˻����顢�˻���������ˮ�š�������־�������־�������ɹ�����1��ʧ�ܷ���0
int cz_func(char *order,char*date,char*school_number,char *money,char *sentence,students *accounts,int *num_of_accounts,int*serial_number,record_of_operation *records, int *num_of_records)
{
	char record_sentence[100];							//����д�������־
	int k;
	int successful = 0;
	//����ͬ��������ȡ��Ҫ��Ϣ
	if (strcmp(order, "��ʧ") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_loss_register(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("��ʧ  date %s number %s\n", date, school_number);
	}
	if (strcmp(order, "���") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_loss_unregister(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("���  date %s number %s\n", date, school_number);
	}
	if (strcmp(order, "����") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_reissue(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("����  date %s number %s\n", date, school_number);
	}
	if (strcmp(order, "��ֵ") == 0)
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
		//printf("��ֵ  date %s number %s money %s\n", date, school_number,money);
		sprintf(record_sentence, "����: %s ѧ��: %s ���: %s ���: %d", date, school_number, money,successful);
		write_card_log(order, record_sentence, records, num_of_records);//д�������־
		return successful;																				//��ֵ�����Ĳ�����־��һ������Ҫ��������
	}
	if (strcmp(order, "����") == 0)
	{
		for (k = 22; k <= 31; k++)
		{
			school_number[k - 22] = sentence[k];
		}
		successful += batch_close_account(date, school_number, accounts, num_of_accounts, serial_number);
		//printf("����  date %s number %s\n", date, school_number);
	}
	sprintf(record_sentence, "����: %s ѧ��: %s ���: %d", date, school_number, successful);
	write_card_log(order, record_sentence, records, num_of_records);//д�������־
	return successful;
}

//��ӡ������־������Ϊ������־�������־�������޷���ֵ
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
				printf("�ѵ����¼��β!\n");
				return;
			}
			printf("%s  %s\n", records[k].order, records[k].record);
		}
		printf("����0�˳����س�����ҳ\n");
		n += 150;
	}
	getchar();
}

//��ӡ������������־������Ϊ������������־���顢������������־��һ����Ϣλ������͸�����������־���һ����Ϣλ�����飬�޷���ֵ
void print_window_log(each_window_record window_record[][60000], int* position_1, int* position_2)
{
	printf("�����봰�ں�:");
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
				printf("�ѵ����¼��β!\n");
				return;
			}
			printf("No:%d    ", k % 60000);
			printf("����:%s  ", window_record[n][k % 60000].date);
			printf("����:%s  ", window_record[n][k % 60000].card_number);
			printf("���:%s  ", window_record[n][k % 60000].money);
			printf("���:%d  ", window_record[n][k % 60000].result);
			printf("У����:%d\n", window_record[n][k % 60000].check_code);
		}
		printf("����0�˳����س�����ҳ\n");
		m += 150;
	}
}

//�������Ѻ���������operation_initialization()������Ϊ�������ޡ��������ޡ�xf014.txt�ļ������������ļ��еĴ��������������ڵ�һ����Ϣλ����xf�ļ��е�ƫ�������˺����顢�˺���������������־���顢������־������������������־���顢������������־���һ����Ϣλ������͸�����������־��һ����Ϣλ������
int batch_consume(long long int down, long long int up, FILE* xf, int* num_of_window, int* position,students *accounts,int *num_of_accounts,record_of_operation *records,int *num_of_records,each_window_record **window_record,int *window_position,int *window_position_1)
{
	int check_code;//У����
	int successful = 0;											//��¼�����ɹ���
	char record_order[5] = "����";
	char record_sentence[100];
	int k,m,n;
	char sentence[50];
	char date[17];
	char card_number[8];
	card_number[7] = '\0';
	char money[10];
	date[16] = '\0';
	int j = 1;											//���ڸ�������ѭ��
	for (k = 1; k <= *num_of_window; k++)
	{
		printf("��ȡ��������������\n");
		fseek(xf, position[k], SEEK_SET);
		while (1)
		{
			j = 1;
			fgets(sentence, 50, xf);//sentence����ÿһ��
			if (sentence[0] == 'E')//�ж��ļ��Ƿ��Ѿ�����
				break;
			if (sentence[0] == 'W')//�жϸô�����Ϣ�Ƿ��Ѷ���
				break;
			//��sentence����ȡʱ��
			date[0] = sentence[8];
			date[1] = sentence[9];
			date[2] = sentence[10];
			date[3] = sentence[11];
			date[4] = sentence[12];
			date[5] = sentence[13];
			date[6] = sentence[14];
			date[7] = sentence[15];
			//sentence[16]�Ƕ���
			date[8] = sentence[17];
			date[9] = sentence[18];
			date[10] = sentence[19];
			date[11] = sentence[20];
			date[12] = sentence[21];
			date[13] = sentence[22];
			date[14] = sentence[23];
			date[15] = sentence[24];
			//�ж�ʱ���Ƿ��ڷ�Χ��
			if (!((atoll(date) >= down) && (atoll(date) < up)))
				break;
			position[k] = ftell(xf);//���µ�һ������ȡ��Ϣλ��
			//��sentence������ȡ����
			card_number[0] = sentence[0];
			card_number[1] = sentence[1];
			card_number[2] = sentence[2];
			card_number[3] = sentence[3];
			card_number[4] = sentence[4];
			card_number[5] = sentence[5];
			card_number[6] = sentence[6];
			//��sentence������ȡ���
			for (m = 26; sentence[m] != ';';m++)
			{
				money[m - 26] = sentence[m];
			}
			money[m - 26] = '\0';
			//�ۼ����
			//���ҿ��Ŷ�Ӧ�˻�
			for (n = 0; n <= *num_of_accounts - 1; n++)
			{
				if (memcmp(card_number,accounts[n].card_now,7)==0)
					break;																//�ҵ��˻�������ѭ��
				if (n == *num_of_accounts - 1)
				{
					//printf("���Ų����ڻ򲻿���!\n");									//���������û�ҵ��ͷ���
					//sprintf(record_sentence, "����: %s ����: %d ����: %s ���: %s ���: %d", date, k, card_number,money,0);
					//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
					//write_window_log(date, card_number, money, 0, window_record, k, window_position);
					j = 0;
				}
			}
			if (j == 0)
				continue;
			//����˻��Ƿ��쳣
			if (accounts[n].status == 0)
			{
				//printf("���˺�������!\n");
				//sprintf(record_sentence, "����: %s ����: %d ����: %s ���: %s ���: %d", date, k,card_number, money,0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;//�����޷�����
			}
			if ((atoi(accounts[n].card_now) == 0) || (atoi(accounts[n].card_now) == 1111111))
			{
				//printf("���Ų����ڻ򲻿���!\n");
				//sprintf(record_sentence, "����: %s ����: %d ����: %s ���: %s ���: %d", date, k,card_number, money,0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;
			}
			if (atof(money) <= 0.0)
			{
				//printf("���ѽ���Ϊ0����!\n");
				//sprintf(record_sentence, "����: %s ����: %d ����: %s ���: %s ���: %d", date, k, card_number, money, 0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;
			}
			if (accounts[n].money - atof(money) < 0.0)
			{
				//printf("����!\n");
				//sprintf(record_sentence, "����: %s ����: %d ����: %s ���: %s ���: %d", date, k, card_number, money,0);
				//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
				//write_window_log(date, card_number, money, 0, window_record, k, window_position);
				continue;
			}
			accounts[n].money = (accounts[n].money*pow(10,5) - atof(money)* pow(10, 5))/ pow(10, 5);
			successful++;
			//printf("���ѳɹ�! ���ѽ��: %s ���ں�:W%d ����: %s ���: %f\n", money, k, card_number, accounts[n].money);
			//sprintf(record_sentence, "����: %s ����: %d ����: %s ���: %s ���: %d", date, k, card_number, money,1);
			//write_card_log(record_order, record_sentence, records, num_of_records);//д���ܲ�����־
			check_code = ((date[10] - '0') + (card_number[6] - '0') + (int)(atof(money))+k)%10;
			write_window_log(date, card_number, money, 1, window_record, k, window_position,window_position_1,check_code);
		}
	}
	return successful;
}

//������������������Ϊ�˺����顢�˺���������ˮ�š�������־���顢��־��������������־���顢������־������������������־���顢������������־��һ����Ϣλ������͸�����������־���һ����Ϣλ�����飬����ֵΪ�����ɹ���
int operation_initialization(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records_1, int* num_of_records_1, record_of_operation* records_2, int* num_of_records_2,each_window_record **window_record, int* window_position,int *window_position_1)
{
	//�������ļ�
	FILE* xf = NULL;
	FILE* cz = NULL;
	cz = fopen("cz002.txt", "r");
	xf = fopen("xf014.txt", "r");
	//��ʼ��xf014.txt�����ڵ�һ����Ϣλ��
	int position[100];												//position[0]��ʹ��
	printf("��ʼ��xf014.txt�ļ���...\n");
	int num_of_window = xf_position(position, xf);
	printf("һ����ȡ��%d������������������\n", num_of_window);
	getchar();
	char ch = fgetc(cz);
	ch = fgetc(cz);
	ch = fgetc(cz);													//ȥ����һ���ĵ���ʶ(CZ)
	//���д�����Ϣ
	char sentence[50];											//������
	char order[5];													//����ָ��
	order[4] = '\0';
	char school_number[11];									//ѧ��
	school_number[10] = '\0';
	char date[17];													//����
	date[16] = '\0';
	char money[4];												//��������ǳ�ֵ����Ҫ��֪���
	money[3] = '\0';
	int k;																	//������ȡ��Ϣ
	int successful = 0;											//��¼�ɹ�������������
	int result;															//��ǰ�����ɹ�Ϊ1��ʧ��Ϊ0
	int successful_2 = 0;										//��¼�ɹ�������������
	printf("��ȡ���������ļ���...\n");
	while (fgets(sentence, 50, cz))							
	{
		//��ȡʱ��
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//��ȡָ��,sentence[17 18 19 20]ָ������
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021092300000000)
		{
			//������ʱ��ν�������
			successful_2+=batch_consume(2021092300000000, 2021101400000000, xf, &num_of_window, position,accounts,num_of_accounts,records_2,num_of_records_2,window_record,window_position, window_position_1);
			//������ȡ��һ��
			successful+=cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number,records_1,num_of_records_1);
			break;//����ѭ����������һʱ���
		}
		//���ղ�ָͬ����в���
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1,num_of_records_1);
	}
	printf("��ȡ���������ļ���...\n");
	while (fgets(sentence, 50, cz))
	{
		//��ȡʱ��
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//��ȡָ��,sentence[17 18 19 20]ָ������
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021101407000000)
		{
			//������ʱ��ν�������
			successful_2 += batch_consume(2021101407000000, 2021110400000000, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			//������ȡ��һ��
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			break;//����ѭ����������һʱ���
		}
		//���ղ�ָͬ����в���
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("��ȡ���������ļ���...\n");
	while (fgets(sentence, 50, cz))
	{
		//��ȡʱ��
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//��ȡָ��,sentence[17 18 19 20]ָ������
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021110407000000)
		{
			//������ʱ��ν�������
			successful_2 += batch_consume(2021110407000000, 2021112500000000, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			//������ȡ��һ��
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			break;//����ѭ����������һʱ���
		}
		//���ղ�ָͬ����в���
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("��ȡ���������ļ���...\n");
	while (fgets(sentence, 50, cz))
	{
		//��ȡʱ��
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//��ȡָ��,sentence[17 18 19 20]ָ������
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) >= 2021112507000000)
		{
			//������ʱ��ν�������
			successful_2 += batch_consume(2021112507000000, 2021121600000000, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			//������ȡ��һ��
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			break;//����ѭ����������һʱ���
		}
		//���ղ�ָͬ����в���
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("��ȡ���������ļ���...\n");
	while (fgets(sentence, 50, cz))
	{
		//��ȡʱ��
		for (k = 0; k <= 15; k++)
		{
			date[k] = sentence[k];
		}
		//��ȡָ��,sentence[17 18 19 20]ָ������
		order[0] = sentence[17];
		order[1] = sentence[18];
		order[2] = sentence[19];
		order[3] = sentence[20];
		if (atoll(date) == 2021121606440770)
		{
			//�������һ��
			successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
			//������ʱ��ν�������
			successful_2 += batch_consume(2021121607000000, 9999999999999999, xf, &num_of_window, position,accounts,num_of_accounts, records_2, num_of_records_2,window_record,window_position, window_position_1);
			break;//����ѭ��
		}
		//���ղ�ָͬ����в���
		successful += cz_func(order, date, school_number, money, sentence, accounts, num_of_accounts, serial_number, records_1, num_of_records_1);
	}
	printf("����ȡ��%d����Ч����������Ϣ\n", successful_2);
	fclose(cz);
	fclose(xf);
	return successful;
}