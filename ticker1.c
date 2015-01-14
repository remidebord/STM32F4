#include "main.h"
	
Ticker tick(TIM1);
DigitalOut led4(PD12);
DigitalOut led6(PD15);

void blink(void)
{
	led4 = !led4;
}

int main(void)
{
	Systick_Init();

	tick.attach(&blink, 1);
	//tick.attach_us(&blink, 15);
	
  while(1)
  {
		led6 = !led6;
		Delay(1);
  }
}
