#ifndef _INIT_DRIVERS_H
#define _INIT_DRIVERS_H

#include "stm8s.h"

#define PHOTOSENSOR_GPIO_PORT  GPIOA
#define PHOTOSENSOR_GPIO_PIN   GPIO_PIN_2

void init_clk(void) ;
void init_timer_1(void);
void init_timer_2(void);
void init_led(void);
void Init_UART2(void);
void init_LM2596(void);
void init_PhotoSensor(void);
void init_Remoter(void);

#endif

