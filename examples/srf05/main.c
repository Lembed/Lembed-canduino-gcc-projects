/*
 *   Author: Torrentula
 *   Website: torrentula.funpic.de
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
 *	This example shows how to read out an SRF05 ultra sonic range sensor
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SRFout PD6 // SRF05 trigger pin is connected here
#define SRFin PD2 // SRF05 echo pin is connected here

volatile unsigned int microseconds = 0; // to save pulse length
volatile uint8_t INT0_interrupt = 0; // to indicate whether an INT0 interrupt has already occured
volatile uint8_t measurement_complete = 0; // is used to inicate that the measurement is complete

void init_SRF05(void){
	// setupt Timer
	TCCR0A = (1<<WGM01); // CTC Mode of Timer 0
	// ((16000000 / 8 ) / 100000) -1 = 19
	OCR0A = 19; // 155 steps = interrupt every 10µs

	TIMSK0 |= (1<<OCIE0A); // Enable compare interrupt
	//~~~~~~~~~~~~~~~~~

	EIMSK |= (1<<INT0); // External interrupt mask register INT0 

	DDRD |= (1<<SRFout); // set triggerpin to output
	DDRD &= ~(1<<SRFin); // set INT0 as input
}

uint16_t SRF05_getDistance(void){

	uint16_t dist = 0;

	dist = 0; // reset distance to prevent adding up the distances
	microseconds = 0; // reset microseconds counter variable
	TCNT0 = 0; // se timer to 0

	// trigger the SRF05 measurement:
	PORTD |= (1<<SRFout);
	_delay_us(12); // 12µs trigger signal
	PORTD &= ~(1<<SRFout);

	// trigger on rising edge:
	EICRA = (1<<ISC01) | (1<<ISC00);

	sei(); // enable all interrupts

	while(measurement_complete != 1){
		// wait until measurement is complete
	}

	cli(); // disable all interrupts

	measurement_complete = 0; // reset 

	dist = microseconds / 58;

	return dist;
}

int main(void)
{
    init_SRF05();
    uint16_t distance = 0;

    while(1)
    {
		distance = SRF05_getDistance();

		/*

		do whatever you need to do with the distance

		*/

		_delay_ms(100); // give the SRF05 some time to settle bevore starting a new measurement
	}
}

ISR(INT0_vect){  // external interrupt 0 

	if(INT0_interrupt == 0){ // only if there hasn't been triggered on a rising edge yet
		TCCR0B |= (1<<CS01); // start Timer with prescaler 8

		// set INT0 to trigger in falling edge
		EICRA = (1<<ISC01); // EICRA = External Interrup Control Register A

		INT0_interrupt = 1; // to indicate, that an interrupt has already occured
	}
	else{ // if an INT0 interrupt has occured before
		TCCR0B &= ~(1<<CS01); // stop Timer

		INT0_interrupt = 0; // reset variable
		measurement_complete = 1; // variable to indicate end of measurement
	}
}

//Interrput every 10µs
ISR(TIMER0_COMPA_vect){
	microseconds += 10;
}