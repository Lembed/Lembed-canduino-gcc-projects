#include<avr/io.h>
#include<util/delay.h>

//Power-HIGH:
int segDispPH[]={63,6,91,79,102,109,125,7,127,111};

//Power-LOW:
int segDispPL[]={192,249,164,176,153,146,130,248,128,144};


int main()
{
	DDRC=255;
	int i=0;

	while(1)
	{
		PORTC=segDispPL[i];
		i++;
		i%=10; //i ranges from 0~9
		_delay_ms(1000);
	}

	return 0;
}
