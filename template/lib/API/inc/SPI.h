#ifndef __SPI_H
#define __SPI_H

/* includes ---------------------------------------------------------------- */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"
#include "Common.h"

/* class ------------------------------------------------------------------- */
class SPI
{
	private:
		
		SPI_TypeDef* m_spi;
	
		GPIO_TypeDef* m_csPort;
		int m_csPin;
	
		GPIO_TypeDef* m_sckPort;
		int m_sckPin;
		int m_sckPinSource;
	
		GPIO_TypeDef* m_mosiPort;
		int m_mosiPin;
		int m_mosiPinSource;
	
		GPIO_TypeDef* m_misoPort;
		int m_misoPin;
		int m_misoPinSource;
	
		char m_mode;
	
	public:
		
	SPI(SPI_TypeDef* SPIx, GPIO_common GPIO_c_cs, GPIO_common GPIO_c_sck, GPIO_common GPIO_c_mosi, GPIO_common GPIO_c_miso);

	void mode(char x);
	void cs(char x);
	char write(char c);
	void write(char* buffer, int length);
	void read(char* buffer, int length);
	
};

#endif
