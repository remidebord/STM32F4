#include "main.h"

/* Serial port config */
//Serial serial(USART1, PB7, PB6);
//Serial serial(USART1, PA10, PA9);
//Serial serial(USART2, PA3, PA2);
//Serial serial(USART3, PB11, PB10);
//Serial serial(UART4, PC11, PC10);
//Serial serial(UART5, PD2, PC12);
Serial serial(USART6, PC7, PC6);

int main(void)
{
	char buffer[255];
	int length;
	
  Systick_Init();
	
	/* Change baudrate (default: 9600) */
	serial.baud(115200);
	
  while (1)
  {
		/* echo mode */
		length = serial.read(buffer);
		
		if(length > 0)
		{
			serial.write_b(buffer, length);
		}
  }
}
