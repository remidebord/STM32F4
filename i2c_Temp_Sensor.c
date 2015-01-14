#include "main.h"

//  MCP9803 address
#define TEMP_SENSOR_ADDRESS  (0x90)

float temperature = 0;

//I2C	i2c(I2C1, PB7, PB6);
//I2C	i2c(I2C2, PB11, PB10);
I2C	i2c(I2C3, PC9, PA8);

int main(void)
{
	char buffer1[255] = "";
	char buffer2[255] = "";
	
  Systick_Init();

	buffer1[0] = 0x01;		// Configuration register (0x00)
	buffer1[1] = 0x60;		// ADC resolution 12 bits	
	
	i2c.write_b(TEMP_SENSOR_ADDRESS, buffer1, 2);	
	
  while (1)
  {
		buffer1[0] = 0x00;	// Temperature register (0x00)
		
		i2c.write_b(TEMP_SENSOR_ADDRESS, buffer1, 1);
		i2c.read(TEMP_SENSOR_ADDRESS, buffer2, 2);
		
		// Conversion
    temperature = buffer2[0];
    temperature += ((buffer2[1] & 0x80) >> 7)*0.5;
    temperature += ((buffer2[1] & 0x40) >> 6)*0.25;
    temperature += ((buffer2[1] & 0x20) >> 5)*0.125;
    temperature += ((buffer2[1] & 0x10) >> 4)*0.0625;
		
		Delay(100);
  }
}
