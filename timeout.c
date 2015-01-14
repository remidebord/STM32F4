#include "main.h"

TimeOut timeout(TIM14);

DigitalOut led4(PD12);
DigitalOut led6(PD15);

AnalogIn ain(ADC1, PA0);

PwmOut pwm(PB10, 1000, TIM2_CH3);

int val;

void blink1(void)
{
	led4 = !led4;
	led6 = 0;
	
	timeout.start();
}

void blink2(void)
{
	led6 = 1;
	
	val = ain.read();
}

int main(void)
{
	Systick_Init();

	pwm.enable_irq();
	pwm.attach(&blink1);	
	pwm = 0.5;
	
	timeout.attach_us(&blink2, 960);
	
  while(1)
  {
		
  }
}