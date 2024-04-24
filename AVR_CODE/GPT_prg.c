/*
 * GPT_prg.c
 *
 *  Created on: Sep 5, 2022
 *      Author: Dell
 */
#define F_CPU Cpu
#define  Crystal 32768UL
#define Cpu 8000000UL
#include "STD_Types.h"
#include "GPT_int.h"
#include "GPT_prv.h"

//#include <avr/interrupt.h>
typedef struct
{
	// new edition : adding volatile , because any variable shared between ISR and any function may be removed by some compilers
	volatile func_ptr GPT_pfunTIMER0 ; // void (*GPT_pfunTIMER0)(void) 
	volatile func_ptr GPT_pfunTIMER1_OC1A ; 
	volatile func_ptr GPT_pfunTIMER1_OC1B ;
	volatile func_ptr GPT_pfunTIMER2 ;

}GPT_tstrISR;

/* channels tick time */
static struct{
	u16 Channel_0_TickTime;
	u16 Channel_1_TickTime_OC1A;
	u16 Channel_1_TickTime_OC1B;
	u16 Channel_2_TickTime;
}Timer_TickTime;
//*********************** for adding more functionality on the same timer (Timer 0 and Timer 2) *****************************************
static u16 Channel_0_TickTime1=0;
static u16 Channel_0_TickTime2=0;
static u16 Channel_2_TickTime1=0;
static u16 Channel_2_TickTime2=0;
volatile func_ptr GPT_pfunTIMER0_1 = NULL ;
volatile func_ptr GPT_pfunTIMER0_2 = NULL ;
volatile func_ptr GPT_pfunTIMER2_1=NULL;
volatile func_ptr GPT_pfunTIMER2_2=NULL;
//**************************************************************************************************************************************
static u8 Global_u8Prescaler_Timer0;
static u8 Global_u8Prescaler_Timer1;
static u8 Global_u8Prescaler_Timer2;
static u8 Global_u8DesiredPin;
GPT_tstrISR GPT_strISR;

//************************************************* For OverFlow **********************************************************************'
//Timer 0
static u16 Global_u16TIMER0_OVFCounter=0; 
static u8 Global_u8TIMER0_OVFPreload=0;

// Timer 1
static u16 Global_u16TIMER1_OVFCounter=0;
static u16 Global_u16TIMER1_OVFPreload=0;

//Timer 2
static u16 Global_u16TIMER2_OVFCounter=0;
static u8 Global_u8TIMER2_OVFPreload=0;

//modes
static u8 Global_u8TIMER0_Mode;
static u8 Global_u8TIMER1_Mode;
static u8 Global_u8TIMER2_Mode;

// Prescaler Values
static const f32 TIMER_PRESCALER_VALUES[] =  {0.0, 1.0, 8.0, 64.0, 256.0, 1024.0};
static const f32 TIMER2_PRESCALER_VALUES[] = {0.0, 1.0, 8.0,32.0, 64.0,128.0, 256.0, 1024.0};
//**************************************************************************************************************************************

