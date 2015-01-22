/*!
 * \file Timer.cpp
 * \brief Timer API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 12 janvier 2015
 *
 * Librairie Timer (PWM, Ticker, TimeOut).
 *
 */

#include "Timer.h"

extern "C"
{
	char timUpdateAttach[14]; // 12 + 2 basic timer
	void (*TimUpdateInterrupt[14])(void);
	
	char timCCAttach[32];		// 32 channels
	void (*TimCCInterrupt[32])(void);
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe Ticker
 *
 *  \param TIMx : Timer number
 *
 */

Ticker :: Ticker(TIM_TypeDef* TIMx)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	m_tim = TIMx;
	
  /* TIMx clock enable */
  if(m_tim == TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(m_tim == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(m_tim == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(m_tim == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(m_tim == TIM5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	else if(m_tim == TIM6) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	else if(m_tim == TIM7) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	else if(m_tim == TIM8) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	else if(m_tim == TIM9) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	else if(m_tim == TIM10) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	else if(m_tim == TIM11) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	else if(m_tim == TIM12) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	else if(m_tim == TIM13) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	else if(m_tim == TIM14) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* IRQ Vector */
  if(m_tim == TIM1) m_irqVector = TIM1_UP_TIM10_IRQn;
	else if(m_tim == TIM2) m_irqVector = TIM2_IRQn;
	else if(m_tim == TIM3) m_irqVector = TIM3_IRQn;
	else if(m_tim == TIM4) m_irqVector = TIM4_IRQn;
	else if(m_tim == TIM5) m_irqVector = TIM5_IRQn;
	else if(m_tim == TIM6) m_irqVector = TIM6_DAC_IRQn;
	else if(m_tim == TIM7) m_irqVector = TIM7_IRQn;
	else if(m_tim == TIM8) m_irqVector = TIM8_UP_TIM13_IRQn;
	else if(m_tim == TIM9) m_irqVector = TIM1_BRK_TIM9_IRQn;
	else if(m_tim == TIM10) m_irqVector = TIM1_UP_TIM10_IRQn;
	else if(m_tim == TIM11) m_irqVector = TIM1_TRG_COM_TIM11_IRQn;
	else if(m_tim == TIM12) m_irqVector = TIM8_BRK_TIM12_IRQn;
	else if(m_tim == TIM13) m_irqVector = TIM8_UP_TIM13_IRQn;
	else if(m_tim == TIM14) m_irqVector = TIM8_TRG_COM_TIM14_IRQn;
	
	/* Determine index for function pointer array and flag */
	if(m_tim == TIM1)	m_index = 0;
	else if(m_tim == TIM2) m_index = 1;
	else if(m_tim == TIM3) m_index = 2;
	else if(m_tim == TIM4) m_index = 3;
	else if(m_tim == TIM5) m_index = 4;
	else if(m_tim == TIM6) m_index = 5;
	else if(m_tim == TIM7) m_index = 6;
	else if(m_tim == TIM8) m_index = 7;
	else if(m_tim == TIM9) m_index = 8;
	else if(m_tim == TIM10) m_index = 9;
	else if(m_tim == TIM11) m_index = 10;
	else if(m_tim == TIM12) m_index = 11;
	else if(m_tim == TIM13) m_index = 12;
	else if(m_tim == TIM14) m_index = 13;
	
  /* Enable the TIMx global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = m_irqVector;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*!
 *  \brief Attach a function to ticker IRQ (callback)
 *
 *  Association de la fonction de rappel
 *
 *  \param void(*f)(void) : function pointer (address)
 *  \param ms : periodic time (ms)
 *
 */

void Ticker :: attach(void(*f)(void), int ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Attribute function adress to the function pointer */	
	TimUpdateInterrupt[m_index] = f;
	timUpdateAttach[m_index] = 1;
	
	/* Time base configuration */
  if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
	{
		TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB2_PRESC) * 2) / 10000) - 1; // 168 MHz / 10000 (100 us)
	}
  else TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB1_PRESC) * 2) / 10000) - 1; // 84 MHz / 10000 (100 us)
	
  TIM_TimeBaseStructure.TIM_Period = (ms * 10) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(m_tim, &TIM_TimeBaseStructure);
 
  /* Enable TIMx Update Interrupt */
  TIM_ITConfig(m_tim, TIM_IT_Update, ENABLE);
 
  /* Enable TIMx */
  TIM_Cmd(m_tim,ENABLE);
}

