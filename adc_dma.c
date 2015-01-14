#include "main.h"

int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;
int value5 = 0;

AnalogIn_DMA ain1(PC3); // Channel 13
AnalogIn_DMA ain2(PA5); // Channel 5
AnalogIn_DMA ain3(PA1); // Channel 1
AnalogIn_DMA ain4(PC4); // Channel 14
AnalogIn_DMA ain5(PB0); // Channel 8

int main(void)
{
	Systick_Init();
	
  while (1)
  {
		value1 = ain1.read_b();
		value2 = ain2.read_b();
		value3 = ain3.read_b();
		value4 = ain4.read_b();
		value5 = ain5.read_b();
  }
}