GPT_tenuErrorStatus GPT_enuConfigure(u8 Copy_u8Timer,GPT_tstrOptions* Add_strOptions)
{
	GPT_tenuErrorStatus Local_enuErrorStatus = GPT_OK;
	 if(Copy_u8Timer > GPT_u8_TIMER2)
	 {
		 Local_enuErrorStatus = GPT_NOK;
	 }
	 else
	 { 
		 switch(Copy_u8Timer)
		{
			case GPT_u8_TIMER0 :
                                /* select CTC mode*/
								/*??*/   GPT_u8_TCCR0_REG   |= (Add_strOptions->Local_u8Mode);
							      Global_u8TIMER0_Mode = (Add_strOptions->Local_u8Mode);
								// Select compare mode //
								GPT_u8_TCCR0_REG   |= (Add_strOptions->Local_u8CompareOutputMode);
                                /* load a value in OCR0 */
                                GPT_u8_OCR0_REG=Add_strOptions->Local_u8CompareValue;
                                /* select timer clock */
                          //      GPT_u8_TCCR0_REG |=  Add_strOptions->Local_u8Prescaler;
                              
                                /* enable interrupt*/
                                GPT_u8_TIMSK_REG |= Add_strOptions->Local_u8Interrupt;
								Global_u8Prescaler_Timer0 = (Add_strOptions->Local_u8Prescaler);

								break;
		    case GPT_u8_TIMER1 :
			
			                    GPT_u8_TCCR1B  |= (Add_strOptions->Local_u8Mode);
								Global_u8TIMER1_Mode = (Add_strOptions->Local_u8Mode);
								GPT_u8_TCCR1A  |= (Add_strOptions->Local_u8CompareOutputMode);
								if( (Add_strOptions->Local_u8DesiredPin)==GPT_u8_OC1A)
								{   
								    GPT_u16_OCR1AL |= (Add_strOptions->Local_u8CompareValue);
									Global_u8DesiredPin = Add_strOptions->Local_u8DesiredPin;
								}
								else if (Add_strOptions->Local_u8DesiredPin==GPT_u8_OC1B)
								{	
									GPT_u16_OCR1BL |= (Add_strOptions->Local_u8CompareValue);
									Global_u8DesiredPin = Add_strOptions->Local_u8DesiredPin;
								}
								else
								{
									Local_enuErrorStatus= GPT_NOK;
								}
			                    GPT_u8_TIMSK_REG   |= (Add_strOptions->Local_u8Interrupt);
							    Global_u8Prescaler_Timer1 = (Add_strOptions->Local_u8Prescaler);
								break;
			case GPT_u8_TIMER2 : 
			                    GPT_u8_TCCR2_REG   |= (Add_strOptions->Local_u8Mode);
								Global_u8TIMER2_Mode = (Add_strOptions->Local_u8Mode);
			                    GPT_u8_TCCR2_REG   |= (Add_strOptions->Local_u8CompareOutputMode);
			                    GPT_u8_OCR2_REG    |= (Add_strOptions->Local_u8CompareValue);
			                    GPT_u8_TIMSK_REG   |= (Add_strOptions->Local_u8Interrupt);
			                    Global_u8Prescaler_Timer2 = (Add_strOptions->Local_u8Prescaler);
								GPT_u8_ASSR_REG |= GPT_Asynchronise_Option;
			                    break;						
			                    					
		}		            

	 }
	return Local_enuErrorStatus;
}

 GPT_tenuErrorStatus GPT_enuDelay_ms(u8 Copy_u8ChannelNumber, u16 Copy_u16TimeMs)
{
	GPT_tenuErrorStatus Loc_enuErrorStatus = GPT_OK;

	if(Copy_u8ChannelNumber > GPT_u8_TIMER2)
	{
		Loc_enuErrorStatus = GPT_NOK;
	}
	else
	{
		switch(Copy_u8ChannelNumber)
		{
			// Prescaler = 64 , Fcpu=8000000 , desired time = 1ms
	     	case GPT_u8_TIMER0:
	  	        	/* preload value to count 1 ms */
             		GPT_u8_TCNT0_REG = 131;
             		/* set the Prescaler to 64 and start the timer */
             		
             		GPT_u8_TCCR0_REG &= ~(0x07); // to clear Prescaler
             		GPT_u8_TCCR0_REG |= GPT_u8_NO_PRESCALER_64;
             
             		/* wait until the desired time in ms pass */
             		while(Copy_u16TimeMs != 0)
             		{
             			--Copy_u16TimeMs;
             
             			/* wait until overflow flag rises
             			 * note that overflow takes 1 ms
             			 */
             			while((GPT_u8_TIFR_REG & 1) == 0)
             			{
             				asm("NOP");
	 	    	    	}
	                   
	  	                	/* clear the overflow flag */
	  	                	GPT_u8_TIFR_REG |= 1;
	                   
	  	                	/* load the preload value again */
	  	                	GPT_u8_TCNT0_REG = 131;
	  	             }
	                   
	 	             /* disable the timer */
	 	           	GPT_u8_TCCR0_REG &= ~(0x07); // to clear Prescaler
	 	            break;

			case GPT_u8_TIMER1:
				/* preload value to count 1 ms */
				GPT_u16_TCNT1L = 65411;

				/* set the Prescaler to 64 and start the timer */
				GPT_u8_TCCR1B &= ~(0x07);
				GPT_u8_TCCR1B |= GPT_u8_NO_PRESCALER_64;

				/* wait until the desired time in ms pass */
				while(Copy_u16TimeMs != 0)
				{
					--Copy_u16TimeMs;

					/* wait until overflow flag rises
					 * note that overflow takes 1 ms
					 */
					while((GPT_u8_TIFR_REG & 0x04) == 0)
					{
						asm("NOP");
					}

					/* clear the overflow flag */
					GPT_u8_TIFR_REG |= 0X04;

					/* load the preload value again */
					GPT_u16_TCNT1L = 65411;
				}

				/* disable the timer */
				GPT_u8_TCCR1B &= ~(0x07);
				break;

			case GPT_u8_TIMER2:
				/* preload value to count 1 ms */
				GPT_u8_TCNT2_REG = 131;

				/* set the Prescaler to 64 and start the timer */
				GPT_u8_TCCR2_REG &= ~(0x07);
				GPT_u8_TCCR2_REG |= GPT_u8_NO_PRESCALER_64;

				/* wait until the desired time in ms pass */
				while(Copy_u16TimeMs != 0)
				{
					--Copy_u16TimeMs;

					/* wait until overflow flag rises
					 * note that overflow takes 1 ms
					 */
					while((GPT_u8_TIFR_REG & 0X40) == 0)
					{
						asm("NOP");
					}

					/* clear the ovwrflow flag */
					GPT_u8_TIFR_REG |= 0X40;

					/* load the preload value again */
					GPT_u8_TCNT2_REG = 131;
				}

				/* disable the timer */
				GPT_u8_TCCR2_REG &= ~(0X07);
				break;
		}
	}

	return Loc_enuErrorStatus;
}


