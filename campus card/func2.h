//���崰��������غ���

#ifndef _FUNC2_H
#define _FUNC2_H
#include"func1.h"

//�ֶ����Ѻ���������Ϊ�˺����顢�˺���������������־���顢������־������������������־���顢������������־���һ����Ϣλ�����顢������ÿ�����Ѹſ������ϵͳ���һ���������ڣ��ɹ�����1��ʧ�ܷ���0
extern int consume(students* accounts, int* num_of_accounts, record_of_operation* records, int* num_of_records, each_window_record** window_record, int* position, int* position_1, window_record_of_day** record_of_day, char* latest_date);

//��ʼ��������ÿ�ռ�¼������Ϊ������ÿ�����Ѽ�¼���飬�޷���ֵ
extern void initial_each_day_record(window_record_of_day records[][1000]);

#endif
