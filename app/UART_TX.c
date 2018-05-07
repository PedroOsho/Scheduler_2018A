/*
 * UART_TX.c
 *
 *  Created on: May 6, 2018
 *      Author: PEDRO
 */
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

#define DataByte {0,0,1,1,0,0,1,1}
#define DataParity {0,0,0,0,0,0,0,0,0,0}

#define SENDING 1
#define WAITING 0
/*******************************************************************************
 * Variables
 ******************************************************************************/
T_BIT	Send_Data[8] = DataByte;
T_BIT	ParityTx[10] = DataParity;
T_BIT	state = 0;
T_UBYTE Bit_PositionTx = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
void sendMesaggeTx()
{
	if(state == WAITING)
	{
		GPIO_ClearPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
		state=SENDING;
	}
	else
	{
	if(Bit_PositionTx < 8)
	{
		if(Send_Data[Bit_PositionTx] == TRUE)
		{
			GPIO_SetPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
		}
		else
		{
			GPIO_ClearPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
		}
		Bit_PositionTx++;
	}
	else
	{
		if(ParityTx[0] == TRUE)
		{
			GPIO_SetPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
		}
		else
		{
			GPIO_ClearPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
		}
		state=WAITING;
		Bit_PositionTx=0;
	}
	}
}