GPT_tenuErrorStatus GPT_enuStartTimer(u8 Copy_u8Timer,u32 Copy_time_in_ms) // Hint: make sure to check above if the frequency is F-CPU or Crystal which 32768
{
	   GPT_tenuErrorStatus Local_enuErrorStatus= GPT_OK;
       if(Copy_u8Timer >GPT_u8_TIMER2)
	   {
		   Local_enuErrorStatus = GPT_NOK;
	   }
	   else
	   { 
		   switch (Copy_u8Timer)
		     {
			   case GPT_u8_TIMER0 : 
			                  GPT_u8_TCCR0_REG |= Global_u8Prescaler_Timer0;
							  if(Global_u8TIMER0_Mode == GPT_u8_NORMAL)
							  { 
								    f32 Local_f32TickTime_ms =  (TIMER_PRESCALER_VALUES[Global_u8Prescaler_Timer0] * 1000/ F_CPU)  ;// Calculate tick time = Prescaler / F-cpu
								  Global_u16TIMER0_OVFCounter = (u16)(Copy_time_in_ms / Local_f32TickTime_ms / 256) ; // time of over flow = desired time / (tick time * 256 )= (desired time / tick time) / 256
								  Global_u8TIMER0_OVFPreload =(u8)(256- (( Copy_time_in_ms /Local_f32TickTime_ms ) -  ((u32)Global_u16TIMER0_OVFCounter * 256) ));
								  // to calculate preload : [ desired time / (tick time * 256 ) ] - [desired time / (tick time * 256 ) ] : this suppose to give 0 but this will not happen
								  //because the first term we calculate it in the equation so the result will be for example 15.438
								  // but second term we store it in integer so it will give only 15 : 15.438-15= 0.438
								  // we have to multiply 0.438 by 256 then subtract the result from 256
								  if(Global_u8TIMER0_OVFPreload>0)
								  {
									  Global_u16TIMER0_OVFCounter++;
								  }
								  GPT_u8_TCNT0_REG = Global_u8TIMER0_OVFPreload;
							  }
							  else
							  { 			
							     Timer_TickTime.Channel_0_TickTime =(u16) Copy_time_in_ms;
							  }
							  break;
							  
			    case GPT_u8_TIMER1 :
			                   GPT_u8_TCCR1B |= Global_u8Prescaler_Timer1;
							   if(1)
							   {
								    asm("NOP");
							   }
							   else
							   {								   
						             if(Global_u8TIMER1_Mode == GPT_u8_NORMAL)
						              {
							             	     f32 Local_u32TickTime_ms =  (TIMER_PRESCALER_VALUES[Global_u8Prescaler_Timer1]* 1000.0/ F_CPU) ;
							             	     Global_u16TIMER1_OVFCounter = (u16)(Copy_time_in_ms /Local_u32TickTime_ms / 65536) ; // time of over flow = desired time / (tick time * 65536 )= (desired time / tick time) / 65536
							             	     Global_u16TIMER1_OVFPreload =65536- (( Copy_time_in_ms /Local_u32TickTime_ms ) -  (Global_u16TIMER1_OVFCounter * 65536) );
							             	     // to calculate preload : [ desired time / (tick time * 65536 ) ] - [desired time / (tick time * 65536 ) ] : this suppose to give 0 but this will not happen
							             	     //because the first term we calculate it in the equation so the result will be for example 15.438
							             	     // but second term we store it in integer so it will give only 15 : 15.438-15= 0.438
							             	     // we have to multiply 0.438 by 256 then subtract the result from 256
							             	     if(Global_u16TIMER1_OVFPreload>0)
							             	     {
								             	     Global_u16TIMER1_OVFCounter++;
							             	     }
							             	     GPT_u16_TCNT1L = Global_u16TIMER1_OVFPreload;
						               }
						               else
						          	   {
							              if(Global_u8DesiredPin == GPT_u8_OC1A)
						             	     { Timer_TickTime.Channel_1_TickTime_OC1A = (u16)Copy_time_in_ms;}
							              else
						             	     {Timer_TickTime.Channel_1_TickTime_OC1B =(u16)Copy_time_in_ms;}
					 	       	       }		
								}														   								   
			                    break;
							   
				 case GPT_u8_TIMER2 :
				               GPT_u8_TCCR2_REG |= Global_u8Prescaler_Timer2;
							   if(Global_u8TIMER2_Mode == GPT_u8_NORMAL)
							   {
								    f32 Local_u32TickTime_ms =  (TIMER2_PRESCALER_VALUES[Global_u8Prescaler_Timer2]* 1000.0/ F_CPU) ;
								   Global_u16TIMER2_OVFCounter = (u16)(Copy_time_in_ms /Local_u32TickTime_ms / 256) ; // time of over flow = desired time / (tick time * 256 )= (desired time / tick time) / 256
								   Global_u8TIMER2_OVFPreload =256- (( Copy_time_in_ms /Local_u32TickTime_ms ) -  (Global_u16TIMER2_OVFCounter * 256) );
								   // to calculate preload : [ desired time / (tick time * 256 ) ] - [desired time / (tick time * 256 ) ] : this suppose to give 0 but this will not happen
								   //because the first term we calculate it in the equation so the result will be for example 15.438
								   // but second term we store it in integer so it will give only 15 : 15.438-15= 0.438
								   // we have to multiply 0.438 by 256 then subtract the result from 256
								   if(Global_u8TIMER2_OVFPreload>0)
								   {
									   Global_u16TIMER2_OVFCounter++;
								   }
								   GPT_u8_TCNT2_REG = Global_u8TIMER2_OVFPreload;
							   }
							   else
							     { 
									 Timer_TickTime.Channel_2_TickTime = (u16)Copy_time_in_ms;
								 }									 
				               break;
			   
		     }
		    
	   }
	  
	return Local_enuErrorStatus;
}

