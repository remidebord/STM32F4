#include "main.h"

#define FREQUENCY 1000 // 1kHz

void SetColor(float red, float green, float blue);

PwmOut ledR(PA9, FREQUENCY, TIM1_CH2);
PwmOut ledG(PC9, FREQUENCY, TIM3_CH4);
PwmOut ledB(PA8, FREQUENCY, TIM1_CH1);

int main(void)
{
	float i = 0;
	float j = 0;
	
  Systick_Init();
	
  while (1)
  {
		// Red to green
		for(i = 0, j = 1.0; i <= 1.0; i += 0.01, j -= 0.01)
		{
			SetColor(j, i, 0);
			Delay(20);
		}
		
		// Green to blue
		for(i = 0, j = 1.0; i <= 1.0; i += 0.01, j -= 0.01)
		{
			SetColor(0, j, i);
			Delay(20);
		}
		
		// Blue to red
		for(i = 0, j = 1.0; i <= 1.0; i += 0.01, j -= 0.01)
		{
			SetColor(i, 0, j);
			Delay(20);
		}
  }
}

void SetColor(float red, float green, float blue)
{
  ledR = red;
	ledG = green;
	ledB = blue;
}
