/*
 * delay.c
 *
 *  Created on: Apr 10, 2018
 *      Author: Manuel Lara
 */
#include "source/stdtypedef.h"
#include "delay.h"

void DELAY_delay(volatile uint32_t lul_DelayTime){
	volatile uint32_t n = 0;
	lul_DelayTime *= 3690;

    for (n = 0; n < lul_DelayTime; n++){
        __asm("NOP"); 				/* delay */
    }
}
