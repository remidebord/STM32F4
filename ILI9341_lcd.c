#include "main.h"

// SPI config (CS, SCK, MOSI, MISO)
SPI spi(SPI5, PC2, PF7, PF9, PF8);

// LCD (DC, RST)
LCD lcd(&spi, PD13, PG13);

int main(void)
{
	Systick_Init();
	
	lcd.init();
	
	Delay(10);
	
	lcd.rect(0, 0, 240, 320, BLACK, 1);	
	lcd.line(0, 0, 120, 120, WHITE);	
	lcd.rect(10, 10, 30, 30, RED, 0);	
	lcd.circle(100, 60, 50, BLUE);
	
	lcd.rotate(2);
	
	lcd.line(0, 0, 120, 120, WHITE);
	lcd.rect(10, 10, 30, 30, RED, 0);
	lcd.circle(100, 60, 50, BLUE);
	
	lcd.txt((char*)"Hello world!\0", 12);
	
	lcd.cursor(0, 50);
	
	lcd.txt((char*)"Hello world!\0", 12, 2, WHITE, RED);
	
  while (1);
}