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
 * This example code can be used to control up to 8 servos via UART.
 * The PWM signal to drive the servos is created in software and the 
 * compare values are retrieved from a lookup table.
 *
 * You only have to fit the definitions of Servo_DDR and Servo_PORT 
 * to your needs and change the controller definition inside the Makefile.
 *
 * Have fun with this little gem :) 
 *
 */

 #ifndef F_CPU
 #define F_CPU 16000000UL
 #endif
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

// You only have to change theset two values to get it working
#define Servo_DDR DDRC
#define Servo_PORT PORTC

// Don't change anything here, from now on everything I have already taken care of for you
#define F_PWM 50
#define Prescaler 8
#define ICR1_val ((F_CPU / F_PWM) / Prescaler) - 1

volatile uint8_t tmpval[4];
volatile uint8_t val;
volatile uint16_t PWM_setting[8];

/* The lookup table contains the compare values for the software PWM 
 * and maps the received value in degrees to the corresponding compare value
 * the lookup table was calculated using the following function:
 * f(angle) = angle * 3450 / 180 + 1050
 *
 * I created a list of values with the help of WolframAlpha using following input:
 * Table[Round[x * 3450 / 180 +1050], {x, 0, 180}]
 */
const uint16_t pwm_lookup[181] = {
	
	1050, 1069, 1088, 1108, 1127, 1146, 1165, 1184, 1203, 1223,
	1242, 1261, 1280, 1299, 1381, 1338, 1357, 1376, 1395, 1414,
	1433, 1453, 1472, 1491, 1510, 1529, 1548, 1568, 1587, 1606,
	1625, 1644, 1663, 1683, 1702, 1721, 1740, 1759, 1778, 1798,
	1817, 1836, 1855, 1874, 1893, 1913, 1932, 1951, 1970, 1989,
	2008, 2028, 2047, 2066, 2085, 2104, 2123, 2143, 2162, 2181,
	2200, 2219, 2238, 2258, 2277, 2296, 2315, 2334, 2353, 2373,
	2392, 2411, 2430, 2449, 2468, 2488, 2507, 2526, 2545, 2564,
	2583, 2603, 2622, 2641, 2660, 2679, 2698, 2718, 2737, 2756,
	2775, 2794, 2813, 2833, 2852, 2871, 2890, 2909, 2928, 2948,
	2967, 2986, 3005, 3024, 3043, 3063, 3082, 3101, 3120, 3139,
	3158, 3178, 3197, 3216, 3235, 3254, 3273, 3293, 3312, 3331,
	3350, 3369, 3388, 3408, 3427, 3446, 3465, 3484, 3503, 3523,
	3542, 3561, 3580, 3599, 3618, 3638, 3657, 3676, 3695, 3714,
	3733, 3753, 3772, 3791, 3810, 3829, 3848, 3868, 3887, 3906, 
	3925, 3944, 3963, 3983, 4002, 4021, 4040, 4059, 4078, 4098,
	4117, 4136, 4155, 4174, 4193, 4213, 4232, 4251, 4270, 4289, 
	4308, 4328, 4347, 4366, 4385, 4404, 4423, 4443, 4462, 4481,
	4500 
};

void init_uart(void){

	// 9600 baud 
	UBRR0H = 0;
	UBRR0L = 103; 
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // TX / RX / RX interrupt enabled
	UCSR0C = (1<<USBS0) | (3<<UCSZ00); // 8N1
}

void uart_putc(char c){

	while(!( UCSR0A & (1<<UDRE0) )){} // wait until sending is possible
	UDR0 = c;
}

void uart_puts(char *s){

	while(*s){
		uart_putc(*s);
		s++;
	}
}

int main(void){
	
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11); // CTC mode prescaler 8
	
	ICR1 = ICR1_val; // TOP value for counter
	
	Servo_DDR = 0xFF; 
	
	uint8_t tmp;
	
	PWM_setting[0] = pwm_lookup[90];
	PWM_setting[1] = pwm_lookup[90];
	
	for(uint8_t i = 0; i < 8; i++){
		PWM_setting[i] = pwm_lookup[90];
	}

	init_uart();
	uart_puts("UART servo controller by Torrentula\r\n");
		
	sei(); // interrupts globally enabled

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
			
		Servo_PORT = tmp;

	}
	
	return 0;
}
	
ISR(USART_RX_vect){

	static uint8_t byte = 0;
	unsigned char t; 
	
	t = UDR0;
	
	if( (byte < 4) && (t != '\n') ){		
		tmpval[byte] = t;
		byte++;
	}
	else{
		// convert ASCII to decimal
		tmpval[0] -= 48; // hundred
		tmpval[1] -= 48; // ten
		tmpval[2] -= 48; // one
		tmpval[3] -= 48; // channel
		// convert single digits to number
		val = tmpval[0] * 100;
		val += tmpval[1] * 10;
		val += tmpval[2];
						
		PWM_setting[tmpval[3]] = pwm_lookup[val]; // last digit sets channel
		byte = 0; // reset counter
	}
}	

