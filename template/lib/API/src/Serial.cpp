/*!
 * \file Serial.cpp
 * \brief USART API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 13 janvier 2015
 *
 * Librairie USART/UART.
 *
 */

#include "Serial.h"
#include <stdlib.h>

extern "C"
{
	Serial *serial1;
	Serial *serial2;
	Serial *serial3;
	Serial *serial4;
	Serial *serial5;
	Serial *serial6;

	USART_variables USART1_var;
	USART_variables USART2_var;
	USART_variables USART3_var;
	USART_variables UART4_var;
	USART_variables UART5_var;
	USART_variables USART6_var;
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe Serial
 *
 *  \param USARTx : USART number
 *  \param GPIO_c_rx : RX pin
 *  \param GPIO_c_tx : TX pin
 *
 */

Serial :: Serial(USART_TypeDef* USARTx, GPIO_common GPIO_c_rx, GPIO_common GPIO_c_tx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	m_rxPort = GPIO_c_rx.port;
	m_rxPin = GPIO_c_rx.pin;
	m_rxPinSource = GPIO_c_rx.pinSource;
	
	m_txPort = GPIO_c_tx.port;
	m_txPin = GPIO_c_tx.pin;
	m_txPinSource = GPIO_c_tx.pinSource;
	
	m_baudrate = USART_DEFAULT_BAUDRATE;
	m_usart = USARTx;
	
	m_timeout = USART_ENDOFFRAME_RX_TIME;
	
	if(m_usart == USART1)
	{
		serial1 = this;
		m_var = &USART1_var;
	}
	else if(m_usart == USART2)
	{
		serial2 = this;
		m_var = &USART2_var;
	}
	else if(m_usart == USART3)
	{
		serial3 = this;
		m_var = &USART3_var;
	}
	else if(m_usart == UART4)
	{
		serial4 = this;
		m_var = &UART4_var;
	}
	else if(m_usart == UART5)
	{
		serial5 = this;
		m_var = &UART5_var;
	}
	else if(m_usart == USART6)
	{
		serial6 = this;
		m_var = &USART6_var;
	}
	
  /* Enable GPIO clock */
	if(m_rxPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_rxPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_rxPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_rxPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	if(m_txPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_txPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_txPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_txPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* Enable USART clock */
  if(m_usart == USART1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	else if(m_usart == USART2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	else if(m_usart == USART3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	else if(m_usart == UART4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	else if(m_usart == UART5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	else if(m_usart == USART6) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  /* Connect USART pins to alternate function */
  if(m_usart == USART1) GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_USART1);
	else if(m_usart == USART2) GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_USART2);
	else if(m_usart == USART3) GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_USART3);
	else if(m_usart == UART4) GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_UART4);
	else if(m_usart == UART5) GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_UART5);
	else if(m_usart == USART6) GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_USART6);
	
	if(m_usart == USART1) GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_USART1);
	else if(m_usart == USART2) GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_USART2);
	else if(m_usart == USART3) GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_USART3);
	else if(m_usart == UART4) GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_UART4);
	else if(m_usart == UART5) GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_UART5);
	else if(m_usart == USART6) GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_USART6);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = m_rxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(m_rxPort, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = m_txPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(m_txPort, &GPIO_InitStructure);
	
  /* Enable the USARTx Interrupt */
  if(m_usart == USART1) NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	else if(m_usart == USART2) NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	else if(m_usart == USART3) NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	else if(m_usart == UART4) NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	else if(m_usart == UART5) NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	else if(m_usart == USART6) NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* USARTx configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	
  USART_InitStructure.USART_BaudRate = m_baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(m_usart, &USART_InitStructure);
	
  /* Enable USART RX interrupt */
  USART_ITConfig(m_usart, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(m_usart, ENABLE);
}

/*!
 *  \brief Change baudrate
 *
 *  Modifie la vitesse de la liaison série.
 *
 *  \param baudrate: usar speed
 */

void Serial :: baud(int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	m_baudrate = baudrate;
	
	/* Disable USART ! */
	USART_Cmd(m_usart, DISABLE);
	
	USART_InitStructure.USART_BaudRate = m_baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(m_usart, &USART_InitStructure);
	
	USART_Cmd(m_usart, ENABLE);
}

/*!
 *  \brief Receive character
 *
 *  Réception et placement du caractère dans un tableau (A placer dans l'IRQ RX).
 *
 */

void Serial :: receive(void)
{
	m_rxBuffer[m_rxHead++] = USART_ReceiveData(m_usart);
			
	/* To avoid buffer overflow */
	if(m_rxHead >= USART_RX_BUF_LEN)
	{
		m_rxHead = 0;
	}
	
	/* Reload timer */
	m_var->rxTimer = m_timeout;
}

/*!
 *  \brief USART write (non blocking call)
 *
 *  Ecriture d'un ou plusieurs caractères sur le bus (non bloquant).
 *
 *  \param buffer: data array
 *  \param length: data length
 */

char Serial :: write(char *buffer, char length)
{
	char i;
	
	if(m_var->length != 0) return 0;
	
	m_var->length = length;
	m_var->index = 0;
	
	/* Copy data to USART buffer */
	for(i = 0; i < length; i++, buffer++)
	{
		m_var->buffer[i] = *buffer;
	}
	
	USART_ITConfig(m_usart, USART_IT_TXE, ENABLE);
	
	return 1;
}

/*!
 *  \brief USART write (blocking call)
 *
 *  Ecriture d'un ou plusieurs caractères sur le bus (bloquant).
 *
 *  \param buffer: data array
 *  \param length: data length
 */

char Serial :: write_b(char *buffer, char length)
{
	char i;
	
	if(m_var->length != 0) return 0;
	
	m_var->length = length;
	m_var->index = 0;
	
	/* Copy data to USART buffer */
	for(i = 0; i < length; i++, buffer++)
	{
		m_var->buffer[i] = *buffer;
	}
	
	USART_ITConfig(m_usart, USART_IT_TXE, ENABLE);
	
	while(m_var->length != 0);
	
	return 1;
}

/*!
 *  \brief USART read
 *
 *  Lecture d'un ou plusieurs caractères.
 *
 *  \param buffer: data array (out)
 *  \return length
 */

int Serial :: read(char *buffer)
{
	int i = 0;
	int length = 0;
	
	if(m_var->rxTimer != 0)
	{
		return 0;
	}
	else
	{
		/* Reload timer */
		m_var->rxTimer = m_timeout;
		
		/* Get head and tail position */
		if(m_rxHead >= m_rxTail) length = m_rxHead - m_rxTail;
		else length = m_rxHead + (USART_RX_BUF_LEN - m_rxTail);
		
		/* head != tail */
		if(m_rxHead == m_rxTail) return 0;
		else
		{			
			for(i = 0; i < length; i++, buffer++)
			{
				*buffer = m_rxBuffer[m_rxTail++];
				
				if(m_rxTail >= USART_RX_BUF_LEN)
				{
					m_rxTail = 0;
				}
			}
				
			return length;
		}
	}
}

/*!
 *  \brief USART read
 *
 *  Read data (blocking).
 *
 *  \param buffer: data array (out)
 *  \return length
 */

int Serial :: read_b(char* buffer)
{
	int length = 0;
	
	while(length == 0)
	{
		length = this->read(buffer);
	}
	
	return length;
}

/*!
 *  \brief Max delay between two bytes
 *
 *  Timeout between two bytes.
 *
 *  \param value: delay in ms
 */

void Serial :: timeout(char value)
{
	m_timeout = value;
}

/*!
 *  \brief USART IRQ handler
 */

extern "C"
{
	void USART1_IRQHandler(void)
	{
		if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
		{   
			USART_SendData(USART1, USART1_var.buffer[USART1_var.index++]);

			if(USART1_var.index == USART1_var.length)
			{
				USART1_var.length = 0;
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			}
		}

		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			serial1->receive();
		}
	}
	
	void USART2_IRQHandler(void)
	{
		if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
		{   
			USART_SendData(USART2, USART2_var.buffer[USART2_var.index++]);

			if(USART2_var.index == USART2_var.length)
			{
				USART2_var.length = 0;
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			}
		}
		
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
			serial2->receive();
		}
	}
	
	void USART3_IRQHandler(void)
	{
		if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
		{ 
			USART_SendData(USART3, USART3_var.buffer[USART3_var.index++]);

			if(USART3_var.index == USART3_var.length)
			{
				USART3_var.length = 0;
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			}
		}
		
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
			serial3->receive();
		}
	}
	
	void UART4_IRQHandler(void)
	{
		if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
		{   
			USART_SendData(UART4, UART4_var.buffer[UART4_var.index++]);

			if(UART4_var.index == UART4_var.length)
			{
				UART4_var.length = 0;
				USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
			}
		}
		
		if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
		{
			serial4->receive();
		}
	}
	
	void UART5_IRQHandler(void)
	{
		if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
		{   
			USART_SendData(UART5, UART5_var.buffer[UART5_var.index++]);

			if(UART5_var.index == UART5_var.length)
			{
				UART5_var.length = 0;
				USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
			}
		}
		
		if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
		{
			serial5->receive();
		}
	}
	
	void USART6_IRQHandler(void)
	{
		if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
		{
			USART_SendData(USART6, USART6_var.buffer[USART6_var.index++]);

			if(USART6_var.index == USART6_var.length)
			{
				USART6_var.length = 0;
				USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
			}
		}
		
		if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
		{
			serial6->receive();
		}
	}
}