GPT_tenuErrorStatus GPT_enuStopTimer(u8 Copy_u8Timer)
{
	GPT_tenuErrorStatus Local_enuErrorStatus= GPT_OK;

    if(Copy_u8Timer > GPT_u8_TIMER2)
     {
 	    Local_enuErrorStatus = GPT_NOK;
     }
    else
       {  // clear Prescaler 
	        switch(Copy_u8Timer)
	           {
                	case GPT_u8_TIMER0:
                 	/* clear the clock source to disable the timer */
					 
                	GPT_u8_TCCR0_REG &= ~(0x07);
					GPT_u8_TCNT0_REG = 0;
                	break;
                
                	case GPT_u8_TIMER1:
                	/* clear the clock source to disable the timer */
                	  GPT_u8_TCCR1B &= ~(0x07);
					  GPT_u16_TCNT1L = 0;
					  
                	break;
                
                	case GPT_u8_TIMER2:
                	/* clear the clock source to disable the timer */
                	GPT_u8_TCCR2_REG &= ~(0x07);
					GPT_u8_TCNT2_REG = 0;
		            break;
	         }
      } 
		return Local_enuErrorStatus;
}

GPT_tenuErrorStatus GPT_enuRegisterCBF(u8 Copy_u8Timer,func_ptr Add_pfuncCBF)
{
	GPT_tenuErrorStatus Local_enuErrorStatus= GPT_OK;
	 if(Copy_u8Timer > GPT_u8_TIMER2 || Add_pfuncCBF == NULL)
		 {
			 Local_enuErrorStatus = GPT_NOK;
		 }
	 else
	 {
		 switch(Copy_u8Timer)
		 {

		       case GPT_u8_TIMER0:
		  		                    GPT_strISR.GPT_pfunTIMER0=Add_pfuncCBF;
		  		                    break;
		       case GPT_u8_TIMER1:
			                       if (Global_u8TIMER1_Mode == GPT_u8_NORMAL)  // if mode was over flow , force desired pin to be oc1A because ISR of over flow takes GPT_strISR.GPT_pfunTIMER1_OC1A as a call back function
								         Global_u8DesiredPin = GPT_u8_OC1A;
										 
			                      if(Global_u8DesiredPin == GPT_u8_OC1A )
										 GPT_strISR.GPT_pfunTIMER1_OC1A=Add_pfuncCBF;
						     	  else 
								      	 GPT_strISR.GPT_pfunTIMER1_OC1B=Add_pfuncCBF;
		    		  		      break;
		       case GPT_u8_TIMER2:
		    		  		        GPT_strISR.GPT_pfunTIMER2=Add_pfuncCBF;
		    		  		         break;
		 }
	 }
		return Local_enuErrorStatus;
}

