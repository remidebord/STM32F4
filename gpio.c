#include "main.h"
	
DigitalOut led4(PD12);
DigitalOut led6(PD15);
	
// DigitalIn pushButton(PA0);
InterruptIn pushButton(PA0);

void Push(void)
{
	led4 = !led4;
}

int main(void)
{
	Systick_Init();
	
	pushButton.rise(&Push);
	
  while (1)
  {
		led6 = !led6;
		Delay(100);
		
// 		if(pushButton) led2 = 1;
// 		else led2 = 0;
  }
}