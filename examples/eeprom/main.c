
#include "eeprom.h"

int main(void)
{
	eeprom_write(0, 0x01);
	eeprom_read(0);
}

