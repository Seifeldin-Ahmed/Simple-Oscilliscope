/*
 * ADC_int.h
 *
 *  Created on: Sep 4, 2022
 *      Author: Dell
 */

#ifndef MCAL_ADC_ADC_INT_H_
#define MCAL_ADC_ADC_INT_H_

/* Check Error Status */
typedef enum
{
 ADC_OK,
 ADC_NOK
}ADC_tenuErrorStatus;


// macros for Reference Selection //
#define ADC_u8_AREF                0X00
#define ADC_u8_AVCC                0X40
#define ADC_u8_INTERNAL_VOLTAGE    0XC0


#define ADC_u8_VOLTAGE_SELECTION_CHECK(OPTION)     (OPTION == ADC_u8_AREF)  ||   \
		                                           (OPTION == ADC_u8_AVCC)  ||   \
		                                           (OPTION == ADC_u8_INTERNAL_VOLTAGE)
//macros for Channels //
#define ADC_u8_ADC0        0X00
#define ADC_u8_ADC1        0X01
#define ADC_u8_ADC2        0X02
#define ADC_u8_ADC3        0X03
#define ADC_u8_ADC4        0X04
#define ADC_u8_ADC5        0X05
#define ADC_u8_ADC6        0X06
#define ADC_u8_ADC7        0X07

// macros for interrupt
#define ADC_INTERRUPT_ENABLE	0X08
#define ADC_INTERRUPT_DISABLE	0X00


ADC_tenuErrorStatus ADC_enuEnable(u8 Copy_ReferenceSelection,u8 Copy_Channel,u8 Copy_u8InterruptMode);


u8 ADC_enuStartConvertion(void);

void ADC_enuDisable(void);

ADC_tenuErrorStatus ADC_enuGetValue(u16* Add_Value);


// New functions

void ADC_enuStartConvertion_Asynchronous(void);

ADC_tenuErrorStatus ADC_enuGetValue_Asynch( func_ptr_one_argument Add_pfuncCBF);

#endif /* MCAL_ADC_ADC_INT_H_ */
