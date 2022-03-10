//定义窗口消费相关函数

#ifndef _FUNC2_H
#define _FUNC2_H
#include"func1.h"

//手动消费函数，参数为账号数组、账号数量、总消费日志数组、消费日志总数、各窗口消费日志数组、各窗口消费日志最后一条消息位置数组、各窗口每日消费概况数组和系统最后一次消费日期，成功返回1，失败返回0
extern int consume(students* accounts, int* num_of_accounts, record_of_operation* records, int* num_of_records, each_window_record** window_record, int* position, int* position_1, window_record_of_day** record_of_day, char* latest_date);

//初始化各窗口每日记录，参数为各窗口每日消费记录数组，无返回值
extern void initial_each_day_record(window_record_of_day records[][1000]);

#endif
