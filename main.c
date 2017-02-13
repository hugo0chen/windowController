/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "initdrivers.h"
#include "motor.h"
#include "stm8s_tim1.h"
#include "led.h"
#include "timer_2.h"
#include "string.h"
#include "schd.h"
#include "string.h"

#define PERIOD_TRY_TIME                  10    //unit: seconds
#define LED_RUNNING_PERIOD      				 1000
#define SPEED_FACTOR            				 1 
#define MAX_SPEED_RECORD_TIMES					 4
#define ACCESS_DELTA_LEGNTH              15
#define MAX_UART_BUF										 128
/* Private defines ---------------------------------------*/
struct DELTATIME{
	uint16_t deltaTime;
	uint8_t index;
};

enum COMMAND_TYPE{
	NO_COMMAND,
	OPEN_COMMAND,
	CLOSE_COMMAND,
	STOP_COMMAND,
	GET_TOTAL_LENGHT_COMMAND
};

struct WINDOW_PARA{
	uint32_t total_length;
	uint32_t current_position;
	enum MOTORSTATUS command_status;
	enum MOTORSTATUS current_status;
	enum MOTORSTATUS actual_motor_status;
};
/*-----------variables prototypes-------------------------*/

const char*constCommand[4]={"stop","clos","open","getl"};
enum COMMAND_TYPE cmd_type = NO_COMMAND;
struct WINDOW_PARA window;

bool enableUpdateFlag = FALSE;
uint8_t stopCommandFlag = 0;
uint8_t getLengthSuccFlag = 0;
uint32_t _trig_loops_count = 0;
uint32_t motor_rotate_message_time = 0;
uint8_t new_motor_status = 0;

uint8_t currentSpeed;
uint8_t speedRecord[MAX_SPEED_RECORD_TIMES]={0,0,0,0}; 
static uint16_t time_Record[4] = {0};

/* ----------function prototypes -------------------------*/
void init_drivers(void);
void init_window_reset(void);
void delay(uint32_t time);
uint8_t flash_led(void);
void excute_Command(void);
void processOpenCommand(void);
void processCloseCommand(void);
void processStopCommand(void);
bool process_Get_Total_Length_Command(void);
BitStatus seft_Test_Command(void);
enum MOTORSTATUS isStopOnSpeed(uint8_t *speed);
enum MOTORSTATUS getMotorDirection(uint16_t* time);
uint8_t stop_check(void);
void update_window_position_status(void);
void resetContext(void);
bool isUpdateWindowStatusEnabled(void);

void sendChar(uint8_t dat);
void sendString(uint8_t* data,uint8_t len);
void send_String(uint8_t*string);
void sendPositionInformation(uint16_t dataPosition);
void sendTotalLength(uint16_t length);
//--------------------------------------------------------
void printfMotorStatus(uint8_t status)
{
	send_String("\n\r");
	if(status == 1){
		send_String("STOP0");
	}else if(status == 2){
		send_String("FORWARD");
	}else if(status == 3){
		send_String("BACKWARD");
	}else{
		send_String("NULL");
	}

send_String("\n\r");
}

void continueDo(void)
{
	if(!stopCommandFlag && getLengthSuccFlag)
	{
		if(window.command_status == MOTOR_FORWARD)
		{
			if(window.current_position < (window.total_length - ACCESS_DELTA_LEGNTH))
			{						
				processOpenCommand();
			}
			else
			{					
				processStopCommand();	
			}
		}
		else if(window.command_status == MOTOR_BACKWARD)
		{
			if(window.current_position > ACCESS_DELTA_LEGNTH)
			{					
				processCloseCommand();
			}
			else
			{
				processStopCommand();	
			}
		}
		else
		{
			processStopCommand();	
		}
	}			
}

void main(void)
{	
	uint8_t secondsPeroidCount = 0;
	
	init_drivers();
	init_window_reset();

	if(seft_Test_Command() != RESET)
	{
		cmd_type = GET_TOTAL_LENGHT_COMMAND;
	}

	for(;;)
	{		
		if(flash_led())
		{
			secondsPeroidCount++;
			if(secondsPeroidCount >= PERIOD_TRY_TIME )
			{
				continueDo();
				secondsPeroidCount = 0;
			}
		}
		
		if(isUpdateWindowStatusEnabled())
		{
			if(new_motor_status&&enableUpdateFlag )
			{
				update_window_position_status();
				new_motor_status = 0;
			}	
		}
		
		if(stop_check())
		{
			secondsPeroidCount = 0;
		}
		excute_Command();
	}
}

bool isUpdateWindowStatusEnabled(void)
{
	return enableUpdateFlag;
}

void update_window_position_status(void)
{
	window.actual_motor_status = getMotorDirection(time_Record);
	
	if( window.command_status == MOTOR_FORWARD )
	{
		window.current_position++;
	}
	else if(( window.command_status == MOTOR_BACKWARD )&&( window.current_position > 6 ))
	{
		window.current_position--;
	}
	if(getLengthSuccFlag)
	{
		sendPositionInformation(window.current_position);//debug
	}
}
enum MOTORSTATUS  isStopOnSpeed(uint8_t *speed)
{	
	uint8_t tempSpeed;
	
