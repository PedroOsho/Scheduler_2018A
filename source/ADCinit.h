/*
 * ADC_init.h
 *
 *  Created on: Apr 10, 2018
 *      Author: Manuel
 */

#ifndef ADCINIT_H_
#define ADCINIT_H_

extern T_UWORD ADCinit_getADCvalue(ADC_Type *base, uint32_t channelGroup);
extern void ADCinit_ADCconfig(void);
extern T_UBYTE ADCinit_setOperationMODE(T_UWORD luw_ADC_VALUE);
extern T_UBYTE ADCinit_setOperationACTION(T_UWORD luw_ADC_VALUE);
extern void ADC_modoNormal(T_UBYTE lub_Modo);
extern void ADC_modoSeguro(T_UBYTE lub_Modo);
extern T_UBYTE ADC_initModo();
#endif /* ADCINIT_H_ */
