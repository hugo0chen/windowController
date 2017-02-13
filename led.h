#ifndef _LED_H
#define _LED_H
#include "stm8s.h"

#define LED_GPIO_PORT    GPIOF
#define LED_1_GPIO_PIN   GPIO_PIN_4

void led_On(void);
void led_off(void);
void led_reverse(void);
#endif

