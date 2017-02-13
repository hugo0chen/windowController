#include "led.h"

void led_On(void)
{
	GPIO_WriteLow(LED_GPIO_PORT,LED_1_GPIO_PIN);
	
}
void led_off(void){
	
	GPIO_WriteHigh(LED_GPIO_PORT,LED_1_GPIO_PIN);
}

void led_reverse(void)
{	
	GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_1_GPIO_PIN);
}