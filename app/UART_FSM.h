/*
 * UART_FSM.h
 *
 *  Created on: May 6, 2018
 *      Author: PEDRO
 */

#ifndef UART_FSM_H_
#define UART_FSM_H_

void UART_StateMachine(void);
void IDLE_state(void);
void SAVE_state(void);
void CODIFY_state(void);
void IDLE_status(void);
void SAVE_status(void);
void CODIFY_status(void);
void ERROR_status(void);

#endif /* UART_FSM_H_ */