//******************************************************** Function Overloading ******************************************************
GPT_tenuErrorStatus GPT_enuStartTimer_MultiTimes(u8 Copy_u8Timer,u32 Copy_time1_in_ms,u32 Copy_time2_in_ms,u32 Copy_time3_in_ms)
{
	GPT_tenuErrorStatus Local_enuErrorStatus= GPT_OK;
	if(Copy_u8Timer >GPT_u8_TIMER2)
	{
		Local_enuErrorStatus = GPT_NOK;
	}
	else
	{
		switch (Copy_u8Timer)
		{
			case GPT_u8_TIMER0 :
			GPT_u8_TCCR0_REG |= Global_u8Prescaler_Timer0;
			Timer_TickTime.Channel_0_TickTime =(u16) Copy_time1_in_ms;
			Channel_0_TickTime1 =(u16) Copy_time2_in_ms;
			Channel_0_TickTime2 =(u16) Copy_time3_in_ms;
			break;
			
			case GPT_u8_TIMER1 :
			GPT_u8_TCCR1B |= Global_u8Prescaler_Timer1;
			if(Global_u8DesiredPin == GPT_u8_OC1A)
		{ Timer_TickTime.Channel_1_TickTime_OC1A = (u16)Copy_time1_in_ms;}
			else
		{Timer_TickTime.Channel_1_TickTime_OC1B =(u16)Copy_time1_in_ms;}
			break;
			
			case GPT_u8_TIMER2 :
			GPT_u8_TCCR2_REG |= Global_u8Prescaler_Timer2;
			Timer_TickTime.Channel_2_TickTime = (u16)Copy_time1_in_ms;
			Channel_2_TickTime1 = (u16) Copy_time2_in_ms;
			Channel_2_TickTime2 =(u16) Copy_time3_in_ms;
			break;
			
		}
		
	}
	
	return Local_enuErrorStatus;
}

