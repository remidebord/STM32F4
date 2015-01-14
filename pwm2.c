#include "main.h"
	
Ticker tick(TIM6);
DigitalOut led4(PD12);
DigitalOut led6(PD15);

//PwmOut pwm(PA4, 1000, TIM14_CH1);
//PwmOut pwm(PB9, 1000, TIM17_CH1);
PwmOut pwm(PB10, 1000, TIM2_CH3);

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

	tick.attach(&blink1, 1000);
	
	pwm.enable_irq();
	pwm.attach(&blink2);	
	
	pwm = 0.5;
	
  while(1)
  {
		
  }
}