/*
 * PWMinit.c

 *
 *  Created on: Apr 12, 2018
 *      Author: Manuel
 */
/********************* INCLUSIONES ******************** */
#include "drivers_HAL/fsl_tpm.h"
#include "source/stdtypedef.h"
#include "pin_mux.h"
#include "PWMinit.h"
#include "board.h"

/********************* DEFINICIONES ******************** */
/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_DIRECCIONBASE 		TPM2
#define TPM_NUMEROdeCANAL0 		0u
#define TPM_NUMEROdeCANAL1		1u
#define NOPOWER			0u
#define HALFofPOWER		50u
#define	FULLPOWER		100u
/* Get source clock for TPM driver */
#define TPM_CLOCKPRINCIPAL 		CLOCK_GetFreq(kCLOCK_PllFllSelClk)

/********************* VARIABLES ******************** */
volatile T_UBYTE rub_updatedDutycycle = 0;
volatile T_UBYTE rub_getCharValue = 0U;
T_BOOLEAN rbi_subiendo = TRUE;

/********************* CÓDIGO ******************** */
/*!
 * @brief PWMinit_PWMconfig configures PWM channel on default values
 */
void PWMinit_PWMconfig(void){
    tpm_config_t TPM_info;
    tpm_chnl_pwm_signal_param_t TPM_PARAMETRO[2];

    /* Configure tpm params with frequency 24kHZ */
    TPM_PARAMETRO[0].chnlNumber = (tpm_chnl_t)TPM_NUMEROdeCANAL0;
    TPM_PARAMETRO[0].level = kTPM_HighTrue;
    TPM_PARAMETRO[0].dutyCyclePercent = rub_updatedDutycycle;

    TPM_PARAMETRO[1].chnlNumber = (tpm_chnl_t)TPM_NUMEROdeCANAL1;
	TPM_PARAMETRO[1].level = kTPM_HighTrue;
	TPM_PARAMETRO[1].dutyCyclePercent = rub_updatedDutycycle;

    CLOCK_SetTpmClock(1u);				/* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    TPM_GetDefaultConfig(&TPM_info);

    /* Initialize TPM module */
    TPM_Init(TPM_DIRECCIONBASE, &TPM_info);

    TPM_SetupPwm(TPM_DIRECCIONBASE, TPM_PARAMETRO, 2U, kTPM_EdgeAlignedPwm, 24000U, TPM_CLOCKPRINCIPAL);
    TPM_StartTimer(TPM_DIRECCIONBASE, kTPM_SystemClock);
}

/*!
 * @brief PWMinit_PWMset sets a value onto a PWM channel
 */
void PWMinit_PWMtestRoutine(void){
	if(rbi_subiendo)
		rub_updatedDutycycle += 1;
	else
		rub_updatedDutycycle -= 1;

	if(rub_updatedDutycycle == 100)
		rbi_subiendo = false;
	else if(rub_updatedDutycycle == 0)
		rbi_subiendo = true;
	else;

	TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL1, kTPM_CenterAlignedPwm,
	                           rub_updatedDutycycle);
}

void PWMinit_PWMnormalDecrease(){
    TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL0, kTPM_CenterAlignedPwm,
    						HALFofPOWER);
}

void PWMinit_PWMhighDecrease(){
    TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL0, kTPM_CenterAlignedPwm,
    						FULLPOWER);
}

void PWMinit_PWMnormalIncrease(){
    TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL1, kTPM_CenterAlignedPwm,
    						HALFofPOWER);
}

void PWMinit_PWMhighIncrease(){
    TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL1, kTPM_CenterAlignedPwm,
    						FULLPOWER);
}

void PWMinit_PWMnoIncrease(){
    TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL1, kTPM_CenterAlignedPwm,
    						NOPOWER);
}
void PWMinit_PWMnoDecrease(){
    TPM_UpdatePwmDutycycle(TPM_DIRECCIONBASE, (tpm_chnl_t)TPM_NUMEROdeCANAL0, kTPM_CenterAlignedPwm,
    						NOPOWER);
}
