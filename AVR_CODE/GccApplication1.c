/*
 * GccApplication1.c
 *
 * Created: 03/12/2023 1:35:13 PM
 *  Author: Seifeldin Ahmed
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "STD_Types.h"
#include "UART_int.h"
#include "GPT_int.h"
#include "GPT_prv.h"
#include "ADC_int.h"


#define _PINS_ST_SIZE 2
#define _TIME_SNAP_SIZE 5
#define _FRAME_SIZE (7)
#define _SAMPLE_PIN_INDEX 0
#define _SAMPLE_TIME_INDEX 2

// Send the following frame for each sample:
// PIN1 PIN2 TIME4 TIME3 TIME2 TIME1 TIME0 

#define _SAMPLES_NUM 200
typedef enum  {
	SAMPLING,
	SENDING,
	IDLE
} states_t;

states_t currentState;
u8 pin_states[_SAMPLES_NUM] = {0};
u8 fractions[_SAMPLES_NUM] = {0};
unsigned long int time_snap[_SAMPLES_NUM] = {0};


extern unsigned char Global_var;


int main(void)
{   
	     DDRB = 0 ; // make DDRB input 
		 DDRA = 254; //output
		 DDRD = 2 ; 
		 DDRC = 255; //output
		 UART_tstrConfiguration uart_object;
		 uart_object.UART_u8ParityBit = UART_u8_DISABLED_PARITY;
		 uart_object.UART_u8PStopBits = UART_u8_ONE_STOP_BIT;
		 uart_object.UART_u8DataBits =UART_u8_EIGHT_DATA_BITS;
		 uart_object.UART_u8Mode = UART_u8_FULL_DUPLEX;
		 uart_object.UART_u32BaudRate = 9600;
		 UART_enuInit(&uart_object);
		 
		 
		 ADC_enuEnable(ADC_u8_AVCC,ADC_u8_ADC0,ADC_INTERRUPT_DISABLE); 
		 	 
		 GPT_tstrOptions timer_object;
 		 timer_object.Local_u8Prescaler=GPT_u8_NO_PRESCALER_8;
 		 timer_object.Local_u8Mode=GPT_u8_NORMAL;
 		 timer_object.Local_u8CompareOutputMode=GPT_OC1A_NORMAL_PORT_CONNECTION; 
 		 timer_object.Local_u8Interrupt=GPT_TIMER1_OVERFLOW_INTERRUPT_ENABLE;
 		 timer_object.Local_u8DesiredPin = GPT_u8_OC1A;
 		 GPT_enuConfigure(GPT_u8_TIMER1 , &timer_object);
		 sei();
				 
		 currentState = SAMPLING;
		 
 		 static volatile uint8_t samples_cnt = 0;
 		 static char _go_signal = 'G';
		 GPT_enuStartTimer(GPT_u8_TIMER1,0);		 
         u8 Digital_value, fra;
		 float analog_value;
    while(1)
    {
		    	 
        	 switch(currentState)	
        	 {    
	        	 case SAMPLING:
	        	 {

					  Digital_value = ADC_enuStartConvertion();
					  analog_value = ((Digital_value * 5ul)/255.0); 
					  fra = (u8)((analog_value - (u8)analog_value) * 10); 
 					  pin_states[samples_cnt] = (u8)(analog_value);
					  fractions[samples_cnt] = fra;
  		        	  time_snap[samples_cnt] = TIMER_Ticks(GPT_u8_TIMER1);

 		        	  samples_cnt++;
 		        	  currentState = (samples_cnt >= _SAMPLES_NUM) ? SENDING : SAMPLING; 
		        	  break;
	        	 }
        	        	 case SENDING:
        	        	 {
        		        	 // For _SAMPLES_NUM samples send the construct the buffer.
        		        	 static uint8_t _sample_buf[_FRAME_SIZE];
        		        	 for(uint8_t i = 0; i < _SAMPLES_NUM; ++i)
        		        	 {
        			        	 // Construct the buffer
        			        	 // Add pin value.
								 _sample_buf[_SAMPLE_PIN_INDEX] = pin_states[i] ;  
								 _sample_buf[_SAMPLE_PIN_INDEX + 1] = fractions[i] ;  
        			        	 // Add time snap value.
        			        	 _sample_buf[_SAMPLE_TIME_INDEX + 0] = ((time_snap[i] & 0xFE000000) >> 25);
        			        	 _sample_buf[_SAMPLE_TIME_INDEX + 1] = ((time_snap[i] & 0x01FC0000) >> 18);
        			        	 _sample_buf[_SAMPLE_TIME_INDEX + 2] = ((time_snap[i] & 0x0003F800) >> 11);
        			        	 _sample_buf[_SAMPLE_TIME_INDEX + 3] = ((time_snap[i] & 0x000007F0) >> 4);
        						 _sample_buf[_SAMPLE_TIME_INDEX + 4] = ((time_snap[i] & 0x0000000F) >> 0);
        			        	 
        			        	 // Send sample.
        			        	 UART_enuTransmite(_sample_buf[_SAMPLE_PIN_INDEX]);
								 UART_enuTransmite(_sample_buf[_SAMPLE_PIN_INDEX + 1]);
								   
        			        	 UART_enuTransmite(_sample_buf[_SAMPLE_TIME_INDEX + 0]);
        			        	 UART_enuTransmite(_sample_buf[_SAMPLE_TIME_INDEX + 1]);
        			        	 UART_enuTransmite(_sample_buf[_SAMPLE_TIME_INDEX + 2]);
        			        	 UART_enuTransmite(_sample_buf[_SAMPLE_TIME_INDEX + 3 ]);
        						 UART_enuTransmite(_sample_buf[_SAMPLE_TIME_INDEX + 4 ]);
        			        	 
        		        	 }
        		        	 // Trigger receiving for go signal.
        		        	 // turn on led
        					 PORTC = 128;
        		        	 UART_enuReceive(&_go_signal);
        	        	 }
        	        	 case IDLE:
        	        	 {
        		        	 currentState = (_go_signal == 'G') ? SAMPLING : IDLE;
        		        	 if(currentState == SAMPLING)
        		        	 {
        			        	 GPT_u16_TCNT1L=0;
        			        	 Global_var=0;
        						 PORTC = 0;
        		        	 }
        		        	 break;
        	        	 }
                 }
                
        
    }
}
