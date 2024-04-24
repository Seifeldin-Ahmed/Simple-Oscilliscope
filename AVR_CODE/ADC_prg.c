/*
 * ADC_prg.c
 *
 *  Created on: Sep 4, 2022
 *      Author: Dell
 */

#include "STD_Types.h"
#include "ADC_int.h"
#include "ADC_prv.h"



static u16 ADC_u16Value = 0;
func_ptr_one_argument ADC_pfun ;



ADC_tenuErrorStatus ADC_enuEnable(u8 Copy_ReferenceSelection,u8 Copy_Channel,u8 Copy_u8InterruptMode)
{
	ADC_tenuErrorStatus Local_enuErrorStatus = ADC_OK;

 	if( ( ( ADC_u8_VOLTAGE_SELECTION_CHECK(Copy_ReferenceSelection))==0 ) || (Copy_Channel > ADC_u8_ADC7))
 	{
 		Local_enuErrorStatus = ADC_NOK;
 	}
 	else  total time = number of ticks 
 	{
 		ADC_u8_ADMUX_REG |=Copy_ReferenceSelection;
 		ADC_u8_ADMUX_REG |=Copy_Channel;
 		ADC_u8_ADCSRA_REG |= 0X06; //PRESCALER = 64 // Freq= 125khz --> 8 micro second for one clock	//can take 9615 Samples per second
 		ADC_u8_ADCSRA_REG |= 0x80; // ADC Enable
 		ADC_u8_ADCSRA_REG |= Copy_u8InterruptMode;

	}

	return Local_enuErrorStatus;
}

u8 ADC_enuStartConvertion(void)
{
    ADC_u8_ADCSRA_REG |= (1<<6); //Start Conversion
	while(!(ADC_u8_ADCSRA_REG & (1<<4)));		
	return ADC_u8_ADCH_REG;
}



void ADC_enuDisable(void)
{
	ADC_u8_ADCSRA_REG &= ~(1<<8); // ADC Disable
}



// NEW FUNCTIONS //

void ADC_enuStartConvertion_Asynchronous(void)
{
   
	ADC_u8_ADCSRA_REG |= 0X40; //Start Conversion
}

ADC_tenuErrorStatus ADC_enuGetValue_Asynch(func_ptr_one_argument Add_pfuncCBF)
{
	ADC_tenuErrorStatus  Local_enuErrorStatus = ADC_OK;
	/* Check Arguments */
	if(Add_pfuncCBF == NULL )
	{
		Local_enuErrorStatus=ADC_NOK;
	}
	else
	{
			ADC_pfun=Add_pfuncCBF;
	}
	return Local_enuErrorStatus;
}




void __vector_16(void)    __attribute__((signal,used));
void __vector_16(void)
{
	ADC_u16Value = (u16)(ADC_u8_ADCL_REG | (ADC_u8_ADCH_REG << 8));
	
	if(ADC_pfun != NULL)
	    ADC_pfun(&ADC_u16Value);
	
}
