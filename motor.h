#ifndef _MOTOR_H
#define  _MOTOR_H
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

#define LM2596_GPIO_PORT      GPIOC
#define LM2596_IN_1_PIN       GPIO_PIN_1
#define LM2596_IN_2_PIN       GPIO_PIN_2
#define LM2596_ENA_PIN        GPIO_PIN_4

//typedef void(*Motor_Action)(void);

enum MOTORSTATUS{
	MOTOR_NULL = 0,
	MOTOR_STOP  = 1,
	MOTOR_FORWARD = 2,
	MOTOR_BACKWARD = 3
};

/*struct MOTOR{
	enum MOTORSTATUS motor_status;
	Motor_Action motor_act;
};*/


void Free_Stop(void);
void Fast_Stop(void);
void Closing(void);
void Opening(void);

#endif


