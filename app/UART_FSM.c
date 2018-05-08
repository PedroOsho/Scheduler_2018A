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
#define REDLED_GPIO REDLED_GPIO_
#define REDLED_PIN REDLED_PIN_

#define GREENLED_GPIO GREENLED_GPIO_
#define GREENLED_PIN GREENLED_PIN_

#define BLUELED_GPIO BLUELED_GPIO_
#define BLUELED_PIN BLUELED_PIN_

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
#define bit_limit 8
/*******************************************************************************
 * Variables
 ******************************************************************************/
enum State_enum {
    IDLE,
    SAVE,
    CODIFY,
    NONE,
};
T_UBYTE State = IDLE;
T_BIT Recived_Data[8];
T_UBYTE Bit_Position = 0;
T_BIT	Parity;
T_UBYTE Ciclo_Base_Rx = 1;
T_UBYTE Ciclo_Conteo_Rx = 1;

/*******************************************************************************
 * Code
 ******************************************************************************/
void IDLE_status()
{
	GPIO_SetPinsOutput(REDLED_GPIO, 1u << REDLED_PIN);
	GPIO_SetPinsOutput(GREENLED_GPIO, 1u << GREENLED_PIN);
	GPIO_SetPinsOutput(BLUELED_GPIO, 1u << BLUELED_PIN);
}
void SAVE_status()
{
	GPIO_SetPinsOutput(REDLED_GPIO, 1u << REDLED_PIN);
	GPIO_ClearPinsOutput(GREENLED_GPIO, 1u << GREENLED_PIN);
	GPIO_SetPinsOutput(BLUELED_GPIO, 1u << BLUELED_PIN);

}
void CODIFY_status()
{
	GPIO_SetPinsOutput(REDLED_GPIO, 1u << REDLED_PIN);
	GPIO_SetPinsOutput(GREENLED_GPIO, 1u << GREENLED_PIN);
	GPIO_ClearPinsOutput(BLUELED_GPIO, 1u << BLUELED_PIN);
}
void ERROR_status()
{
	GPIO_ClearPinsOutput(REDLED_GPIO, 1u << REDLED_PIN);
	GPIO_SetPinsOutput(GREENLED_GPIO, 1u << GREENLED_PIN);
	GPIO_SetPinsOutput(BLUELED_GPIO, 1u << BLUELED_PIN);
}



void IDLE_state()
{
	IDLE_status();
	if(Ciclo_Conteo_Rx >= Ciclo_Base_Rx)
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
		Ciclo_Conteo_Rx=1;
	}
	else
	{
		Ciclo_Conteo_Rx++;
	}
}

void SAVE_state()
{
	if(Ciclo_Conteo_Rx >= Ciclo_Base_Rx)
	{
		if(Bit_Position < bit_limit)
		{
			Recived_Data[Bit_Position] = GPIO_ReadPinInput(Rx1_GPIO, Rx1_PIN);
			State=SAVE;
			Bit_Position++;
			SAVE_status();
			PRINTF("%d \n",Recived_Data[Bit_Position-1]);
		}
		else
		{
			Parity = GPIO_ReadPinInput(Rx1_GPIO, Rx1_PIN);
			Bit_Position=0;
			PRINTF("%d \n",Parity);
			State= CODIFY;
			UART_StateMachine();
		}
		Ciclo_Conteo_Rx=1;
	}
	else
	{
		Ciclo_Conteo_Rx++;
	}

}

void CODIFY_state()
{
	if(Parity == TRUE)
	{
		CODIFY_status();
		Ciclo_Base_Rx=Ciclo_Base_Rx*2;
	}
	else
	{
	}
	State=IDLE;
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
		ERROR_status();
		break;

	}
}







