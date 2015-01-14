#ifndef __CAN_H
#define __CAN_H

/* includes ----------------------------------------------------------------- */
#include "stm32f4xx.h"
#include "Common.h"

/* defines ------------------------------------------------------------------ */
#define CAN_DEFAULT_BAUDRATE 		(125000) // 125 kBps

/* class -------------------------------------------------------------------- */
class CAN
{
	private:
		
		GPIO_TypeDef* m_rxPort;
		int m_rxPin;
		int m_rxPinSource;
	
		GPIO_TypeDef* m_txPort;
		int m_txPin;
		int m_txPinSource;
	
		char m_tq;
		int m_speed;
		CAN_TypeDef* m_can;
	
		CanRxMsg* m_rxMessage;
		char* m_rxFlag;
		char m_mailBox;
	
	public:
		
		CAN(CAN_TypeDef* CANx, GPIO_common GPIO_c_rx, GPIO_common GPIO_c_tx);
		void attach(void(*f)(void));
		char write(char id, char *buffer, char length);
		char write_b(char id, char *buffer, char length);
		int read(char *buffer);
};

#endif /* __CAN_H */
