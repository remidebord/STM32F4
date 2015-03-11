#ifndef __GPIO_H
#define __GPIO_H

/* includes ---------------------------------------------------------------- */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

/* defines ----------------------------------------------------------------- */
#define APB1_PRESC 4
#define APB2_PRESC 2

/* structure --------------------------------------------------------------- */
typedef struct
{
	GPIO_TypeDef* port;
	int pin;
	int pinSource;
}GPIO_common;

typedef struct
{
	TIM_TypeDef* tim;
	char channel;
	char index;
	char af;
}TIM_common;

// GPIO
extern GPIO_common PA0;
extern GPIO_common PA1;
extern GPIO_common PA2;
extern GPIO_common PA3;
extern GPIO_common PA4;
extern GPIO_common PA5;
extern GPIO_common PA6;
extern GPIO_common PA7;
extern GPIO_common PA8;
extern GPIO_common PA9;
extern GPIO_common PA10;
extern GPIO_common PA11;
extern GPIO_common PA12;
extern GPIO_common PA13;
extern GPIO_common PA14;
extern GPIO_common PA15;

extern GPIO_common PB0;
extern GPIO_common PB1;
extern GPIO_common PB2;
extern GPIO_common PB3;
extern GPIO_common PB4;
extern GPIO_common PB5;
extern GPIO_common PB6;
extern GPIO_common PB7;
extern GPIO_common PB8;
extern GPIO_common PB9;
extern GPIO_common PB10;
extern GPIO_common PB11;
extern GPIO_common PB12;
extern GPIO_common PB13;
extern GPIO_common PB14;
extern GPIO_common PB15;

extern GPIO_common PC0;
extern GPIO_common PC1;
extern GPIO_common PC2;
extern GPIO_common PC3;
extern GPIO_common PC4;
extern GPIO_common PC5;
extern GPIO_common PC6;
extern GPIO_common PC7;
extern GPIO_common PC8;
extern GPIO_common PC9;
extern GPIO_common PC10;
extern GPIO_common PC11;
extern GPIO_common PC12;
extern GPIO_common PC13;
extern GPIO_common PC14;
extern GPIO_common PC15;

extern GPIO_common PD0;
extern GPIO_common PD1;
extern GPIO_common PD2;
extern GPIO_common PD3;
extern GPIO_common PD4;
extern GPIO_common PD5;
extern GPIO_common PD6;
extern GPIO_common PD7;
extern GPIO_common PD8;
extern GPIO_common PD9;
extern GPIO_common PD10;
extern GPIO_common PD11;
extern GPIO_common PD12;
extern GPIO_common PD13;
extern GPIO_common PD14;
extern GPIO_common PD15;

extern GPIO_common PE0;
extern GPIO_common PE1;
extern GPIO_common PE2;
extern GPIO_common PE3;
extern GPIO_common PE4;
extern GPIO_common PE5;
extern GPIO_common PE6;
extern GPIO_common PE7;
extern GPIO_common PE8;
extern GPIO_common PE9;
extern GPIO_common PE10;
extern GPIO_common PE11;
extern GPIO_common PE12;
extern GPIO_common PE13;
extern GPIO_common PE14;
extern GPIO_common PE15;

extern GPIO_common PF0;
extern GPIO_common PF1;
extern GPIO_common PF2;
extern GPIO_common PF3;
extern GPIO_common PF4;
extern GPIO_common PF5;
extern GPIO_common PF6;
extern GPIO_common PF7;
extern GPIO_common PF8;
extern GPIO_common PF9;
extern GPIO_common PF10;
extern GPIO_common PF11;
extern GPIO_common PF12;
extern GPIO_common PF13;
extern GPIO_common PF14;
extern GPIO_common PF15;

extern GPIO_common PG0;
extern GPIO_common PG1;
extern GPIO_common PG2;
extern GPIO_common PG3;
extern GPIO_common PG4;
extern GPIO_common PG5;
extern GPIO_common PG6;
extern GPIO_common PG7;
extern GPIO_common PG8;
extern GPIO_common PG9;
extern GPIO_common PG10;
extern GPIO_common PG11;
extern GPIO_common PG12;
extern GPIO_common PG13;
extern GPIO_common PG14;
extern GPIO_common PG15;

// TIM
extern TIM_common TIM1_CH1;
extern TIM_common TIM1_CH2;
extern TIM_common TIM1_CH3;
extern TIM_common TIM1_CH4;

extern TIM_common TIM2_CH1;
extern TIM_common TIM2_CH2;
extern TIM_common TIM2_CH3;
extern TIM_common TIM2_CH4;

extern TIM_common TIM3_CH1;
extern TIM_common TIM3_CH2;
extern TIM_common TIM3_CH3;
extern TIM_common TIM3_CH4;

extern TIM_common TIM4_CH1;
extern TIM_common TIM4_CH2;
extern TIM_common TIM4_CH3;
extern TIM_common TIM4_CH4;

extern TIM_common TIM5_CH1;
extern TIM_common TIM5_CH2;
extern TIM_common TIM5_CH3;
extern TIM_common TIM5_CH4;

extern TIM_common TIM8_CH1;
extern TIM_common TIM8_CH2;
extern TIM_common TIM8_CH3;
extern TIM_common TIM8_CH4;

extern TIM_common TIM9_CH1;
extern TIM_common TIM9_CH2;

extern TIM_common TIM10_CH1;

extern TIM_common TIM11_CH1;

extern TIM_common TIM12_CH1;
extern TIM_common TIM12_CH2;

extern TIM_common TIM13_CH1;

extern TIM_common TIM14_CH1;

#endif