/*!
 *  \brief Attach a function to ticker IRQ (callback)
 *
 *  Association de la fonction de rappel
 *
 *  \param void(*f)(void) : function pointer (address)
 *  \param ms : periodic time (us)
 *
 */

void Ticker :: attach_us(void(*f)(void), int us)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Attribute function adress to the function pointer */	
	TimUpdateInterrupt[m_index] = f;
	timUpdateAttach[m_index] = 1;
	
	/* Time base configuration */
  if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
	{
		TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB2_PRESC) * 2) / 1000000) - 1; // 168 MHz / 1000000 (1 us)
	}
  else TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB1_PRESC) * 2) / 1000000) - 1; // 84 MHz / 1000000 (1 us)

  TIM_TimeBaseStructure.TIM_Period = us - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(m_tim, &TIM_TimeBaseStructure);
 
  /* Enable TIMx Update Interrupt */
  TIM_ITConfig(m_tim, TIM_IT_Update, ENABLE);
 
  /* Enable TIMx */
  TIM_Cmd(m_tim,ENABLE);
}

/*!
 *  \brief Detach any callback function
 *
 *  Séparation de la fonction de rappel et de l'IRQ
 *
 */

void Ticker :: detach()
{
	timUpdateAttach[m_index] = 0;
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe TimeOut
 *
 *  \param TIMx : Timer number
 *
 */

TimeOut :: TimeOut(TIM_TypeDef* TIMx)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	m_tim = TIMx;
	
  /* TIMx clock enable */
  if(m_tim == TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(m_tim == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(m_tim == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(m_tim == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(m_tim == TIM5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	else if(m_tim == TIM6) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	else if(m_tim == TIM7) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	else if(m_tim == TIM8) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	else if(m_tim == TIM9) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	else if(m_tim == TIM10) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	else if(m_tim == TIM11) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	else if(m_tim == TIM12) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	else if(m_tim == TIM13) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	else if(m_tim == TIM14) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* IRQ Vector */
  if(m_tim == TIM1) m_irqVector = TIM1_UP_TIM10_IRQn;
	else if(m_tim == TIM2) m_irqVector = TIM2_IRQn;
	else if(m_tim == TIM3) m_irqVector = TIM3_IRQn;
	else if(m_tim == TIM4) m_irqVector = TIM4_IRQn;
	else if(m_tim == TIM5) m_irqVector = TIM5_IRQn;
	else if(m_tim == TIM6) m_irqVector = TIM6_DAC_IRQn;
	else if(m_tim == TIM7) m_irqVector = TIM7_IRQn;
	else if(m_tim == TIM8) m_irqVector = TIM8_UP_TIM13_IRQn;
	else if(m_tim == TIM9) m_irqVector = TIM1_BRK_TIM9_IRQn;
	else if(m_tim == TIM10) m_irqVector = TIM1_UP_TIM10_IRQn;
	else if(m_tim == TIM11) m_irqVector = TIM1_TRG_COM_TIM11_IRQn;
	else if(m_tim == TIM12) m_irqVector = TIM8_BRK_TIM12_IRQn;
	else if(m_tim == TIM13) m_irqVector = TIM8_UP_TIM13_IRQn;
	else if(m_tim == TIM14) m_irqVector = TIM8_TRG_COM_TIM14_IRQn;
	
	/* Determine index for function pointer array and flag */
	if(m_tim == TIM1)	m_index = 0;
	else if(m_tim == TIM2) m_index = 1;
	else if(m_tim == TIM3) m_index = 2;
	else if(m_tim == TIM4) m_index = 3;
	else if(m_tim == TIM5) m_index = 4;
	else if(m_tim == TIM6) m_index = 5;
	else if(m_tim == TIM7) m_index = 6;
	else if(m_tim == TIM8) m_index = 7;
	else if(m_tim == TIM9) m_index = 8;
	else if(m_tim == TIM10) m_index = 9;
	else if(m_tim == TIM11) m_index = 10;
	else if(m_tim == TIM12) m_index = 11;
	else if(m_tim == TIM13) m_index = 12;
	else if(m_tim == TIM14) m_index = 13;
	
  /* Enable the TIMx global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = m_irqVector;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*!
 *  \brief Attach a function to timeOut IRQ (callback)
 *
 *  Association de la fonction de rappel
 *
 *  \param void(*f)(void) : function pointer (address)
 *  \param ms : periodic time (ms)
 *
 */

void TimeOut :: attach(void(*f)(void), int ms)
{	
	/* Attribute function adress to the function pointer */	
	TimUpdateInterrupt[m_index] = f;
	timUpdateAttach[m_index] = 1;
	
	this->set(ms);
}

/*!
 *  \brief Set timeout in ms
 *
 *  Modification de la valeur du compteur (ms)
 *
 *  \param ms : periodic time (ms)
 *
 */

void TimeOut :: set(int ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Time base configuration */
  if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
	{
		TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB2_PRESC) * 2) / 10000) - 1; // 168 MHz / 10000 (100 us)
	}
  else TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB1_PRESC) * 2) / 10000) - 1; // 84 MHz / 10000 (100 us)
	
  TIM_TimeBaseStructure.TIM_Period = (ms * 10) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //!!
  TIM_TimeBaseInit(m_tim, &TIM_TimeBaseStructure);
	
	TIM_SelectOnePulseMode(m_tim, TIM_OPMode_Single);
 
  /* Enable TIMx Update Interrupt */
  TIM_ITConfig(m_tim, TIM_IT_Update, ENABLE);
}

