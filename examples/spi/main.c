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
 * This example code shows how to initialize the hardware SPI of an AVR microcontroller 
 * to operate as a Master.
 *
 * The SPI bus is one of the easiest to use because it doesn't have a fixed protocol
 *
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

/* Fit these to your controller aswell as the Makefile */ 
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK PB5 // Clock line
#define MISO PB4 // Master In Slave Out line
#define MOSI PB3 // Master Out Slave In line
#define CS PB2 // Chip select pin of the AVR; referred to in the datasheet as "SS"
#define S1 PD1 // Slave 1 Chip select line
#define S2 PD2 // Slave 2 Chip select line
/* *************************************************** */

void init_SPI(void){

	/*  set the MOSI, SCK and SS line as output
		SS needs to be set as output if this controller
		is the only Master 
	*/
	SPI_DDR |= (1<<MOSI) | (1<<SCK) | (1<<CS);
	
	/*  configure SPI 
		SPE = SPI Enable bit; always has to be set
		MSTR = Master bit; controls wether controller is Master or Slave; 1 = Master, 0 = Slave
		SPR0 = sets SPI clock to F_CPU / 16
		
		Please refer to the controller's datasheet 
		for a complete description of all available config bits
	*/
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0); 	
}

void send_SPI(uint8_t sData){
	
	SPDR = sData;
	
	while( !( SPSR & (1<<SPIF)) ){} // wait for end of transmission
}

int main(void)
{
	
	init_SPI();
	
	DDRD |= (1<<S1) | (1<<S2);
	
    while(1)
    {
		/* Here you can send any data you want to */ 
        
		DDRD &= ~(1<<S1); // set CS line of slave 1 low to notify slave of transmission start
		send_SPI(170); // send data
		DDRD |= (1<<S1); // set CS line of slave 1 high to notify slave of transmission end
		_delay_ms(50); 
		
		DDRD &= ~(1<<S2); // set CS line of slave 2 low to notify slave of transmission start
		send_SPI(123); // send data
		DDRD |= (1<<S2); // set CS line of slave 2 low to notify slave of transmission end
		_delay_ms(50);
    }
}