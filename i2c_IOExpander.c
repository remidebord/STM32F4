#include "main.h"

// PCA9554 address
#define PCA9554_1_ADDRESS  (0x70) 

I2C	i2c(I2C1, PB7, PB6);
// I2C	i2c(I2C2, PB11, PB10);

int main(void)
{
	char buffer[255] = "";
	char i = 0x01;
	
	Systick_Init();
	
	buffer[0] = 0x03;		// Configuration register (0x03)
	buffer[1] = 0x00;		// All IO in output (0x00)
	
	i2c.write_b(PCA9554_1_ADDRESS, buffer, 2);	
	
  while (1)
  {
		buffer[0] = 0x01;	// Output port register (0x01)
		buffer[1] = i;		// Outputs states
		
		/* Led chaser */
		if(i < 0x80) i = i << 1;
		else i = 0x01;
		
		i2c.write(PCA9554_1_ADDRESS, buffer, 2);
		
		Delay(100);
  }
}
