/*
 * 	Author: Torrentula
 *  Website: torrentula.funpic.de
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
 *	This example shows how to fill several latches
 *  (maximum = 4 latches) with data using only one
 *	Port.
 *
 *	This is handy if you have limited pins but want to drive
 *  more outputs.
 *
 *  Theoretically you could be driving 32 LEDs with an
 *	ATtiny2313 which has only got 8 pins on PORTB.
 *
 *	These aliases are defined:
 *
 *	LatchX  --> Latch Enable line of latch X
 *	OutputEnableX --> Output Enable line of latch X
 *	EnableDDR --> Direction register for OE and LE lines
 *	EnablePORT --> Port for OE and LE line
 *	DataDDR --> Direction register for data output
 *	DataPORT --> Port for data output
 *
 */ 

#ifndef F_CPU
#define  F_CPU 1000000UL // set to your frequency
#endif

#include <avr/io.h>
#include <util/delay.h>

//############################################
//############ Define latch pins #############
//############################################

// update lines
#define Latch1 0
#define Latch2 1
#define Latch3 2
#define Latch4 3

// output enable lines
#define OutputEnable1 4
#define OutputEnable2 5
#define OutputEnable3 6
#define OutputEnable4 7

// define output registers for update & enable lines

#define EnableDDR  DDRD
#define EnablePORT PORTD
#define DataDDR    DDRB
#define DataPORT   PORTB
//############################################

void update(uint8_t latch){

	EnablePORT |= (1<<latch);    // set pin from "latch" high to begin update
	_delay_ms(10);			     // buy the latch some time to save data
	EnablePORT &= ~(1<<latch);   // set pin from "latch" low to end update
}

void init_latch(void){

	EnableDDR = 0xFF;  // output enable lines & update lines for latches are connected to EnablePORT pins --> set as output
	DataDDR = 0xFF;  // set DataPORT as output
	DataPORT = 0x00; // set all pins of DataPORT low --> latch will be saving all pins low (0b00000000)

	for(uint8_t i = 0; i < 4; i++){ // update all latches
		update(i);
	}
}

void enable(uint8_t pin){

	EnablePORT &= ~(1<<pin); // set pin from "pin" low --> enable output
}

void disable(uint8_t pin){

	EnablePORT |= (1<<pin); // set pin from "pin" high --> disable output
}

int main(void)
{
	init_latch();

    while(1)
    {
        DataPORT = 0b10101010;
		update(Latch1);
		enable(OutputEnable1);

		DataPORT = 0b11011011;
		update(Latch2);
		enable(OutputEnable2);

		// Now other things can be done with DataPORT

		_delay_ms(1000);

		DataPORT = 0b00100100;
		update(Latch1);

		DataPORT = 0b11000011;
		update(Latch2);

		_delay_ms(1000);
    }
}