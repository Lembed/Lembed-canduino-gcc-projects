/*
 *   Author: Torrentula
 *   Website: http://torrentula.funpic.de
 *
 * -------------------------------------------------------------------------
 *
 *	This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------------
 *
 * Please provide a link to this website if you post this on a forum
 * or if you redistribute/modify it
 * 
 * 
 * This example code shows how to create an 8 channel PWM signal in software.
 * This version can generate a PWM signal up to 500Hz at any duty cycle you like.
 * 
 * The compare values are compared inside the interrupt service routine (ISR). This ensures that the
 * PWM signals are generated even if the main loop hangs up unexpectedly. The downside is that the timer needs 
 * to run 256 times faster than the actual PWM frequency (provided you want to use 8bit PWM).
 * 
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_PWM 100
#define PWM_RES 256

#define OCR1A_val (F_CPU / (F_PWM * PWM_RES))

volatile uint8_t PWM_setting[8];

int main(void){
	
	TCCR1B = 1; // CTC mode prescaler 1
	TIMSK1 = (1<<OCIE1A); // enable timer 1 compare match interrupt
	
	DDRD = 0xFF;
	
	for(uint8_t i = 0; i < 8; i++){
		PWM_setting[i] = (F_CPU / (F_PWM * PWM_RES)) / 2; // 50% duty cycle
	}
	
	sei(); // globally enable interrupts

	while(1){
		
		// do anything you want here
		
	}
	
	return 0;
}

ISR(TIMER1_COMPA_vect){

	static uint8_t overflows;
	
	uint8_t tmp = 0; // tmp variable to save pin status
	
	OCR1A += OCR1A_val;
			
	if(PWM_setting[0] > overflows) tmp |= (1<<0);
	if(PWM_setting[1] > overflows) tmp |= (1<<1);
	if(PWM_setting[2] > overflows) tmp |= (1<<2);
	if(PWM_setting[3] > overflows) tmp |= (1<<3);
	if(PWM_setting[4] > overflows) tmp |= (1<<4);
	if(PWM_setting[5] > overflows) tmp |= (1<<5);
	if(PWM_setting[6] > overflows) tmp |= (1<<6);
	if(PWM_setting[7] > overflows) tmp |= (1<<7);
		
	PORTD = tmp; // only one call of PORTD reduces execution time
	
	// check if the timer overflows have exceeded the needed value
	if(overflows > (PWM_RES-1)){
		overflows = 0;
	}
	else{
		overflows++;
	}
}

