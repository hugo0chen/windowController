#include "timer_2.h"
#include "schd.h"

#define TIMES_STEP 10

uint32_t sys_time_tick;
/**
  * @brief Timer2 Update/Overflow Interrupt routine.
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
	sys_time_tick = sys_time_tick + TIMES_STEP;
	//Schd_Run(sys_time_tick);
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}

uint32_t sys_TimeTick(void) {
	return sys_time_tick;
}

bool timeout(uint32_t last_time, uint32_t ms) {
	return (bool)( sys_time_tick > (last_time + ms));
}