	tempSpeed = speed[0];
	speed[0] = speed[1];
	speed[1] = speed[2];
	speed[2] = speed[3];
	if( ( tempSpeed < speed[0] )&&( speed[0] < speed[1])&&( speed[1] < speed[2]))
	{		
		return MOTOR_STOP;
	}
	return MOTOR_NULL;
}

void resetContext(void)
{
	enableUpdateFlag = FALSE;	
	window.current_status = MOTOR_STOP;
	currentSpeed = 0;
	speedRecord[0] = 0;
	speedRecord[1] = 0;
	speedRecord[2] = 0;
	speedRecord[3] = 0;
}

uint8_t stop_check(void)	
{	
	if( ( currentSpeed > 0  && currentSpeed < 100 )&& ( currentSpeed != speedRecord[3] ) )
	{
		speedRecord[3] = currentSpeed;
		if( isStopOnSpeed(speedRecord) == MOTOR_STOP )
		{						
			Fast_Stop();
			resetContext();	
			return 1;
		}
	}	
	return 0;		
}

void excute_Command()
{
	if(cmd_type == GET_TOTAL_LENGHT_COMMAND)
	{
		getLengthSuccFlag = 0;
		if(process_Get_Total_Length_Command())
		{
			cmd_type = NO_COMMAND;
			getLengthSuccFlag = 1;
			resetContext();	
			window.command_status = MOTOR_STOP;
		}
	}
	else
	{
		if(cmd_type == OPEN_COMMAND)
		{
			processOpenCommand();
		}
		else if(cmd_type == CLOSE_COMMAND)
		{
			processCloseCommand();
		}
		else	if(cmd_type == STOP_COMMAND)
		{
			processStopCommand();			
		}
		else
		{
			//TODO
		}
		cmd_type = NO_COMMAND;
	}
}

void init_drivers(void)
{
	init_clk(); 		  // 8MHZ sys_CLK
	init_LM2596();
	init_led();
	init_PhotoSensor();
	init_Remoter();
	init_timer_1();		// max 0.1ms*65536 = 65.536s.
	init_timer_2();   // systick 10ms step
	Init_UART2();
	enableInterrupts(); 
}

void init_window_reset(void)
{
	window.total_length = 0;
	window.current_position = 0;
	window.command_status = MOTOR_STOP;
	window.current_status = MOTOR_STOP;
	window.actual_motor_status = MOTOR_STOP;
}

uint8_t flash_led(void)
{
	static uint32_t runningTick	= 0;
  	
	if ( timeout( runningTick, LED_RUNNING_PERIOD ) )
	{ 
		led_reverse();
		runningTick = sys_TimeTick();	
		return 1;
	}
	return 0;
}

void processOpenCommand()
{
	Opening();
	enableUpdateFlag	= TRUE;
	window.command_status = MOTOR_FORWARD;
}

void processCloseCommand(void)
{	
	Closing();
	enableUpdateFlag	= TRUE;
	window.command_status = MOTOR_BACKWARD;
}

void processStopCommand(void)
{
	Fast_Stop();
	resetContext();
	window.command_status = MOTOR_STOP;
}
 
void sendTotalLength(uint16_t length)
{
	sendChar((uint8_t)(window.total_length>>8));
	sendChar((uint8_t)(window.total_length));
}
bool process_Get_Total_Length_Command(void)
{
	static uint8_t State;
	static stop_count;
	
	switch(State)
	{
		case 0:
			processOpenCommand();
			delay(1000);
			window.current_status = MOTOR_FORWARD;
			State = 1;
			break;
		case 1:
			if(window.current_status == MOTOR_STOP)
			{
					_trig_loops_count = 0;
					window.total_length = _trig_loops_count;				
					processCloseCommand();
					window.current_status = MOTOR_BACKWARD;
					delay(1000);
					State = 2;
			}
			break;
		case 2:
			if(window.current_status == MOTOR_STOP)
			{
					window.total_length = _trig_loops_count+8;
					sendTotalLength(window.total_length); //debug					
					window.current_position = 0;
					_trig_loops_count = 0;
					State = 0;
					return TRUE;
			}
			break;
		default:
			break;		
	}
	return FALSE;
}

void sendPositionInformation(uint16_t dataPosition)
{
	sendChar((uint8_t)(dataPosition>>8));
	sendChar((uint8_t)dataPosition);
}

INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
	static uint8_t trig_index;
	
	time_Record[trig_index++] = TIM1_GetCounter();
	if(trig_index>=4)
	{
		_trig_loops_count++;
		new_motor_status = 1;
		trig_index=0;
	}
}
/**
  * @brief External Interrupt PORTB Interrupt routine.
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
	uint8_t temp_Port_Data;

	temp_Port_Data = GPIO_ReadInputData(GPIOB);
	if(temp_Port_Data&0x0F)
	{
		switch(temp_Port_Data&0x0F)
		{
			case 0x0e:
				cmd_type = OPEN_COMMAND;
				break;
			case 0x0d:
				cmd_type = CLOSE_COMMAND;
				break;
			case 0x0b:
				cmd_type = STOP_COMMAND;
				break;
			case 0x07:
				cmd_type = GET_TOTAL_LENGHT_COMMAND;
				break;
			default:
				break;
		}
	}
}

enum MOTORSTATUS getMotorDirection(uint16_t* time)
{
	struct DELTATIME deltaT[3],tempDelta;
	
	deltaT[0].deltaTime = time[1] - time[0];
	deltaT[0].index = 0;
	deltaT[1].deltaTime = time[2] - time[1];
	deltaT[1].index = 1;
	deltaT[2].deltaTime = time[3] - time[2];
	deltaT[2].index = 2;
	
	if((deltaT[0].deltaTime == deltaT[1].deltaTime)||
		(deltaT[1].deltaTime == deltaT[2].deltaTime))
	{
		return MOTOR_NULL;
	}
	
	currentSpeed = SPEED_FACTOR*(deltaT[0].deltaTime+deltaT[1].deltaTime+deltaT[2].deltaTime);

	if(deltaT[0].deltaTime > deltaT[1].deltaTime) 
	{
		tempDelta.deltaTime = deltaT[1].deltaTime;
		tempDelta.index = deltaT[1].index;
		deltaT[1].deltaTime = deltaT[0].deltaTime;
		deltaT[1].index = deltaT[0].index;
		deltaT[0].deltaTime = tempDelta.deltaTime;
		deltaT[0].index = tempDelta.index;	
	}
	
	if(deltaT[1].deltaTime > deltaT[2].deltaTime)
	{
		tempDelta.deltaTime = deltaT[2].deltaTime;
		tempDelta.index = deltaT[2].index;
		deltaT[2].deltaTime = deltaT[1].deltaTime;
		deltaT[2].index = deltaT[1].index;
		deltaT[1].deltaTime = tempDelta.deltaTime;
		deltaT[1].index = tempDelta.index;
		if(deltaT[0].deltaTime > deltaT[1].deltaTime)
		{
			tempDelta.deltaTime = deltaT[1].deltaTime;
			tempDelta.index = deltaT[1].index;
			deltaT[1].deltaTime = deltaT[0].deltaTime;
			deltaT[1].index = deltaT[0].index;
			deltaT[0].deltaTime = tempDelta.deltaTime;
			deltaT[0].index = tempDelta.index;	
		}
	}

	if(((deltaT[0].index==0)&&(deltaT[1].index == 1))||
		((deltaT[0].index==2)&&(deltaT[1].index == 0))||
		((deltaT[0].index==1)&&(deltaT[1].index == 2)))
	{
		return MOTOR_FORWARD;
	}
	else
		return MOTOR_BACKWARD;
}

BitStatus seft_Test_Command(void)
{
	return (BitStatus)GPIO_ReadInputPin(GPIOA,GPIO_PIN_1);	
}

void delay(uint32_t time)
{
	uint32_t i,j;
	for(j=0;j<time;j++)
	{
		for(i=0;i<100;i++)
		{	}
	}
}

void sendChar(uint8_t dat)
{
	while(( UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET));
	UART2_SendData8(dat);
}

void sendString(uint8_t* data,uint8_t len)
{
	uint8_t i;
	
	for( i = 0;i < len; i ++ )
	{		
		sendChar(data[i]);
	}
}

void send_String(uint8_t*string)
{
	uint8_t*p = string;
	while(*p)
	{
		sendChar(*p++);
	}
}

/*
  * @brief UART2 RX interrupt routine.
*/
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
	  static uint8_t usart_rx_data[MAX_UART_BUF];
	  static uint8_t usart_rx_index;

		usart_rx_data[usart_rx_index++] = UART2_ReceiveData8();
		if(usart_rx_index >= 4)
		{
			usart_rx_data[usart_rx_index] = '\0';
			if(strstr(usart_rx_data,constCommand[0])!=NULL)
			{
				cmd_type = STOP_COMMAND;
			}			
			else if(strstr(usart_rx_data,constCommand[1])!=NULL)
			{
				cmd_type = CLOSE_COMMAND;
			}			
			else if(strstr(usart_rx_data,constCommand[2])!=NULL)
			{
				cmd_type = OPEN_COMMAND;
			}
			else if(strstr(usart_rx_data,constCommand[3])!=NULL){
				cmd_type = GET_TOTAL_LENGHT_COMMAND;
			}		
			memset(usart_rx_data,0,usart_rx_index);
			usart_rx_index = 0;		
		}
		UART2_ClearITPendingBit(UART2_IT_RXNE);
 }

#ifdef USE_FULL_ASSERT

void assert_failed(u8* file, u32 line)
{ 
  while (1)
  {
  }
}
#endif

