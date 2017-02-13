#ifndef __SCHD_H__
#define __SCHD_H__

#include "stm8s.h"

union SchdParameter {
	//void* ptr;
	char intvalue;
	//float floatvalue;
	//char chars[32];
};
enum TASKNUM {
	TASK_1 = 0,
	TASK_2 = 1,
	TASK_3 = 2
};
void resetSchdItem(enum TASKNUM num);

typedef void (*SchdCallback)(union SchdParameter);

/*
 @brief 在指定毫秒后调用回调函数，同时携带注册时的入参
 @param microsecond 延时毫秒数11
 @param p 回调时的参数
 @return 1=add one item, 0=no item added
 */
int Schd_After(int microsecond, SchdCallback callback, union SchdParameter p);

/*
 @brief 在指定毫秒后调用回调函数，同时携带注册时的入参
 @param microsecond 延时毫秒数
 @param value 回调时的参数，整数值
 @return 1=add one item, 0=no item added
 */
int Schd_After_Int(int microsecond, SchdCallback callback, int value);

/*
 @brief 运行调度器
 @param current_microsecond 当前毫秒数
 */
void Schd_Run(uint32_t current_microsecond);

#endif
