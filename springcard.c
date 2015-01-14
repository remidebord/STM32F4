#include "main.h"
#include <string.h>

DigitalOut led1(PC9);
DigitalOut led2(PC8);

Ticker tick(TIM16);

Serial serial(USART1, PA10, PA9);
Springcard rfid(&serial);

void timer(void);

int main(void)
{
	char uid[10];
	char code[16];
	
  Systick_Init();
	
	tick.attach(&timer, 1);
	
	serial.baud(38400);
	serial.timeout(20);
	
  while (1)
  {
		rfid.update();
		
		if(rfid.card(uid, code))
		{
			led1 = !led1;
			
			if(strncmp(code, "BRVE", 4) == 0)
			{
				led2 = 1;
			}
			else
			{
				led2 = 0;
			}
		}
  }
}

void timer(void)
{
	rfid.timer();
}
