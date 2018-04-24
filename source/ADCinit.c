/*
 * ADC_init.c
 *
 *  Created on: Apr 10, 2018
 *      Author: Manuel Lara
 */
/********************* INCLUSIONES ******************** */
#include "drivers_HAL/fsl_adc16.h"
#include "fsl_debug_console.h"
#include "source/stdtypedef.h"
#include "source/ADCinit.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "PWMinit.h"
#include "board.h"

/********************* DEFINICIONES ******************** */
#define ADC_BASE 						ADC0
#define ADC_CANAL						0U
#define ADC_CANALUSUARIO 				0U 				/*PTE20, ADC0_SE0 */
#define	CANAL0							0U

// Definiciones especificas del selector de modo
#define TOLERANCIAincrementoNORMAL				10		// +10°
#define TOLERANCIAincrementoRAPIDO				15		// +20°
#define TOLERANCIAdecrementoNORMAL				10		// -5°
#define TOLERANCIAdecrementoRAPIDO				15		// -10°
#define TOLERANCIAincrementoNORMALhisteresis	8		// +10°
#define TOLERANCIAincrementoRAPIDOhisteresis	13		// +20°
#define TOLERANCIAdecrementoNORMALhisteresis	8		// -5°
#define TOLERANCIAdecrementoRAPIDOhisteresis	13		// -10°
#define VALORDETEMPERATURAbase			27		// A 30°
#define TOPEINCREMENTOnormal		VALORDETEMPERATURAbase + TOLERANCIAincrementoNORMAL
#define TOPEINCREMENTOrapido		VALORDETEMPERATURAbase + TOLERANCIAincrementoRAPIDO
#define TOPEDECREMENTOnormal		VALORDETEMPERATURAbase - TOLERANCIAdecrementoNORMAL
#define TOPEDECREMENTOrapido		VALORDETEMPERATURAbase - TOLERANCIAdecrementoRAPIDO
#define TOPEINCREMENTOnormalhisteresis		VALORDETEMPERATURAbase + TOLERANCIAincrementoNORMALhisteresis
#define TOPEINCREMENTOrapidohisteresis		VALORDETEMPERATURAbase + TOLERANCIAincrementoRAPIDOhisteresis
#define TOPEDECREMENTOnormalhisteresis		VALORDETEMPERATURAbase - TOLERANCIAdecrementoNORMALhisteresis
#define TOPEDECREMENTOrapidohisteresis		VALORDETEMPERATURAbase - TOLERANCIAdecrementoRAPIDOhisteresis
/********************* DECLARACIONES ******************** */
T_UBYTE rub_Contador = 0U;
T_UBYTE rub_LastAction=1;
extern T_UWORD ruw_MODO;
adc16_config_t ADC_EstructuraConfiguracion;
adc16_channel_config_t ADC_EstructuraConfiguraciondelCanal;

/*enumeracion con los modos y acciones */
enum ru_ModoDeOperacion{
	MODO_NORMAL,
	NO_INCDEC,
	INCREMENTO_NORMAL,
	INCREMENTO_RAPIDO,
	DECREMENTO_NORMAL,
	DECREMENTO_RAPIDO,
	MODO_SEGURO
};
/********************* FUNCIONES/CÓDIGO ******************** */
/*!
 * @brief Main function
 */
/* Funcion de configuracion inicial del ADC**
 * configuracion de :
 * Voltaje de referencia
 * Fuente de Reloj
 * Reloj sincrono/asincrono
 * Divisor de reloj
 * resolucion
 * Velocidad
 * Referencia de Voltaje
 */
void ADCinit_ADCconfig(void){
	ADC_EstructuraConfiguracion.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
    ADC_EstructuraConfiguracion.clockSource = kADC16_ClockSourceAsynchronousClock;
    ADC_EstructuraConfiguracion.enableAsynchronousClock = true;
    ADC_EstructuraConfiguracion.clockDivider = kADC16_ClockDivider8;
    ADC_EstructuraConfiguracion.resolution = kADC16_Resolution12or13Bit;
    ADC_EstructuraConfiguracion.longSampleMode = kADC16_LongSampleDisabled;
    ADC_EstructuraConfiguracion.enableHighSpeed = false;
    ADC_EstructuraConfiguracion.enableLowPower = false;
    ADC_EstructuraConfiguracion.enableContinuousConversion = false;
	ADC_EstructuraConfiguracion.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;

	ADC16_Init(ADC_BASE, &ADC_EstructuraConfiguracion);
	ADC16_EnableHardwareTrigger(ADC_BASE, false); 				/* Evitar muestreo por hardware */

	ADC_EstructuraConfiguraciondelCanal.channelNumber = ADC_CANALUSUARIO;
	ADC_EstructuraConfiguraciondelCanal.enableInterruptOnConversionCompleted = TRUE; /* Enable the interrupt. */
	ADC_EstructuraConfiguraciondelCanal.enableDifferentialConversion = FALSE;
}

