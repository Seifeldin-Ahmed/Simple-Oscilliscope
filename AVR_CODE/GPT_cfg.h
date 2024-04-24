/*
 * GPT_cfg.h
 *
 * Created: 04/11/2022 10:01:47 AM
 *  Author: Dell
 */ 

#include "../../LIB/STD_Types.h"

#ifndef GPT_CFG_H_
#define GPT_CFG_H_

/*************************************************** TIMER 0 ***************************************************/

/* compare output pin (OC0) configuration
 * 	options: GPT_OC0_NORMAL_PORT_CONNECTION
 * 			 GPT_OC0_TOGGLE_ON_COMPARE_MATCH
 * 			 GPT_OC0_CLEAR_ON_COMPARE_MATCH
 * 			 GPT_OC0_SET_ON_COMPARE_MATCH
 */
#define GPT_OC0_COMPARE_MATCH_MODE	 GPT_OC0_NORMAL_PORT_CONNECTION


/* timer0 output compare match interrupt status configuration
 * 	options: GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE
 * 			 GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE
 */
#define GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_STATUS	 GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE


/* timer0 overflow interrupt status configuration
 * 	options: GPT_TIMER0_OVERFLOW_INTERRUPT_ENABLE
 * 			 GPT_TIMER0_OVERFLOW_INTERRUPT_DISABLE
 */
#define GPT_TIMER0_OVERFLOW_INTERRUPT_STATUS	 GPT_TIMER0_OVERFLOW_INTERRUPT_ENABLE
//****************************************************************************************************************





/*************************************************** TIMER 1 ***************************************************/
/* compare output pin (OC1A) configuration
 * 	options: GPT_OC1A_NORMAL_PORT_CONNECTION
 * 			 GPT_OC1A_TOGGLE_ON_COMPARE_MATCH
 * 			 GPT_OC1A_CLEAR_ON_COMPARE_MATCH
 * 			 GPT_OC1A_SET_ON_COMPARE_MATCH
 */
#define GPT_OC1A_COMPARE_MATCH_MODE	 GPT_OC1A_NORMAL_PORT_CONNECTION


/* compare output pin (OC1B) configuration
 * 	options: GPT_OC1B_NORMAL_PORT_CONNECTION
 * 			 GPT_OC1B_TOGGLE_ON_COMPARE_MATCH
 * 			 GPT_OC1B_CLEAR_ON_COMPARE_MATCH
 * 			 GPT_OC1B_SET_ON_COMPARE_MATCH
 */
#define GPT_OC1B_COMPARE_MATCH_MODE	 GPT_OC1B_NORMAL_PORT_CONNECTION


/* timer1A output compare match interrupt status configuration
 * 	options: GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE
 * 			 GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE
 */
#define GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_STATUS	 GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE


/* timer1B output compare match interrupt status configuration
 * 	options: GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE
 * 			 GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE
 */
#define GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_STATUS	 GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE


/* timer1 overflow interrupt status configuration
 * 	options: GPT_TIMER1_OVERFLOW_INTERRUPT_ENABLE
 * 			 GPT_TIMER1_OVERFLOW_INTERRUPT_DISABLE
 */
#define GPT_TIMER1_OVERFLOW_INTERRUPT_STATUS	 GPT_TIMER1_OVERFLOW_INTERRUPT_ENABLE
/****************************************************************************************************************/





/*************************************************** TIMER 2 ***************************************************/

/* compare output pin (OC2) configuration
 * 	options: GPT_OC2_NORMAL_PORT_CONNECTION
 * 			 GPT_OC2_TOGGLE_ON_COMPARE_MATCH
 * 			 GPT_OC2_CLEAR_ON_COMPARE_MATCH
 * 			 GPT_OC2_SET_ON_COMPARE_MATCH
 */
#define GPT_OC2_COMPARE_MATCH_MODE	 GPT_OC2_NORMAL_PORT_CONNECTION


/* timer0 output compare match interrupt status configuration
 * 	options: GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE
 * 			 GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE
 */
#define GPT_TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_STATUS	 GPT_TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE


/* timer2 overflow interrupt status configuration
 * 	options: GPT_TIMER2_OVERFLOW_INTERRUPT_ENABLE
 * 			 GPT_TIMER2_OVERFLOW_INTERRUPT_DISABLE
 */
#define GPT_TIMER2_OVERFLOW_INTERRUPT_STATUS	 GPT_TIMER2_OVERFLOW_INTERRUPT_ENABLE
/****************************************************************************************************************/






/****************************************** Options *********************************************/



//****************************** Timer 0 *********************************

/* compare output (OC0) mode options */
#define GPT_OC0_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC0_TOGGLE_ON_COMPARE_MATCH		0X10
#define GPT_OC0_CLEAR_ON_COMPARE_MATCH		0X20
#define GPT_OC0_SET_ON_COMPARE_MATCH		0X30


/* timer0 output compare match interrupt status options */
#define GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE		0X02
#define GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE		0X00

/* timer0 overflow interrupt status options */
#define GPT_TIMER0_OVERFLOW_INTERRUPT_ENABLE		0X01
#define GPT_TIMER0_OVERFLOW_INTERRUPT_DISABLE		0X00


//****************************** Timer 1 *********************************

/* compare output (OC1A) mode options */
#define GPT_OC1A_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC1A_TOGGLE_ON_COMPARE_MATCH	0X40
#define GPT_OC1A_CLEAR_ON_COMPARE_MATCH		0X80
#define GPT_OC1A_SET_ON_COMPARE_MATCH		0XC0

/* compare output (OC1B) mode options */
#define GPT_OC1B_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC1B_TOGGLE_ON_COMPARE_MATCH	0X10
#define GPT_OC1B_CLEAR_ON_COMPARE_MATCH		0X20
#define GPT_OC1B_SET_ON_COMPARE_MATCH		0X30


/* timer1 output compare match interrupt status options */
#define GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE	0X10
#define GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE	0X00

/* timer1 output compare match interrupt status options */
#define GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE	0X08
#define GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE	0X00

/* timer1 overflow interrupt status options */
#define GPT_TIMER1_OVERFLOW_INTERRUPT_ENABLE		0X04
#define GPT_TIMER1_OVERFLOW_INTERRUPT_DISABLE		0X00



//****************************** Timer 2 *********************************

/* compare output (OC2) mode options */
#define GPT_OC2_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC2_TOGGLE_ON_COMPARE_MATCH		0X10
#define GPT_OC2_CLEAR_ON_COMPARE_MATCH		0X20
#define GPT_OC2_SET_ON_COMPARE_MATCH		0X30


/* timer2 output compare match interrupt status options */
#define GPT_TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE		0X80
#define GPT_TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE		0X00

/* timer2 overflow interrupt status options */
#define GPT_TIMER2_OVERFLOW_INTERRUPT_ENABLE		0X40
#define GPT_TIMER2_OVERFLOW_INTERRUPT_DISABLE		0X00


#endif /* GPT_CFG_H_ */