#include "main.h"
	
Ticker tick1(TIM11);
DigitalOut led4(PD12);

Ticker tick2(TIM12);
DigitalOut led6(PD15);

void blink1(void)
{
	led4 = !led4;
}

void blink2(void)
{
	led6 = !led6;
}

int main(void)
{
	Systick_Init();

	tick1.attach(&blink1, 10);
	tick2.attach(&blink2, 10);
	
  while(1)
  {
		
  }
}