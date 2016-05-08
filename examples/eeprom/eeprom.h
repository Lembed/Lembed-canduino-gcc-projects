//
//	eeprom.h
//	Functions for accessing the internal EEPROM on the AT90CAN128.
//
//	Michael Jean <michael.jean@shaw.ca>
//

//
//	The EEPROM on the AT90CAN128 is 4K (4096 bytes).
//

#ifndef _EEPROM_H
#define _EEPROM_H

//
//	Read a single byte of data from EEPROM address `address'.
//
//	N.B. This function disables interrupts during it's duration.
//

uint8_t
eeprom_read
(
	uint16_t	address
);

//
//	Write a single byte `data' to EEPROM address `address'.
//
//	N.B. This function disables interrupts during it's duration.
//

void
eeprom_write
(
	uint16_t	address,
	uint8_t		data
);

//
//	Read `n' bytes from EEPROM, starting at address `address', into the buffer
//	pointed to by `data'.
//
//	N.B. This function disables interrupts during each single byte read, but
//	briefly re-enables them inbetween bytes.
//

void
eeprom_read_many
(
	uint16_t	address,
	uint8_t		*data,
	uint8_t		n
);

//
//	Write `n' bytes to EEPROM, starting at address `address', from the buffer
//	pointed to by `data'.
//
//	N.B. This function disables interrupts during each single byte written, but
//	briefly re-enables them inbetween bytes.
//

void
eeprom_write_many
(
	uint16_t	address,
	uint8_t		*data,
	uint8_t		n
);

#endif

