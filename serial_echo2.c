#include "main.h"

Serial serial(USART2, PA3, PA2);
Ticker tick(TIM6);

int timeout = 1000;

void timer(void)
{
	if(timeout) timeout--;
}

int main(void)
{
	char buffer[255];
	int length;
	
  Systick_Init();
	
	/* Change baudrate (default: 9600) */
	serial.baud(115200);
	
	/* Ticker */
	tick.attach(&timer, 1);
	
  while (1)
  {
		/* echo mode */
		length = serial.read(buffer);
		
		if(length > 0)
		{
			serial.write(buffer, length);
		}
		
		if(timeout == 0)
		{
			timeout = 1000;
			
			serial.write((char*)"Helloooooooooooooooo!", 21);
		}
  }
}