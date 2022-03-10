//������Ҫ�ṹ��Ϳ�Ƭ������غ���

#ifndef _FUNC1_H
#define _FUNC1_H

// ���忨������
struct list_cards {
	char card_number[8];
	struct list_cards* next;
};

//�����˻��ṹ
typedef struct
{
	char school_num[11];															//ѧ��
	char name[10];																		//����
	int status;																				//1�����˻����ã�0��������״̬
	int effective_date;																//��Ч����
	struct list_cards cards;															//��������
	char card_now[8];																//��ǰ���õĿ���,1111111������δ������0000000�����ʧ״̬
	float money;																		//���
	char latest_date[17];															//���ò�ʱ������һ�����ѵ�ʱ��
	float money_20;																	//����20Ҫ������
}students;

//���������־
typedef struct
{
	char order[5];																		//ָ��
	char record[100];																	//��������
}record_of_operation;

//����ṹ�壬���������ÿ���ж��ٽ�������Ӫҵ��
typedef struct
{
	char date[9];														//����20220228
	int num_of_deal;													//�ۼƽ�����
	float money;														//�ܽ��׽��
}window_record_of_day;

//����ṹ�壬������������Ѽ�¼
typedef struct
{
	char date[17];														//����2022030113540000
	char card_number[8];											//3204659
	char money[10];													//17.50
	int result;																//1����ɹ���0����ʧ��
	int check_code;													//У����
}each_window_record;

//�����ʼ�����������뿪���������ݣ�����Ϊ�˺����顢�˺�������������־�������־�������޷���ֵ
extern void accounts_initialization(students* accounts, int* num_of_accounts,record_of_operation *records,int *num_of_records);

//������������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void cards_initialization(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//��ѯ�˻���ϸ��Ϣ������Ϊ�˺����顢�˺���������ˮ�ţ��޷���ֵ
extern void query(students* accounts, int* num_of_accounts, int* serial_number);

//�ֶ�����˻�����������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void add_account(students* accounts, int* num_of_accounts, int* serial_number,record_of_operation *records,int *num_of_records);

//�ֶ���������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void give_card(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//��ֵ����������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void recharge(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//��������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void close_account(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//�ֶ���ʧ����������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void loss_register(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//�ֶ���Һ���������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void loss_unregister(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//�ֶ���������������Ϊ�˺����顢�˺���������ˮ�š�������־�������־�������޷���ֵ
extern void reissue(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records, int* num_of_records);

//������������������Ϊ�˺����顢�˺���������ˮ�š�������־���顢��־��������������־���顢������־������������������־���顢������������־��һ����Ϣλ������͸�����������־���һ����Ϣλ�����飬����ֵΪ�����ɹ���
extern int operation_initialization(students* accounts, int* num_of_accounts, int* serial_number, record_of_operation* records_1, int* num_of_records_1, record_of_operation* records_2, int* num_of_records_2, each_window_record** window_record, int* window_position,int *window_position_1);

//��ӡ������־������Ϊ������־�������־�������޷���ֵ
extern void print_card_log(record_of_operation* records, int* num_of_records);

//��ӡ������������־������Ϊ������������־���顢������������־��һ����Ϣλ������͸�����������־���һ����Ϣλ�����飬�޷���ֵ
extern void print_window_log(each_window_record window_record[][60000], int* position_1, int* position_2);

//д����������Ѽ�¼
void write_window_log(char* date, char* card_number, char* money, int result, each_window_record window_record[][60000], int window, int* window_position, int* window_position_1,int check_code);

#endif
