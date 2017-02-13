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
 @brief ��ָ���������ûص�������ͬʱЯ��ע��ʱ�����
 @param microsecond ��ʱ������11
 @param p �ص�ʱ�Ĳ���
 @return 1=add one item, 0=no item added
 */
int Schd_After(int microsecond, SchdCallback callback, union SchdParameter p);

/*
 @brief ��ָ���������ûص�������ͬʱЯ��ע��ʱ�����
 @param microsecond ��ʱ������
 @param value �ص�ʱ�Ĳ���������ֵ
 @return 1=add one item, 0=no item added
 */
int Schd_After_Int(int microsecond, SchdCallback callback, int value);

/*
 @brief ���е�����
 @param current_microsecond ��ǰ������
 */
void Schd_Run(uint32_t current_microsecond);

#endif
