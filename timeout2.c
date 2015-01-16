#include "main.h"

TimeOut timeout(TIM6);

DigitalOut led4(PD12);
DigitalOut led6(PD15);

void Toggle(void)
{
	led4 = !led4;
}

int main(void)
{
	Systick_Init();
	
	// Set timeout value and callback
	timeout.attach(&Toggle, 1000);
	
	timeout.start();
	
  while (1)
  {
		if(timeout.end())
		{
			timeout.start();
			
			led6 = !led6;
		}
  }
}