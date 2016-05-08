//
//	eeprom.c
//
//	Michael Jean <michael.jean@shaw.ca>
//

#include <avr/interrupt.h>
#include <avr/io.h>

#include "eeprom.h"

uint8_t
eeprom_read (uint16_t address)
{
	uint8_t data = 0;

	cli ();						/* 1 */

	while (EECR & _BV (EEWE))	/* 2 */
		;

	EEAR = address;
	EECR |= _BV (EERE);

	data = EEDR;

	sei ();

	return data;
}

//
//	1.	The datasheet specifies that no interrupts may occur during the read
//		cycle, or data corruption will occur.
//
//	2.	The EEPROM is inaccessable if a write is taking place. This should
//		never happen if one accesses the EEPROM exclusively through this
//		library, but we include it for completeness' sake.
//

void
eeprom_write (uint16_t address, uint8_t	data)
{
	cli ();						/* 1 */

	while (EECR & _BV (EEWE))	/* 2 */
		;

	EEAR = address;
	EEDR = data;

	EECR |= _BV (EEMWE);		/* 3 */
	EECR |= _BV (EEWE);

	while (EECR & _BV (EEWE))
		;

	sei ();
}

//
//	1.	The datasheet specifies that no interrupts may occur during the write
//		cycle, or data corruption will occur.
//
//	2.	See note #2 for the `eeprom_read' function.
//
//	3.	To write to the EEPROM, you must first write the master write enable
//		bit, then write the write enable bit within four clock cycles.
//


void
eeprom_read_many (uint16_t address, uint8_t *data, uint8_t n)
{
	int i;

	for (i = 0; i < n; i++)
		data[i] = eeprom_read (address + i);	/* 1 */
}

//
//	1.	By calling the `eeprom_read' function over and over, we effectively
//		allow interrupts to be enabled in between calls, which means we
//		don't risk interrupt service starvation.
//

void
eeprom_write_many (uint16_t address, uint8_t *data, uint8_t n)
{
	int i;

	for (i = 0; i < n; i++)
		eeprom_write (address + i, data[i]);	/* 1 */
}

//
//	1.	See note #1 for the `eeprom_read_many' function.
//
