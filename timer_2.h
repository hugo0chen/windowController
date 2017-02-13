#ifndef _TIMER_2_H
#define _TIMER_2_H
#include "stm8s.h"


uint32_t sys_TimeTick(void);

bool timeout(uint32_t last_time, uint32_t ms);

#endif