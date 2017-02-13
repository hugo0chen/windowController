#include "initDrivers.h"
#include "motor.h"
#include "led.h"

// HSI = 16MHZ
// sysclk = F(HSI) / div
void init_clk(void)                
{
	CLK_DeInit();
	CLK_HSICmd(ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
	CLK_ClockSecuritySystemEnable();
}

void init_Test_Comman_Pin(void)
{
	GPIO_Init(GPIOA, GPIO_PIN_1 , GPIO_MODE_IN_PU_NO_IT);
}

//PE5--LED
void init_led(void)
{
	GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_1_GPIO_PIN ,
						GPIO_MODE_OUT_PP_LOW_FAST);
}

void Init_UART2(void)
{
	UART2_DeInit();
	UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, 
						UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, 
						UART2_MODE_TXRX_ENABLE);
	
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	UART2_Cmd(ENABLE);
}

void init_LM2596(void)
{
	GPIO_Init(LM2596_GPIO_PORT, (GPIO_Pin_TypeDef)LM2596_IN_1_PIN | 
						LM2596_IN_2_PIN |LM2596_ENA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}
// FALLING TRIG 
void init_PhotoSensor(void)
{	
	GPIO_Init(PHOTOSENSOR_GPIO_PORT, PHOTOSENSOR_GPIO_PIN , GPIO_MODE_IN_PU_IT);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
}
// FALLING TRIG 
void init_Remoter(void)
{	
	GPIO_Init(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 , GPIO_MODE_IN_PU_IT);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_ONLY);
}
//timer1 to record
void init_timer_1(void)
{
	TIM1_DeInit();
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1,ENABLE);
	// 8MHz /8000 = 10KHz  --->0.1ms
	TIM1_TimeBaseInit(8000,TIM1_COUNTERMODE_UP, 0xFFFE,0);
  TIM1_ARRPreloadConfig(ENABLE);
	TIM1_UpdateDisableConfig(DISABLE);
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);
	TIM1_Cmd(ENABLE);
}

// sys_time_tick  // 10ms
void init_timer_2(void)
{
	TIM2_TimeBaseInit(TIM2_PRESCALER_128, 0x0280);
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd(ENABLE);	
}