/*!
 * @brief function int getADCValue
 * @param base: ADC number, channelGroup: ADC Channel to be read
 * @return: unsigned 32 bits int
 */
T_UWORD ADCinit_getADCvalue(ADC_Type *base, uint32_t channelGroup){
	float rub_ValorAdc;
	ADC16_SetChannelConfig(ADC_BASE, ADC_CANAL, &ADC_EstructuraConfiguraciondelCanal);
	rub_ValorAdc=base->R[channelGroup];
	rub_ValorAdc= (3.3/4095)*rub_ValorAdc*100;
	return rub_ValorAdc;
}

/*!
 * @brief function Select operation action
 * @param base: Operation Mode
 * @return: void
 */
void ADC_modoNormal(T_UBYTE lub_Modo){
	T_UBYTE lub_Action;
	if(lub_Modo == MODO_NORMAL){
	lub_Action=ADCinit_setOperationACTION(ADCinit_getADCvalue(ADC0, CANAL0));
	if(lub_Action == NO_INCDEC)
	{
		T_UWORD luw_ADC_Val = ADCinit_getADCvalue(ADC0, CANAL0);
		PRINTF("modo normal %d\n", luw_ADC_Val);
		PWMinit_PWMnoIncrease();
		PWMinit_PWMnoDecrease();
	}
	else if(lub_Action == INCREMENTO_NORMAL)
	{
		T_UWORD luw_ADC_Val = ADCinit_getADCvalue(ADC0, CANAL0);
		PRINTF("modo incremento normal %d\n", luw_ADC_Val);
		PWMinit_PWMnormalIncrease();
		PWMinit_PWMnoDecrease();
	}
	else if(lub_Action == DECREMENTO_NORMAL)
	{
		T_UWORD luw_ADC_Val = ADCinit_getADCvalue(ADC0, CANAL0);
		PRINTF("modo decremento normal %d\n", luw_ADC_Val);
		PWMinit_PWMnoIncrease();
		PWMinit_PWMnormalDecrease();
	}
	else if(lub_Action == DECREMENTO_RAPIDO)
	{
		T_UWORD luw_ADC_Val = ADCinit_getADCvalue(ADC0, CANAL0);
		PRINTF("modo decremento rapido %d\n", luw_ADC_Val);
		PWMinit_PWMnoIncrease();
		PWMinit_PWMhighDecrease();
	}
	else if(lub_Action == INCREMENTO_RAPIDO)
	{
		T_UWORD luw_ADC_Val = ADCinit_getADCvalue(ADC0, CANAL0);
		PRINTF("modo incremento rapido %d\n", luw_ADC_Val);
		PWMinit_PWMhighIncrease();
		PWMinit_PWMnoDecrease();
	}
	else{}
}
else{}
}

/*!
 * @brief function Select operation Mode
 * @param base: void
 * @return: Unsigned 32 bits int  Modo
 */
T_UBYTE ADC_initModo()
{
	T_UBYTE lub_Action;
	lub_Action=ADCinit_setOperationACTION(ADCinit_getADCvalue(ADC0, CANAL0));
	PRINTF("1 seg");

if(lub_Action == INCREMENTO_RAPIDO || lub_Action == DECREMENTO_RAPIDO)
	{
	rub_Contador++;
	if(rub_Contador>=30U)
		{
		PRINTF("30 seg");
		rub_Contador=30;
		return MODO_SEGURO;
		}
	else
	{return MODO_NORMAL;}
	}
else
	{
	rub_Contador=0;
	return MODO_NORMAL;
	}
}
/*!
 * @brief function Actions in Safe Mode
 * @param base: Modo
 * @return: void
 */

void ADC_modoSeguro(T_UBYTE lub_Modo){
	T_UBYTE lub_Action;
	if(lub_Modo == MODO_SEGURO)
	{
		lub_Action=ADCinit_setOperationACTION(ADCinit_getADCvalue(ADC0, CANAL0));
		if(lub_Action == INCREMENTO_RAPIDO || lub_Action == DECREMENTO_RAPIDO)
			{
		T_UWORD ADC_Valor = ADCinit_getADCvalue(ADC0, CANAL0);
		PRINTF("modo Seguro %d\n", ADC_Valor);
		PWMinit_PWMhighIncrease();
		PWMinit_PWMhighDecrease();
			}
		else{
			ruw_MODO=ADC_initModo();
		}
	}

	else
	{
	}
}