/*!
 *  \brief Attach a function to timeOut IRQ (callback)
 *
 *  Association de la fonction de rappel
 *
 *  \param void(*f)(void) : function pointer (address)
 *  \param ms : periodic time (us)
 *
 */

void TimeOut :: attach_us(void(*f)(void), int us)
{
	/* Attribute function adress to the function pointer */	
	TimUpdateInterrupt[m_index] = f;
	timUpdateAttach[m_index] = 1;
	
	this->set_us(us);
}

/*!
 *  \brief Set timeout in us
 *
 *  Modification de la valeur du compteur (us)
 *
 *  \param ms : periodic time (us)
 *
 */

void TimeOut :: set_us(int us)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Time base configuration */
  if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
	{
		TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB2_PRESC) * 2) / 1000000) - 1; // 168 MHz / 1000000 (1 us)
	}
  else TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB1_PRESC) * 2) / 1000000) - 1; // 84 MHz / 1000000 (1 us)
	
  TIM_TimeBaseStructure.TIM_Period = us - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //!!
  TIM_TimeBaseInit(m_tim, &TIM_TimeBaseStructure);
	
	TIM_SelectOnePulseMode(m_tim, TIM_OPMode_Single);
 
  /* Enable TIMx Update Interrupt */
  TIM_ITConfig(m_tim, TIM_IT_Update, ENABLE);
}

/*!
 *  \brief Detach any callback function
 *
 *  Séparation de la fonction de rappel et de l'IRQ
 *
 */

void TimeOut :: detach()
{
	timUpdateAttach[m_index] = 0;

	/* Disable TIMx */
  TIM_Cmd(m_tim,DISABLE);
}

/*!
 *  \brief Start counter
 *
 *  Lancement du compteur
 *
 */

void TimeOut :: start(void)
{
	/* Reset counter */
	TIM_SetCounter(m_tim, 0);
	
	/* Enable TIMx */
  TIM_Cmd(m_tim,ENABLE);
}

/*!
 *  \brief Timeout reach end value
 *
 *  Valeur du compteur égale à sa valeur de rechargement
 *
 */

