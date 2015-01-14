#ifndef __I2C_H
#define __I2C_H

/* includes ---------------------------------------------------------------- */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "Common.h"

/* defines ----------------------------------------------------------------- */
#define I2C_OWN_ADDRESS 	0x00
#define I2C_BUF_LEN 			64
#define I2C_TIMEOUT				(SystemCoreClock / 10000)	// 0.1 ms
#define I2C_SPEED 				400000	// 400kHz

#define I2C_READ 	1
#define I2C_WRITE 0

/* structure --------------------------------------------------------------- */
typedef struct
{
	char buffer[I2C_BUF_LEN];
	char busy;
	char index;
	char length;
	char address;
	char readWrite;
}I2C_variables;

/* class ------------------------------------------------------------------- */
class I2C
{
	private:
		
		I2C_TypeDef* m_I2C;
	
		GPIO_TypeDef* m_sdaPort;
		int m_sdaPin;
		int m_sdaSource;
	
		GPIO_TypeDef* m_sclPort;
		int m_sclPin;
		int m_sclSource;
		
		I2C_variables* m_var;
		
	public:
		
		I2C(I2C_TypeDef* I2Cx, GPIO_common GPIO_c_sda, GPIO_common GPIO_c_scl);
		char read(char address, char* command, char size);
		char write(char address, char* command, char size);
		char write_b(char address, char* command, char size);
		void release(void);
};

#endif