/*!
 * @brief function Definition of a Action operation
 * @param base: ADC Value
 * @return: Unsigned 32 bits int  Action operation
 */
T_UBYTE ADCinit_setOperationACTION(T_UWORD luw_ADC_VALUE){
//normal
	if(rub_LastAction == NO_INCDEC)
	{
		if( (luw_ADC_VALUE < TOPEINCREMENTOnormal) && (luw_ADC_VALUE > TOPEDECREMENTOnormal ))
		{
			rub_LastAction=NO_INCDEC;
			return NO_INCDEC;
		}
	}
	else
	{
		if( (luw_ADC_VALUE < TOPEINCREMENTOnormalhisteresis) && (luw_ADC_VALUE > TOPEDECREMENTOnormalhisteresis ))
		{
			rub_LastAction=NO_INCDEC;
			return NO_INCDEC;
		}
	}
//Incremento normal
	if(rub_LastAction == INCREMENTO_NORMAL)
	{
		if( (luw_ADC_VALUE > TOPEINCREMENTOnormalhisteresis) && (luw_ADC_VALUE < TOPEINCREMENTOrapido) )
		{
			rub_LastAction=INCREMENTO_NORMAL;
			return INCREMENTO_NORMAL;
		}
	}
	else if(rub_LastAction == INCREMENTO_RAPIDO)
	{
		if( (luw_ADC_VALUE > TOPEINCREMENTOnormalhisteresis) && (luw_ADC_VALUE < TOPEINCREMENTOrapidohisteresis) )
		{
			rub_LastAction=INCREMENTO_NORMAL;
			return INCREMENTO_NORMAL;
		}
	}
	else
	{
		if( (luw_ADC_VALUE > TOPEINCREMENTOnormal) && (luw_ADC_VALUE < TOPEINCREMENTOrapido) )
		{
			rub_LastAction=INCREMENTO_NORMAL;
			return INCREMENTO_NORMAL;
		}
	}
//Incremento rapido
	if(rub_LastAction == INCREMENTO_RAPIDO)
	{
		if(luw_ADC_VALUE > TOPEINCREMENTOrapidohisteresis)
		{
			rub_LastAction=INCREMENTO_RAPIDO;
			return INCREMENTO_RAPIDO;
		}
	}
	else
	{
		if(luw_ADC_VALUE > TOPEINCREMENTOrapido)
		{
			rub_LastAction=INCREMENTO_RAPIDO;
			return INCREMENTO_RAPIDO;
		}
	}
//Incremento Normal
	if(rub_LastAction == DECREMENTO_NORMAL)
	{
		 if( (luw_ADC_VALUE < TOPEDECREMENTOnormalhisteresis) && (luw_ADC_VALUE > TOPEDECREMENTOrapido) )
		 {
			 rub_LastAction=DECREMENTO_NORMAL;
			 return DECREMENTO_NORMAL;
		 }
	}
	else if(rub_LastAction == DECREMENTO_RAPIDO)
	{
		if( (luw_ADC_VALUE < TOPEDECREMENTOnormalhisteresis) && (luw_ADC_VALUE > TOPEDECREMENTOrapidohisteresis) )
		{
			rub_LastAction=DECREMENTO_NORMAL;
			return DECREMENTO_NORMAL;
		}
	}
	else
	{
		if( (luw_ADC_VALUE < TOPEDECREMENTOnormal) && (luw_ADC_VALUE > TOPEDECREMENTOrapido) )
		{
			rub_LastAction=DECREMENTO_NORMAL;
			return DECREMENTO_NORMAL;
		}
	}
//Incremento Rapido
	if(rub_LastAction == DECREMENTO_RAPIDO)
	{
		if ( luw_ADC_VALUE < TOPEDECREMENTOrapidohisteresis)
		{
			rub_LastAction=DECREMENTO_RAPIDO;
			return DECREMENTO_RAPIDO;
		}
	}
	else
	{
		if ( luw_ADC_VALUE < TOPEDECREMENTOrapido)
		{
			rub_LastAction=DECREMENTO_RAPIDO;
			return DECREMENTO_RAPIDO;
		}
	}
}