GPT_tenuErrorStatus GPT_enuRegisterCBF_MultiTimes(u8 Copy_u8Timer,func_ptr Add_pfuncCBF1,func_ptr Add_pfuncCBF2,func_ptr Add_pfuncCBF3)
{
	GPT_tenuErrorStatus Local_enuErrorStatus= GPT_OK;
	if(Copy_u8Timer > GPT_u8_TIMER2 || Add_pfuncCBF1 == NULL || Add_pfuncCBF2 == NULL)
	{
		Local_enuErrorStatus = GPT_NOK;
	}
	else
	{
		switch(Copy_u8Timer)
		{

			case GPT_u8_TIMER0:
			GPT_strISR.GPT_pfunTIMER0=Add_pfuncCBF1;
			 GPT_pfunTIMER0_1=Add_pfuncCBF2;
			 GPT_pfunTIMER0_2=Add_pfuncCBF3;
			break;
			case GPT_u8_TIMER1:
			
			if(Global_u8DesiredPin == GPT_u8_OC1A)
			GPT_strISR.GPT_pfunTIMER1_OC1A=Add_pfuncCBF1;
			else
			GPT_strISR.GPT_pfunTIMER1_OC1B=Add_pfuncCBF1;
			break;
			case GPT_u8_TIMER2:
			GPT_strISR.GPT_pfunTIMER2=Add_pfuncCBF1;
			GPT_pfunTIMER2_1 =Add_pfuncCBF2;
			GPT_pfunTIMER2_2 =Add_pfuncCBF3;
			break;
		}
	}
	return Local_enuErrorStatus;
}
//*******************************************************************************************************************************

u8 Global_var;

u32 TIMER_Ticks(u8 Copy_Timer)
{
	u32 Local_TimerTicks;

	switch (Copy_Timer)
	{
		case GPT_u8_TIMER0:
		Local_TimerTicks = GPT_u8_TCNT0_REG + Global_var * 256;
		GPT_u8_TCNT0_REG=0;
		Global_var=0;
		break;

		case GPT_u8_TIMER1:
		Local_TimerTicks = GPT_u16_TCNT1L + Global_var * 65536; 

		break;

		case GPT_u8_TIMER2:
		Local_TimerTicks = GPT_u8_TCNT2_REG + Global_var * 256;
		GPT_u8_TCNT2_REG = 0;
		Global_var=0;
		break;
	}

	return Local_TimerTicks;
}

//*********************************************************************************************** Compare Match ISR *******************************************************************************************
// ISR for Timer 0 COMP //
void __vector_10(void)     __attribute__((signal,used));
void __vector_10(void)    // if Prescaler = 1024 and desired time = 10 ms so OCR0 has to be 77 
{
	static u16 Loc_u16TickTime=0;
	static u16 Loc_u16TickTime1=0;
	static u16 Loc_u16TickTime2=0;
	Loc_u16TickTime++;
	Loc_u16TickTime1++;
	Loc_u16TickTime2++;
	if(GPT_strISR.GPT_pfunTIMER0 != NULL)
	{
            if(Loc_u16TickTime == Timer_TickTime.Channel_0_TickTime)
                 {
	                Loc_u16TickTime = 0;
  	                GPT_strISR.GPT_pfunTIMER0();
                 }
	}	  
	 if(GPT_pfunTIMER0_1 != NULL)
	 {
	       if(Loc_u16TickTime1 ==Channel_0_TickTime1 )
	           {
	          	Loc_u16TickTime1=0;
		          GPT_pfunTIMER0_1();
	           }
	 }	  
	if(GPT_pfunTIMER0_2 != NULL)
 	 {
		 	 if(Loc_u16TickTime2 ==Channel_0_TickTime2 )
		 	 {
			 	 Loc_u16TickTime2=0;
			 	 GPT_pfunTIMER0_2();
		 	 }
	 }
}

// ISR for Timer 1 COMPA //
void __vector_7(void)     __attribute__((signal,used));
void __vector_7(void)    // if Prescaler = 1024 and desired time = 10 ms so OCR0 has to be 77
{
	static u16 Loc_u16TickTime=0;
	Loc_u16TickTime++;
	
   if(GPT_strISR.GPT_pfunTIMER1_OC1A != NULL)
    {
	 	if(Loc_u16TickTime == Timer_TickTime.Channel_1_TickTime_OC1A)
     	{
		     Loc_u16TickTime = 0;	 
	     	 GPT_strISR.GPT_pfunTIMER1_OC1A();
	    }
    }	
}

