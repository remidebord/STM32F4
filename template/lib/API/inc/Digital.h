#ifndef __DIGITAL_H
#define __DIGITAL_H

/* includes ---------------------------------------------------------------- */
#include "stm32f4xx_gpio.h"
#include "Common.h"

/* class ------------------------------------------------------------------- */
class DigitalIn
{
	private:
		
		GPIO_TypeDef* m_port;
		int m_pin;
		int m_value;
	
	public:
		
		DigitalIn(GPIO_common GPIO_c);
		void mode(GPIOPuPd_TypeDef pull);	// Set input pin mode (GPIO_PuPd_UP, GPIO_PuPd_DOWN, GPIO_PuPd_NOPULL)
		int read();												// Read output state
		operator int();										// Read output state (shorthand)
};

class DigitalOut
{
	private:
		
		GPIO_TypeDef* m_port;
		int m_pin;
		int m_value;
	
	public:
		
		DigitalOut(GPIO_common GPIO_c);
		void write(int n);							// Write on the output
		int read();											// Read output state
		DigitalOut& operator= (int n);	// Write on the output (shorthand)
		operator int();									// Read output state (shorthand)
};

class InterruptIn
{
	private:
		
		GPIO_TypeDef* m_port;
		int m_pin;
		int m_pinSource;
		int m_irqVector;
	
	public:
		
		InterruptIn(GPIO_common GPIO_c);
		void rise(void(*f)(void));
		void fall(void(*f)(void));
		void risefall(void(*f)(void));
		void mode(GPIOPuPd_TypeDef pull);
};

#endif
