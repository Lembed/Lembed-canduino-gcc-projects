
#include <avr/io.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


void initUart (unsigned int);
void putChar (char);
char getChar (void);
void putString (char *);


int main(void)
{
	putString("Hello world!");
}

void initUart (unsigned int baud)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (0xf & (baud >> 8));
	UBRR0L = (unsigned char) (0xff & baud);

	/*
	 * Enable Tx, Rx as an asynchronous mode
	 * Frame format : 8 data, no parity, 1 stop bits
	 * Disable all interrupts
	 */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); //0x18
	UCSR0C = 3 << UCSZ0; //0x06
}

void putChar (char data)
{
	/* Wait until the transmit buffer is empty */
	while (!(UCSR0A & (1 << UDRE0)))
		;

	UDR0 = data;
}

char getChar (void)
{
	/* Wait until the data is received */
	while (!(UCSR0A & (1 << RXC0)))
		;

	return UDR0;
}

void putString (char *str)
{
	while (*str)
		putChar(*str++);
}

