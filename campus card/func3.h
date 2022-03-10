#ifndef _FUNC3_H
#define _FUNC3_H

//初始化wz003.txt文件，结果存入参数指向的两个数组中，无返回值
extern void get_wz(int* position_1,int *position_2);

//对学号进行模糊查询，参数为账号数组和账号总数，无返回值
extern void schoolNumber_regex(students* accounts, int* num_of_accounts);

//对姓名模糊查询，参数为账号数组和账号总数，无返回值
extern void name_regex(students* accounts, int* num_of_accounts);

//归并排序99个窗口的记录，按时间升序，参数为各窗口消费日志数组、储存排序结果数组、各窗口消费日志第一条位置和最后一条位置数组，返回结果数组元素总数
extern int sort_windows(each_window_record records[][60000], each_window_record* destination, int* position_1, int* position_2);

//查询截止日期内指定账户的消费金额，参数为账号数组、账号总数、排序后的消费日志数组和日志总数，无返回值
extern void query_sorted_records(students* accounts, int* num_of_accounts, each_window_record* records, int number);

//用第二种方法排序99个窗口的记录，按时间升序，参数为各窗口消费日志数组、储存排序结果数组、各窗口消费日志第一条位置和最后一条位置数组，返回结果数组元素总数
extern int qsort_windows(each_window_record records[][60000], each_window_record* destination, int* position_1, int* position_2);

#endif
