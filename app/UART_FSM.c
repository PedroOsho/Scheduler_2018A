/*
 * UART_FSM.c
 *
 *  Created on: May 6, 2018
 *      Author: PEDRO
 */

/******************************************************************************
 *   INCLUDES
 *****************************************************************************/
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "drivers_HAL/fsl_gpio.h"
#include "pin_mux.h"
#include "board.h"
#include "GPIO_init.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define Rx1_GPIO Rx1_GPIO_
#define Rx1_PIN Rx1_PIN_

#define Rx2_GPIO Rx2_GPIO_
#define Rx2_PIN Rx2_PIN_

#define Tx1_GPIO Tx1_GPIO_
#define Tx1_PIN Tx1_PIN_

#define Tx2_GPIO Tx2_GPIO_
#define Tx2_PIN Tx2_PIN_

#define T_UBYTE uint8_t
#define T_BIT bool

#define FALSE 0
#define TRUE 1
/*******************************************************************************
 * Variables
 ******************************************************************************/
enum State_enum {
    IDLE,
    SAVE,
    CODIFY,
    NONE,
};
T_UBYTE bit_limit =8;
T_UBYTE State = IDLE;
T_BIT Recived_Data[8];
T_UBYTE Bit_Position = 0;
T_BIT	Parity;
/*******************************************************************************
 * Code
 ******************************************************************************/
void IDLE_state()
{
	if(GPIO_ReadPinInput(Rx1_GPIO, Rx1_PIN)==FALSE)
	{
		State=SAVE;
	}
	else
	{
		State=IDLE;
	}
	PRINTF("el estado es: %d \n",State);
}

void SAVE_state()
{
	if(Bit_Position < bit_limit)
	{
		Recived_Data[Bit_Position] = GPIO_ReadPinInput(Rx1_GPIO, Rx1_PIN);
		State=SAVE;
		Bit_Position++;
		PRINTF("%d \n",Recived_Data[Bit_Position-1]);
	}

	else
	{
		Parity = GPIO_ReadPinInput(Rx1_GPIO, Rx1_PIN);
		Bit_Position=0;
		PRINTF("%d \n",Parity);
		State= IDLE;
		//UART_StateMachine();
	}

}

void CODIFY_state()
{
	if(Parity == TRUE)
	{

	}
	else
	{

	}
}


void UART_StateMachine()
{
	switch(State)
	{
	case IDLE:
		IDLE_state();
		break;

	case SAVE:
		SAVE_state();
		break;

	case CODIFY:
		CODIFY_state();
		break;

	default:
		break;

	}
}







