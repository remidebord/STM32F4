#include "main.h"

#define FREQUENCY 1000 // 1kHz

PwmOut pwm(PA7, FREQUENCY, TIM14_CH1);
//PwmOut pwm(PC6, FREQUENCY, TIM8_CH1);
//PwmOut pwm(PA1, FREQUENCY, TIM2_CH2);
//PwmOut pwm(PE11, FREQUENCY, TIM1_CH2);
//PwmOut pwm(PA7, FREQUENCY, TIM3_CH2);

int main(void)
{
	Systick_Init();
	
	// Set duty cycle to 50%
	pwm = 0.5;
	
  while (1)
  {
  }
}
