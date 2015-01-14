#ifndef __SERIAL_H
#define __SERIAL_H

/* includes ----------------------------------------------------------------- */
#include "stm32f4xx.h"
#include "Common.h"

/* defines ------------------------------------------------------------------ */
#define USART_DEFAULT_BAUDRATE 		(9600)
#define USART_RX_BUF_LEN					(512)
#define USART_ENDOFFRAME_RX_TIME	(10)	// 10 ms

/* structure ---------------------------------------------------------------- */
typedef struct
{
	char rxTimer;
	char index;
	char length;
	char buffer[USART_RX_BUF_LEN];
}USART_variables;

/* public variables --------------------------------------------------------- */
extern "C"
{
	extern USART_variables USART1_var;
	extern USART_variables USART2_var;
	extern USART_variables USART3_var;
	extern USART_variables UART4_var;
	extern USART_variables UART5_var;
	extern USART_variables USART6_var;
}

/* class -------------------------------------------------------------------- */
class Serial
{
	private:
		
		GPIO_TypeDef* m_rxPort;
		int m_rxPin;
		int m_rxPinSource;
	
		GPIO_TypeDef* m_txPort;
		int m_txPin;
		int m_txPinSource;
	
		int m_baudrate;
		USART_TypeDef* m_usart;
	
		char m_rxHead;
		char m_rxTail;
		char m_rxBuffer[USART_RX_BUF_LEN];
		int m_timeout;
		
		USART_variables* m_var;
	
	public:
		
		Serial(USART_TypeDef* USARTx, GPIO_common GPIO_c_rx, GPIO_common GPIO_c_tx);
		void receive(void);
		void baud(int baudrate);
		char write(char *buffer, char length);
		char write_b(char *buffer, char length);
		int read(char *buffer);
		int read_b(char* buffer);
		void timeout(char value);
};

#endif /* __SERIAL_H */
