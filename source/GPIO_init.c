/*
 * GPIO_init.c
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

#define REDLED_GPIO REDLED_GPIO_
#define REDLED_PIN REDLED_PIN_

#define GREENLED_GPIO GREENLED_GPIO_
#define GREENLED_PIN GREENLED_PIN_

#define BLUELED_GPIO BLUELED_GPIO_
#define BLUELED_PIN BLUELED_PIN_

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void init_GPIO_UART_in()
{
    gpio_pin_config_t int_UART = {
    	kGPIO_DigitalInput, 0,
        };
    GPIO_PinInit( Rx1_GPIO, Rx1_PIN, &int_UART);
    GPIO_PinInit( Rx2_GPIO, Rx2_PIN, &int_UART);
}

void init_GPIO_UART_out()
{
    gpio_pin_config_t out_UART = {
    	kGPIO_DigitalOutput, 0,
        };
    GPIO_PinInit( Tx1_GPIO, Tx1_PIN, &out_UART);
    GPIO_PinInit( Tx2_GPIO, Tx2_PIN, &out_UART);
}

void init_GPIO_LED_STATUS()
{
    gpio_pin_config_t LED_pin = {
    	kGPIO_DigitalOutput, 0,
        };
    GPIO_PinInit( REDLED_GPIO, REDLED_PIN, &LED_pin);
    GPIO_PinInit( GREENLED_GPIO, GREENLED_PIN, &LED_pin);
    GPIO_PinInit( BLUELED_GPIO, BLUELED_PIN, &LED_pin);
}
