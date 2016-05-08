 /*
 * 	Author: Torrentula
 * 	Website: torrentula.funpic.de
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
 * TWI master library used (twimaster.c) is by P. Fleury and can be found here:
 * http://homepage.hispeed.ch/peterfleury/avr-software.html
 *
 *
 */
 
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include "twimaster.c"

#define HMC5883L_WRITE 0x3C // write address
#define HMC5883L_READ 0x3D // read address

int16_t raw_x = 0;
int16_t raw_y = 0;
int16_t raw_z = 0;

void init_HMC5883L(void){

	i2c_start(HMC5883L_WRITE);
	i2c_write(0x00); // set pointer to CRA
	i2c_write(0x70); // write 0x70 to CRA
	i2c_stop();

	i2c_start(HMC5883L_WRITE);
	i2c_write(0x01); // set pointer to CRB
	i2c_write(0xA0);
	i2c_stop();

	i2c_start(HMC5883L_WRITE);
	i2c_write(0x02); // set pointer to measurement mode
	i2c_write(0x00); // continous measurement
	i2c_stop();
}

void getHeading(void){

	i2c_start_wait(HMC5883L_WRITE);
	i2c_write(0x03); //set pointer to X-axis MSB
	i2c_stop();

	i2c_rep_start(HMC5883L_READ); 

	raw_x = ((uint8_t)i2c_readAck())<<8;
	raw_x |= i2c_readAck();

	raw_z = ((uint8_t)i2c_readAck())<<8;
	raw_z |= i2c_readAck();

	raw_y = ((uint8_t)i2c_readAck())<<8;
	raw_y |= i2c_readNak();

	i2c_stop();
}

int main(void){

	i2c_init();
	init_HMC5883L();

	while(1){

		getHeading();

                // convert the raw data into a heading in degrees
		float headingDegrees = atan2((double)raw_y,(double)raw_x)* 180 / 3.14159265 + 180; 

		/*
                Here you can do something with the heading data you just acquired
                */
	}
} 