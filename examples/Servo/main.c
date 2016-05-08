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
 *   This example shows how to set a servos direction with the 16bit Timer1
 *
 *
 *	Calculations:
 *
 *	Clock cycles completed in 20ms:
 *	F_CPU / 50Hz = 16000000Hz / 50Hz = 320000
 *
 *	Prescaler and compare value:
 *	320000 / Prescaler = compare value
 * 	320000 / 8 = 40000
 *
 *	1.5ms pulse width (middle position):
 *	20ms / 15 = 40000 / 15 = 2666 = OCR1A value
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void){

	TCCR1A |= (1<<COM1A1) | (1<<WGM11); // non-inverting mode for OC1A
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); // Mode 14, Prescaler 8

	ICR1 = 40000; // 320000 / 8 = 40000

	DDRB |= (1<<PB1); // OC1A set to output 

	while(1){

		OCR1A = 2000; // set to 0° --> pulsewidth = 1ms
		_delay_ms(1000);

		OCR1A = 3000; // set to 90° --> pulsewdith = 1.5ms
		_delay_ms(1000);

		OCR1A = 4000; // set to 180° --> pulsewidth = 2ms
		_delay_ms(1000);
	}
}
