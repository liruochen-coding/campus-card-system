#ifndef _FUNC3_H
#define _FUNC3_H

//��ʼ��wz003.txt�ļ�������������ָ������������У��޷���ֵ
extern void get_wz(int* position_1,int *position_2);

//��ѧ�Ž���ģ����ѯ������Ϊ�˺�������˺��������޷���ֵ
extern void schoolNumber_regex(students* accounts, int* num_of_accounts);

//������ģ����ѯ������Ϊ�˺�������˺��������޷���ֵ
extern void name_regex(students* accounts, int* num_of_accounts);

//�鲢����99�����ڵļ�¼����ʱ�����򣬲���Ϊ������������־���顢�������������顢������������־��һ��λ�ú����һ��λ�����飬���ؽ������Ԫ������
extern int sort_windows(each_window_record records[][60000], each_window_record* destination, int* position_1, int* position_2);

//��ѯ��ֹ������ָ���˻������ѽ�����Ϊ�˺����顢�˺�������������������־�������־�������޷���ֵ
extern void query_sorted_records(students* accounts, int* num_of_accounts, each_window_record* records, int number);

//�õڶ��ַ�������99�����ڵļ�¼����ʱ�����򣬲���Ϊ������������־���顢�������������顢������������־��һ��λ�ú����һ��λ�����飬���ؽ������Ԫ������
extern int qsort_windows(each_window_record records[][60000], each_window_record* destination, int* position_1, int* position_2);

#endif