// ISR for Timer 1 COMPB //
void __vector_8(void)     __attribute__((signal,used));
void __vector_8(void)    // if Prescaler = 1024 and desired time = 10 ms so OCR0 has to be 77
{
	static u16 Loc_u16TickTime=0;
	Loc_u16TickTime++;
	
	if(Loc_u16TickTime == Timer_TickTime.Channel_1_TickTime_OC1B)
	{
		Loc_u16TickTime = 0;
		GPT_strISR.GPT_pfunTIMER1_OC1B();
	}
}

// ISR for Timer 2 COMP //
void __vector_4(void)     __attribute__((signal,used));
void __vector_4(void)    // if Prescaler = 1024 and desired time = 10 ms so OCR0 has to be 77
{
		static u16 Loc_u16TickTime=0;
		static u16 Loc_u16TickTime1=0;
		static u16 Loc_u16TickTime2=0;
		Loc_u16TickTime++;
		Loc_u16TickTime1++;
		Loc_u16TickTime2++;
   if(GPT_strISR.GPT_pfunTIMER2 != NULL)
    {
	  	  if(Loc_u16TickTime == Timer_TickTime.Channel_2_TickTime)
          {
		      Loc_u16TickTime = 0;
		     GPT_strISR.GPT_pfunTIMER2();
	      }
   }	
	
	  if(GPT_pfunTIMER2_1 != NULL)
      {
    	    if(Loc_u16TickTime1 == Channel_2_TickTime1)
    	     {
    		    Loc_u16TickTime1 = 0;
    		    GPT_pfunTIMER2_1();
    	    }
      }
     if(GPT_pfunTIMER2_2 != NULL)
     {
	       if(Loc_u16TickTime2 == Channel_2_TickTime2)
	       {
		      Loc_u16TickTime2 = 0;
		      GPT_pfunTIMER2_2();
	       }
    } 
	
}

//****************************************************************************************************************************************************************************************************************





//*********************************************************************************************** OVER FLOW ISR *******************************************************************************************
// ISR for Timer 0 OVF //
void __vector_11(void)     __attribute__((signal,used));
void __vector_11(void)
{
  static u16 Loc_u16TickTime=0;
  Loc_u16TickTime++;
  
  if(GPT_strISR.GPT_pfunTIMER0 != NULL)
  {
	  if(Loc_u16TickTime == Global_u16TIMER0_OVFCounter)
	  {
		  Loc_u16TickTime = 0;
		   GPT_u8_TCNT0_REG = Global_u8TIMER0_OVFPreload;
		   GPT_strISR.GPT_pfunTIMER0();
		 
	  }
  }
}

// ISR for Timer 1 OVF //
// void __vector_9(void)     __attribute__((signal,used));
// void __vector_9(void)
// {
// 	static u16 Loc_u16TickTime=0;
// 	Loc_u16TickTime++;
// 	
// 	if(GPT_strISR.GPT_pfunTIMER1_OC1A != NULL)
// 	{
// 		if(Loc_u16TickTime == Global_u16TIMER1_OVFCounter)
// 		{
// 			Loc_u16TickTime = 0;
// 			GPT_u16_TCNT1L = Global_u16TIMER1_OVFPreload;
// 			GPT_strISR.GPT_pfunTIMER1_OC1A();
// 		}
// 	}
// }


void __vector_9(void)     __attribute__((signal,used));
void __vector_9(void)
{
	  Global_var ++;
}


// ISR for Timer 2 OVF //
void __vector_5(void)     __attribute__((signal,used));
void __vector_5(void)
{
	static u16 Loc_u16TickTime=0;
	Loc_u16TickTime++;
	
	if(GPT_strISR.GPT_pfunTIMER2 != NULL)
	{
		if(Loc_u16TickTime == Global_u16TIMER2_OVFCounter)
		{
			Loc_u16TickTime = 0;
			GPT_u8_TCNT2_REG = Global_u8TIMER2_OVFPreload;
			GPT_strISR.GPT_pfunTIMER2();
		}
	}
}


