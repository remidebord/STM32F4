#include "main.h"
#include <string.h>

#define EEPROM_ADDRESS	(0xA0) // 24LC16

#define WRITE

I2C i2c(I2C2, PF7, PF6);

int main(void)
{
	char buffer[255] = "";
	
	Systick_Init();
	
#ifdef WRITE
	// Memory address
	buffer[0] = 0x00;
	
	// Data to store
	strcpy(buffer+1, "Hello World!");
	
	// Store data
	i2c.write_b(EEPROM_ADDRESS, buffer, 13);
#else
	// Memory address
	buffer[0] = 0x00;
	
	i2c.write_b(EEPROM_ADDRESS, buffer, 1);

	// Read eeprom
	i2c.read(EEPROM_ADDRESS, buffer, 12);
#endif

	Delay(1000);
}
