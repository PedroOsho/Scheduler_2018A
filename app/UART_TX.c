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
#define DataParity {1,1,1,0,0,0,1,0,0,1}

#define SENDING 1
#define WAITING 0

#define INIT_ARRAY 0
#define END_DATA_ARRAY 8
#define END_PARITY_ARRAY 10
/*******************************************************************************
 * Variables
 ******************************************************************************/
T_BIT	Send_Data[8] = DataByte;
T_BIT	ParityTx[10] = DataParity;
T_BIT	state = 0;
T_UBYTE Bit_PositionTx = 0;
T_UBYTE Parity_PositionTx = 0;
T_UBYTE Ciclo_Base = 1;
T_UBYTE Ciclo_Conteo = 1;
/*******************************************************************************
 * Code
 ******************************************************************************/
void sendMesaggeTx()
{
	if(Ciclo_Conteo >= Ciclo_Base)
	{
		if(state == WAITING)
		{
			GPIO_ClearPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
			state=SENDING;
		}
		else
		{
			if(Bit_PositionTx < END_DATA_ARRAY)
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
				if(Parity_PositionTx<END_PARITY_ARRAY)
				{
					if(ParityTx[Parity_PositionTx] == TRUE)
					{
						GPIO_SetPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
						Ciclo_Base=Ciclo_Base*2;
					}
					else
					{
						GPIO_ClearPinsOutput(Tx1_GPIO, 1u << Tx1_PIN);
					}
					Parity_PositionTx++;
				}
				else
				{
					Parity_PositionTx=INIT_ARRAY;
				}
				state=WAITING;
				Bit_PositionTx=INIT_ARRAY;
			}
		}
		Ciclo_Conteo=1;
	}
	else
	{
		Ciclo_Conteo++;
	}
}

void sincrinizacion()
{
	T_UBYTE contador=0;
	for(contador=0; contador>800 ;contador++)
	{
	}

}