char TimeOut :: end(void)
{
	if(m_tim->CNT == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe Timer (used a 32 bit timer like TIM2 or TIM5!)
 *
 *  \param TIMx : Timer number
 *
 */

Timer :: Timer(TIM_TypeDef* TIMx)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	m_tim = TIMx;
	
  /* TIMx clock enable */
  if(m_tim == TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(m_tim == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(m_tim == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(m_tim == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(m_tim == TIM5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	else if(m_tim == TIM6) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	else if(m_tim == TIM7) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	else if(m_tim == TIM8) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	else if(m_tim == TIM9) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	else if(m_tim == TIM10) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	else if(m_tim == TIM11) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	else if(m_tim == TIM12) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	else if(m_tim == TIM13) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	else if(m_tim == TIM14) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	/* Time base configuration */
  if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
	{
		TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB2_PRESC) * 2) / 1000000) - 1; // 168 MHz / 1000000 (1 us)
	}
  else TIM_TimeBaseStructure.TIM_Prescaler = (((SystemCoreClock / APB1_PRESC) * 2) / 1000000) - 1; // 84 MHz / 1000000 (1 us)
	
  
	// 32 bits timer ?
	if((m_tim == TIM2) || (m_tim == TIM5))
	{
		TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
	}
	else
	{
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	}
	
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //!!
  TIM_TimeBaseInit(m_tim, &TIM_TimeBaseStructure);
}

/*!
 *  \brief Start counter
 *
 *  Lancement du compteur
 *
 */

void Timer :: start(void)
{
	/* Enable TIMx */
  TIM_Cmd(m_tim, ENABLE);
}

/*!
 *  \brief Stop counter
 *
 *  Arrêt du compteur
 *
 */

void Timer :: stop(void)
{
	/* Enable TIMx */
  TIM_Cmd(m_tim, DISABLE);
}

/*!
 *  \brief Reset counter
 *
 *  Remise à zéro de la valeur du compteur
 *
 */

void Timer :: reset(void)
{
	/* Reset counter */
	TIM_SetCounter(m_tim, 0);
}

/*!
 *  \brief Get the time passed in seconds
 *
 *  Temps passé en secondes
 *
 */

int Timer :: read(void)
{
	return (this->read_ms() / 1000);
}

/*!
 *  \brief Get the time passed in mili-seconds
 *
 *  Temps passé en milli-secondes
 *
 */

int Timer :: read_ms(void)
{
	return (this->read_us() / 1000);
}

/*!
 *  \brief Get the time passed in micro-seconds
 *
 *  Temps passé en micro-secondes
 *
 */

int Timer :: read_us(void)
{
	return TIM_GetCounter(m_tim);
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe PwmOut
 *
 *  \param GPIO_c : pwm pin
 *  \param freq : pwm frequency
 *  \param PWM_c : timer
 *
 */

PwmOut :: PwmOut(GPIO_common GPIO_c, int freq, TIM_common PWM_c)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	int channelPulse = 0;
	
	m_port = GPIO_c.port;
	m_pin = GPIO_c.pin;
	m_pinSource = GPIO_c.pinSource;
	
	m_tim = PWM_c.tim;
	m_channel = PWM_c.channel;
	m_index = PWM_c.index;
	
	m_freq = freq;
	m_dutyCycle = 0;
	m_prescaler = 0;

  /* GPIOx Clocks enable */
	if(m_port == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_port == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_port == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_port == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_port == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	else if(m_port == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  
  /* GPIOx Configuration: Channel x as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = m_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(m_port, &GPIO_InitStructure);
  
  /* Alternate function */
	GPIO_PinAFConfig(m_port, m_pinSource, PWM_c.af);
	
	/* Caluclate prescaler */
	this->prescaler();

	/* Calculate pwm period */
	this->period();

  /* TIM1 clock enable */
  if(m_tim == TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(m_tim == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(m_tim == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(m_tim == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(m_tim == TIM5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	else if(m_tim == TIM8) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	else if(m_tim == TIM9) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	else if(m_tim == TIM10) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	else if(m_tim == TIM11) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	else if(m_tim == TIM12) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	else if(m_tim == TIM13) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	else if(m_tim == TIM14) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
  
  /* Time Base configuration */	
	TIM_TimeBaseStructure.TIM_Prescaler = m_prescaler - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = m_timerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(m_tim, &TIM_TimeBaseStructure);

  /* Channel x Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	/* Calculate pulse width */
	channelPulse = (((1 - m_dutyCycle)*100) * (m_timerPeriod - 1)) / 100;
  
	TIM_OCInitStructure.TIM_Pulse = channelPulse;
	
	switch(m_channel)
	{
		case TIM_Channel_1: TIM_OC1Init(m_tim, &TIM_OCInitStructure); break;
		case TIM_Channel_2: TIM_OC2Init(m_tim, &TIM_OCInitStructure); break;
		case TIM_Channel_3: TIM_OC3Init(m_tim, &TIM_OCInitStructure); break;
		case TIM_Channel_4: TIM_OC4Init(m_tim, &TIM_OCInitStructure); break;
	}

  /* TIMx counter enable */
  TIM_Cmd(m_tim, ENABLE);

  /* TIMx Main Output Enable */
  if((m_tim == TIM1) || (m_tim == TIM8))
	{
		TIM_CtrlPWMOutputs(m_tim, ENABLE);
	}
}

/*!
 *  \brief Calculate prescaler
 *
 *  Calcul de la valeur du prescaler en fonction de l'échelle et de la fréquence du bus
 *
 */

void PwmOut :: prescaler(void)
{
	/* PWM freq superior at 1.3 kHz and TIM2/TIM5 (32bit) are not used ? */
	if((m_freq < FREQ_LOW_MAX) && (m_tim != TIM2) && (m_tim != TIM5))
	{
		if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
		{
			m_prescaler = (TIM_CLK2 / PRESCALER_LOW_FREQ);
		}
		else
		{
			m_prescaler = (TIM_CLK1 / PRESCALER_LOW_FREQ);
		}
	}
	else
	{
		if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
		{
			m_prescaler = 2;
		}
		else
		{
			m_prescaler = 1;
		}
	}
}

/*!
 *  \brief Calculate period
 *
 *  Calcul de la valeur du compteur
 *
 */

void PwmOut :: period(void)
{
	if((m_tim == TIM1) || (m_tim == TIM8) || (m_tim == TIM9) || (m_tim == TIM10) || (m_tim == TIM11))
	{
		m_timerPeriod = ((TIM_CLK2 / m_prescaler) / m_freq) - 1;
	}
  else m_timerPeriod = ((TIM_CLK1 / m_prescaler) / m_freq) - 1;
}

/*!
 *  \brief Enable timer IRQ
 *
 *  Activation de l'interruption associée au timer
 *
 */

void PwmOut :: enable_irq(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	int irqVector;

	/* Disable TIMx */
  TIM_Cmd(m_tim,DISABLE);
	
	/* IRQ Vector */
  if(m_tim == TIM1) irqVector = TIM1_CC_IRQn;
	else if(m_tim == TIM2) irqVector = TIM2_IRQn;
	else if(m_tim == TIM3) irqVector = TIM3_IRQn;
	else if(m_tim == TIM4) irqVector = TIM4_IRQn;
	else if(m_tim == TIM5) irqVector = TIM5_IRQn;
	else if(m_tim == TIM8) irqVector = TIM8_CC_IRQn;
	else if(m_tim == TIM9) irqVector = TIM1_BRK_TIM9_IRQn;
	else if(m_tim == TIM10) irqVector = TIM1_UP_TIM10_IRQn;
	else if(m_tim == TIM11) irqVector = TIM1_TRG_COM_TIM11_IRQn;
	else if(m_tim == TIM12) irqVector = TIM8_BRK_TIM12_IRQn;
	else if(m_tim == TIM13) irqVector = TIM8_UP_TIM13_IRQn;
	else if(m_tim == TIM14) irqVector = TIM8_TRG_COM_TIM14_IRQn;
	
  /* Enable the TIMx global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = irqVector;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable TIMx Update Interrupt */
  switch(m_channel)
	{
		case TIM_Channel_1: TIM_ITConfig(m_tim, TIM_IT_CC1, ENABLE); break;
		case TIM_Channel_2: TIM_ITConfig(m_tim, TIM_IT_CC2, ENABLE); break;
		case TIM_Channel_3: TIM_ITConfig(m_tim, TIM_IT_CC3, ENABLE); break;
		case TIM_Channel_4: TIM_ITConfig(m_tim, TIM_IT_CC4, ENABLE); break;
	}
  
	/* Enable TIMx */
  TIM_Cmd(m_tim,ENABLE);
}

/*!
 *  \brief Attach a function to PwmOut IRQ (callback)
 *
 *  Association de la fonction de rappel
 *
 *  \param void(*f)(void) : function pointer (address)
 *
 */

void PwmOut :: attach(void(*f)(void))
{
	/* Attribute function adress to the function pointer */	
	TimCCInterrupt[m_index] = f;
	timCCAttach[m_index] = 1;
}

/*!
 *  \brief Detach any callback function
 *
 *  Séparation de la fonction de rappel et de l'IRQ
 *
 */

void PwmOut :: detach(void)
{
	timCCAttach[m_index] = 0;
}

/*!
 *  \brief Modify pwm frequency
 *
 *  Modification de la fréquence de la PWM
 *
 *  \param n: pwm frequency (Hz)
 */

void PwmOut :: freq(int n)
{	
	m_freq = n;
	
	/* Update prescaler if needed */
	this->prescaler();
	
	/* Calculate pwm period */
	this->period();
	
	TIM_PrescalerConfig(m_tim, m_prescaler - 1, TIM_PSCReloadMode_Immediate);
		
	/* Set pwm period */
	TIM_SetAutoreload(m_tim, m_timerPeriod);
	
	/* Restart counter */
	TIM_SetCounter(m_tim, 0);
	
	/* !important: Set duty cycle */
	this->write(m_dutyCycle);
}

/*!
 *  \brief Modify pwm duty cycle
 *
 *  Modification du rapport cyclique de la PWM
 *
 *  \param n: pwm duty cycle (0.01 to 1.00)
 */

void PwmOut :: write(float n)
{	
	int channelPulse;
	
	/* overflow et underflow protection */
	if(n >= 1) n = 1;
	else if(n <= 0) n = 0;
	
	/* Store duty cycle value */
	m_dutyCycle = n;
	
	/* Calculate pulse width */
	channelPulse = (((1 - m_dutyCycle)*100) * (m_timerPeriod - 1)) / 100;
	
	/* Set pulse width to the TIMx channel */
	switch(m_channel)
	{
		case TIM_Channel_1: m_tim->CCR1 = channelPulse; break;
		case TIM_Channel_2: m_tim->CCR2 = channelPulse; break;
		case TIM_Channel_3: m_tim->CCR3 = channelPulse; break;
		case TIM_Channel_4: m_tim->CCR4 = channelPulse; break;
	}
}

/*!
 *  \brief Modify pwm duty cycle (shorthand)
 *
 *  Modification du rapport de la PWM
 *
 *  \param n: pwm duty cycle (0.01 to 1.00)
 */

PwmOut& PwmOut ::  operator= (float n)
{
	this->write(n);
	
	return *this;
}

/*!
 *  \brief Read pwm duty cycle
 *
 *  Lecture du rapport cyclique de la PWM
 *
 *  \return m_dutyCycle: duty cycle (0.01 to 1.00)
 */

float PwmOut :: read()
{
	return m_dutyCycle;
}

/*!
 *  \brief Read pwm duty cycle (shorthand)
 *
 *  Lecture du rapport cyclique de la PWM
 *
 *  \return m_dutyCycle: duty cycle (0.01 to 1.00)
 */

PwmOut :: operator float()
{
	return this->read();
}

/*!
 *  \brief Timer IRQ handler
 */

extern "C"
{
	void TIM1_CC_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[0]) (*TimCCInterrupt[0])();		
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[1]) (*TimCCInterrupt[1])();		
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		}
		
		if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
		{
			/* Call function */
			if(timCCAttach[2]) (*TimCCInterrupt[2])();		
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		}
		
		if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
		{
			/* Call function */
			if(timCCAttach[3]) (*TimCCInterrupt[3])();		
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
		}
	}
	
	void TIM1_UP_TIM10_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[0]) (*TimUpdateInterrupt[0])();
			
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[9]) (*TimUpdateInterrupt[9])();
			
			TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[26]) (*TimCCInterrupt[26])();		
			
			TIM_ClearITPendingBit(TIM10, TIM_IT_CC1);
		}
	}
	
	void TIM2_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[1]) (*TimUpdateInterrupt[1])();
			
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[4]) (*TimCCInterrupt[4])();		
			
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[5]) (*TimCCInterrupt[5])();		
			
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		}
		
		if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
		{
			/* Call function */
			if(timCCAttach[6]) (*TimCCInterrupt[6])();		
			
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		}
		
		if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
		{
			/* Call function */
			if(timCCAttach[7]) (*TimCCInterrupt[7])();		
			
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		}
	}
	
	void TIM3_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[2]) (*TimUpdateInterrupt[2])();				
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[8]) (*TimCCInterrupt[8])();		
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[9]) (*TimCCInterrupt[9])();		
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		}
		
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
		{
			/* Call function */
			if(timCCAttach[10]) (*TimCCInterrupt[10])();		
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		}
		
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
		{
			/* Call function */
			if(timCCAttach[11]) (*TimCCInterrupt[11])();		
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		}
	}
	
	void TIM4_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[3]) (*TimUpdateInterrupt[3])();				
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[12]) (*TimCCInterrupt[12])();		
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[13]) (*TimCCInterrupt[13])();		
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		}
		
		if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
		{
			/* Call function */
			if(timCCAttach[14]) (*TimCCInterrupt[14])();		
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		}
		
		if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
		{
			/* Call function */
			if(timCCAttach[15]) (*TimCCInterrupt[15])();		
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		}
	}
	
	void TIM5_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[4]) (*TimUpdateInterrupt[4])();				
			
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[16]) (*TimCCInterrupt[16])();		
			
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[17]) (*TimCCInterrupt[17])();		
			
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
		}
		
		if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)
		{
			/* Call function */
			if(timCCAttach[18]) (*TimCCInterrupt[18])();		
			
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
		}
		
		if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)
		{
			/* Call function */
			if(timCCAttach[19]) (*TimCCInterrupt[19])();		
			
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
		}
	}
	
	void TIM6_DAC_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[5]) (*TimUpdateInterrupt[5])();		
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		}
	}
	
	void TIM7_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[6]) (*TimUpdateInterrupt[6])();		
			
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		}
	}
	
	void TIM8_CC_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[20]) (*TimCCInterrupt[20])();		
			
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[21]) (*TimCCInterrupt[21])();		
			
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
		}
		
		if (TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET)
		{
			/* Call function */
			if(timCCAttach[22]) (*TimCCInterrupt[22])();		
			
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
		}
		
		if (TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET)
		{
			/* Call function */
			if(timCCAttach[23]) (*TimCCInterrupt[23])();		
			
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);
		}
	}
	
	void TIM8_UP_TIM13_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[7]) (*TimUpdateInterrupt[7])();		
			
			TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM13, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[12]) (*TimUpdateInterrupt[12])();		
			
			TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM13, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[30]) (*TimCCInterrupt[30])();		
			
			TIM_ClearITPendingBit(TIM13, TIM_IT_CC1);
		}
	}
	
	void TIM1_BRK_TIM9_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[8]) (*TimUpdateInterrupt[8])();		
			
			TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[24]) (*TimCCInterrupt[24])();		
			
			TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[25]) (*TimCCInterrupt[25])();		
			
			TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);
		}
	}
	
	void TIM1_TRG_COM_TIM11_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[10]) (*TimUpdateInterrupt[10])();		
			
			TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM11, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[27]) (*TimCCInterrupt[27])();		
			
			TIM_ClearITPendingBit(TIM11, TIM_IT_CC1);
		}
	}
	
	void TIM8_BRK_TIM12_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[11]) (*TimUpdateInterrupt[11])();		
			
			TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[28]) (*TimCCInterrupt[28])();		
			
			TIM_ClearITPendingBit(TIM12, TIM_IT_CC1);
		}
		
		if (TIM_GetITStatus(TIM12, TIM_IT_CC2) != RESET)
		{
			/* Call function */
			if(timCCAttach[29]) (*TimCCInterrupt[29])();		
			
			TIM_ClearITPendingBit(TIM12, TIM_IT_CC2);
		}
	}
	
	void TIM8_TRG_COM_TIM14_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
		{
			/* Call function */
			if(timUpdateAttach[13]) (*TimUpdateInterrupt[13])();		
			
			TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		}
		
		if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET)
		{
			/* Call function */
			if(timCCAttach[31]) (*TimCCInterrupt[31])();		
			
			TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
		}
	}
}
