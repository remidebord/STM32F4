#include "main.h"

/* Serial port config */
Serial serial(USART1, PA10, PA9);

/* GPS */
GPS gps(&serial);

float latitude;
float longitude;
int time;
int date;

int main(void)
{
  Systick_Init();
	
  while (1)
  {
		gps.update();
		
		if(gps.fix())
		{
			latitude = gps.latitude();
			longitude = gps.longitude();
			time = gps.time();
			date = gps.date();
		}
  }
}