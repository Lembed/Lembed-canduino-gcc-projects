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
 * This version can generate a PWM signal up to ~40kHz at any duty cycle you like.
 * The compare values are directly compared in the main loop which allows higher frequencies
 * than an interrupt driven version. 
 * 
 * The compare functionality could be put into another function that gets called every time the 
 * loop starts again to minimize the crowded code inside the main loop. 
 * 
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

#define F_PWM 10000
#define Prescaler 8
#define ICR1_val ((F_CPU / F_PWM) / Prescaler) - 1

uint16_t PWM_setting[8];

int main(void){
	
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11); // CTC mode prescaler 8
	
	ICR1 = ICR1_val;
	
	DDRD = 0xFF;
	
	uint8_t tmp;
	
	for(uint8_t i = 0; i < 8; i++){
		PWM_setting[i] = (((F_CPU / F_PWM) / Prescaler) - 1) / 2; // set duty cycle
	}

	while(1){
		
		tmp = 0; 
			
		if(PWM_setting[0] > TCNT1) tmp |= (1<<0);
		if(PWM_setting[1] > TCNT1) tmp |= (1<<1);
		if(PWM_setting[2] > TCNT1) tmp |= (1<<2);
		if(PWM_setting[3] > TCNT1) tmp |= (1<<3);
		if(PWM_setting[4] > TCNT1) tmp |= (1<<4);
		if(PWM_setting[5] > TCNT1) tmp |= (1<<5);
		if(PWM_setting[6] > TCNT1) tmp |= (1<<6);
		if(PWM_setting[7] > TCNT1) tmp |= (1<<7);
		
		PORTD = tmp;
	}
	
	return 0;
}

