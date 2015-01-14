#include "main.h"

#define RTC_ADDRESS	(0xA2) // PCF8593

char seconds = 0;
char minutes = 0;
char hours = 0;
char day = 0;
char month = 0;
char year = 0;

I2C	i2c(I2C1, PB7, PB6);
// I2C	i2c(I2C2, PB11, PB10);

int main(void)
{
	char buffer1[255] = "";
	char buffer2[255] = "";
	
	Systick_Init();

	buffer1[0] = 0x00;	// Control/Status Register
	buffer1[1] = 0x80;	// Disable counting
	buffer1[2] = 0x00;	// 0 (cents)
	buffer1[3] = 0x00;	// 0 (seconds)
	buffer1[4] = 0x48;	// 48 (minutes)
	buffer1[5] = 0x09;	// 9 (hours)
	buffer1[6] = 0x24;	// years/date
	buffer1[7] = 0x08;	// weekdays/months
	
	i2c.write_b(RTC_ADDRESS, buffer1, 8);

	buffer1[0] = 0x00;	// Control/Status Register
	buffer1[1] = 0x00;	// Enable counting
	
	i2c.write_b(RTC_ADDRESS, buffer1, 2);
	
  while (1)
  {
		buffer1[0] = 0x02;
		
		i2c.write_b(RTC_ADDRESS, buffer1, 1);
		i2c.read(RTC_ADDRESS, buffer2, 5);
		
		seconds  =  ((buffer2[0] & 0xF0) >> 4)*10 + (buffer2[0] & 0x0F);
		minutes  =  ((buffer2[1] & 0xF0) >> 4)*10 + (buffer2[1] & 0x0F);
		hours    =  ((buffer2[2] & 0x30) >> 4)*10 + (buffer2[2] & 0x0F);
		year     =   (buffer2[3] & 0xC0) >> 6;
		day      =  ((buffer2[3] & 0x30) >> 4)*10 + (buffer2[3] & 0x0F);
		month    =  ((buffer2[4] & 0x10) >> 4)*10 + (buffer2[4] & 0x0F);
		
		Delay(100);
  }
}
