/*============================================================================*/
/*                     AZOR - EMBEDDED SYSTEMS SOFTWARE                       */
/*============================================================================
* C Source:        periodic_tasks_exec.c
* version:         1.0 
* created_by:      MDRR
* date_created:    July 31 2013 */

/********************* INCLUSIONES ******************** */
#include <gsc_scheduler/periodic_tasks/periodic_tasks_exec.h>
#include <gsc_scheduler/periodic_tasks/periodic_tasks_modules.h>

/********************* DEFINICIONES ******************** */
#define	CANAL0	0U
T_UWORD ruw_MODO = 0u;
/**************************************************************
*  Name                 : periodic_tasks_exec_5tks
*	ID					 : TASK_5TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_5tks(void){

}

/**************************************************************
*  Name                 : periodic_tasks_exec_10tks
*	ID					 : TASK_10TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_10tks(void){
	//ADC_modoNormal(MODO, ADCinit_setOperationACTION( ADCinit_getADCvalue(ADC0, CANAL0)));
}

/**************************************************************
*  Name                 : periodic_tasks_exec_20tks
*	ID					 : TASK_20TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_20tks(void){
	ADC_modoNormal(ruw_MODO);
}

/**************************************************************
*  Name                 : periodic_tasks_exec_50tks
*	ID					 : TASK_50TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_50tks(void){

}

/**************************************************************
*  Name                 : periodic_tasks_exec_100tks
*	ID					 : TASK_100TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_100tks(void){
}

/**************************************************************
*  Name                 : periodic_tasks_exec_200tks
*	ID					 : TASK_200TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_200tks(void){
	ADC_modoSeguro(ruw_MODO);
}

/**************************************************************
*  Name                 : periodic_tasks_exec_500tks
*	ID					 : TASK_500TKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_500tks(void) {
}


/**************************************************************
*  Name                 : periodic_tasks_exec_1Mtks
*	ID					 : TASK_1MTKS
*  Description          : Container for functionality that is
					   executed periodically.
*  Parameters           : none
*  Return               : none
*  Critical/explanation : no
**************************************************************/
void periodic_tasks_exec_1Mtks(void){
	ruw_MODO=ADC_initModo();
}
