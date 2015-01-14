#ifndef __TIMER_H
#define __TIMER_H

/* includes ---------------------------------------------------------------- */
#include "Common.h"

/* defines ----------------------------------------------------------------- */
#define PRESCALER_LOW_FREQ (100000)
#define FREQ_LOW_MAX 1300

#define TIM_CLK1 ((SystemCoreClock / APB1_PRESC) * 2)
#define TIM_CLK2 ((SystemCoreClock / APB2_PRESC) * 2)

#define USE_APB1 1
#define USE_APB2 2

#define NULL 0

/* class ------------------------------------------------------------------- */
class Ticker
{
	private:
		
		TIM_TypeDef* m_tim;
	  char m_index;
		int m_irqVector;
	
	public:
		
		Ticker(TIM_TypeDef* TIMx);
		void attach(void(*f)(void), int ms);
		void attach_us(void(*f)(void), int us);
		void detach(void);
};

class TimeOut
{
	private:
		
		TIM_TypeDef* m_tim;
	  char m_index;
		int m_irqVector;
	
	public:
		
		TimeOut(TIM_TypeDef* TIMx);
		void attach(void(*f)(void), int ms);
		void attach_us(void(*f)(void), int us);
		void detach(void);
		void start(void);
};

class PwmOut
{
	private:
		
		GPIO_TypeDef* m_port;
		int m_pin;
		int m_pinSource;
		TIM_TypeDef* m_tim;
		int m_channel;
		char m_index;
		int m_timerPeriod;
		int m_freq;
		float m_dutyCycle;
		int m_prescaler;
	
		void prescaler(void);
		void period(void);
	
	public:
		
		PwmOut(GPIO_common GPIO_c, int freq, TIM_common PWM_c);
		void enable_irq(void);
		void attach(void(*f)(void));
		void detach(void);
		void freq(int n);							// Modify frequency
		void write(float dutyCycle);	// Write
		PwmOut& operator= (float n);	// Write (shorthand)
		float read();									// Read
		operator float();
};

#endif /* __TIMER_H */
