#include "motor.h"
 
 static void delay(uint16_t delay_time)
 {
	 uint16_t i,j;
	 for(i=delay_time;i>0;i--)
	 {
		 for(j=10;j>0;j--)
		 {
		 }
	 }
 }
// L298N --//Negative
// ENA=0; 
// IN1=1;
// IN2=0; 
// ENA=1;
void Closing(void)
{
  GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_ENA_PIN);      
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_IN_1_PIN); 
  GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_IN_2_PIN);   
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_ENA_PIN);
	delay(100);
}
//l298N --//Positive
//ENA=0;  
//IN1=0; 
//IN2=1;
//ENA=1; 
void Opening(void)
{
  GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_ENA_PIN);
  GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_IN_1_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_IN_2_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_ENA_PIN);
	delay(100);
}
//L298N---//Free Running Motor Stop  
//ENA=0;  
//N1=1; 
//IN2=1;   
//ENA=0;  
void Free_Stop(void)
{
	GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_ENA_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_IN_1_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_IN_2_PIN);
  GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_ENA_PIN);
	delay(100);

}
//L298N---//Fast Motor Stop  
//ENA=0;  
//N1=1; 
//IN2=1;   
//ENA=1;  
void Fast_Stop(void)
{
	GPIO_WriteLow(LM2596_GPIO_PORT, LM2596_ENA_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_IN_1_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_IN_2_PIN);
  GPIO_WriteHigh(LM2596_GPIO_PORT, LM2596_ENA_PIN);
	delay(100);
}