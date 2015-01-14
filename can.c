#include "main.h"

CAN can(CAN1, PA11, PA12);
//CAN can(CAN2, PB12, PB13);

char buffer[255];
int id;

void rcv(void)
{
	id = can.read(buffer);
}

int main(void)
{
  Systick_Init();
	
	/* Callback function on Rx IRQ*/
	can.attach(&rcv);
	
  while (1)
  {
		/* Loopback mode */
		can.write_b(120, "HiWorld!", 8); 
		
		Delay(100);
  }
}
