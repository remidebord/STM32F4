/*!
 * \file CAN.cpp
 * \brief CAN API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 14 janvier 2015
 *
 * Librairie CAN.
 *
 */

#include "CAN.h"

CanRxMsg CAN1_rxMessage;
char CAN1_rxFlag;

CanRxMsg CAN2_rxMessage;
char CAN2_rxFlag;

char canAttach[2];
void (*canInterrupt[2])(void);

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe CAN
 *
 *  \param CAN : CAN number
 *  \param GPIO_c_rx : RX pin
 *  \param GPIO_c_tx : TX pin
 *
 */

CAN :: CAN(CAN_TypeDef* CANx, GPIO_common GPIO_c_rx, GPIO_common GPIO_c_tx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef	CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	m_rxPort = GPIO_c_rx.port;
	m_rxPin = GPIO_c_rx.pin;
	m_rxPinSource = GPIO_c_rx.pinSource;
	
	m_txPort = GPIO_c_tx.port;
	m_txPin = GPIO_c_tx.pin;
	m_txPinSource = GPIO_c_tx.pinSource;
	
	m_can = CANx;
	
	if(m_can == CAN1)
	{
		m_rxMessage = &CAN1_rxMessage;
		m_rxFlag = &CAN1_rxFlag;
		m_mailBox = 0;
	}
	else if(m_can == CAN2)
	{
		m_rxMessage = &CAN2_rxMessage;
		m_rxFlag = &CAN2_rxFlag;
		m_mailBox = 1;
	}
	
	/* Enable CANx clock */
	if(m_can == CAN1) RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	else if(m_can == CAN2)
	{
		/* In case you are using CAN2 only, you have to enable the CAN1 clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
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
	
	/* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = m_rxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(m_rxPort, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = m_txPin;
  GPIO_Init(m_txPort, &GPIO_InitStructure);
	
	/* Connect GPIO pin to the alternate function CANx */
	if(m_can == CAN1)
	{
		GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_CAN1);
		GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_CAN1);
	}
	else if(m_can == CAN2)
	{
		GPIO_PinAFConfig(m_txPort, m_txPinSource, GPIO_AF_CAN2);
		GPIO_PinAFConfig(m_rxPort, m_rxPinSource, GPIO_AF_CAN2);
	}
	
	/* CAN initialization */
  CAN_DeInit(m_can);
  CAN_StructInit(&CAN_InitStructure);
	
	/* CAN test (mode loopback) */
	//CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	
	/* Enable non-automatic retransmission mode */
	CAN_InitStructure.CAN_NART = ENABLE;
    
  /* CAN Baudrate = 125kBps (CAN clocked at 30 MHz) */
	/* Baudrate = ClockSpeed / (Prescaler * TimeQuantums) */
	/* Baudrate = PCLK1 / (CAN_Prescaler * (CAN_SJW + CAN_BS1 + CAN_BS2)) */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
	
	m_tq = (CAN_InitStructure.CAN_SJW + 1) + (CAN_InitStructure.CAN_BS1 + 1) + (CAN_InitStructure.CAN_BS2 + 1);
	
  CAN_InitStructure.CAN_Prescaler = ((SystemCoreClock * APB1_PRESC) / (CAN_DEFAULT_BAUDRATE * m_tq));
  CAN_Init(m_can, &CAN_InitStructure);
	
	/* CAN filter initialization */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		/* Filter mode (identifier/mask mode) */
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	/* 32 bits filter (standard identifier 11 bits) */
	
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; /* No filter */
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	
  if(m_can == CAN1)
	{
		CAN_FilterInitStructure.CAN_FilterNumber = 0;	
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; /* Use FIFO 0 */
	}
  else if(m_can == CAN2)
	{
		CAN_FilterInitStructure.CAN_FilterNumber = 14;	
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1; /* Use FIFO 1 */
	}
	
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;	/* Filter activation */
  CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* CANx IRQ configuration */
	if(m_can == CAN1) NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	else if(m_can == CAN2) NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	if(m_can == CAN1) CAN_ITConfig(m_can, CAN_IT_FMP0, ENABLE);
	else if(m_can == CAN2) CAN_ITConfig(m_can, CAN_IT_FMP1, ENABLE);
}

/*!
 *  \brief Attach a function to CAN Rx IRQ (callback)
 *
 *  Association de la fonction de rappel
 *
 *  \param void(*f)(void) : function pointer (address)
 *
 */

void CAN :: attach(void(*f)(void))
{
	/* Attribute function adress to the function pointer */	
	if(m_can == CAN1)
	{
		canInterrupt[0] = f;
		canAttach[0] = 1;
	}
	else if(m_can == CAN2)
	{
		canInterrupt[1] = f;
		canAttach[1] = 1;
	}
}

/*!
 *  \brief CAN write (non blocking call)
 *
 *  Ecriture d'un ou plusieurs caractères sur le bus (non bloquant).
 *
 *  \param identifier: identifier
 *  \param buffer: data array
 *  \param length: data length
 *  \return status (Ok, failed)
 */

char CAN :: write(char id, char *buffer, char length)
{
	CanTxMsg TxMessage;
	char i = 0;

	/* CAN busy ? */
	if(CAN_TransmitStatus(m_can, m_mailBox) == CAN_TxStatus_Pending)
	{
		return CAN_TxStatus_Pending;
	}
	
	/* CAN message */
	TxMessage.StdId = id;					/* Identifier */
	TxMessage.RTR = CAN_RTR_DATA;	/* frame type (data) */
	TxMessage.IDE = CAN_ID_STD;		/* Standard identifier */
	
	/* Data frame (64 bits) */
	if(length > 8) TxMessage.DLC = 8;
	else TxMessage.DLC = length;
	
	/* Copy data */
	for(i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = *(buffer + i);
	}

	/* Send CAN message */
	CAN_Transmit(m_can, &TxMessage);
	
	return CAN_TxStatus_Ok;
}

/*!
 *  \brief CAN write (blocking call)
 *
 *  Ecriture d'un ou plusieurs caractères sur le bus (bloquant).
 *
 *  \param identifier: identifier
 *  \param buffer: data array
 *  \param length: data length
 *  \return status (Ok, failed)
 */

char CAN :: write_b(char id, char *buffer, char length)
{
	CanTxMsg TxMessage;
	char status;
	char i = 0;
	
	/* CAN busy ? */
	if(CAN_TransmitStatus(m_can, m_mailBox) == CAN_TxStatus_Pending)
	{
		return CAN_TxStatus_Pending;
	}
	
	/* CAN message */
	TxMessage.StdId = id;					/* Identifier */
	TxMessage.RTR = CAN_RTR_DATA;	/* frame type (data) */
	TxMessage.IDE = CAN_ID_STD;		/* Standard identifier */
	
	/* Data frame (64 bits) */
	if(length > 8) TxMessage.DLC = 8;
	else TxMessage.DLC = length;
	
	/* Copy data */
	for(i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = *(buffer + i);
	}

	/* Send CAN message */
	CAN_Transmit(m_can, &TxMessage);
	
	/* Wait end of transmit */
	while((status = CAN_TransmitStatus(m_can, 0)) == CAN_TxStatus_Pending);

	return status;
}

/*!
 *  \brief CAN read
 *
 *  Lecture d'un ou plusieurs caractères.
 *
 *  \param buffer: data array (out)
 *  \return identifier
 */

int CAN :: read(char *buffer)
{
	char i;
	
	/* Message received ? */
	if(*m_rxFlag)
	{
		*m_rxFlag = 0;
		
		/* Copy data */
		for(i = 0; i < m_rxMessage->DLC; i++)
		{
			*(buffer + i) = m_rxMessage->Data[i];
		}
		
		return (int)m_rxMessage->StdId;
	}
	else
	{
		return 0;
	}
}

/*!
 *  \brief CAN IRQ handler
 */

extern "C"
{
	void CAN1_RX0_IRQHandler(void)
	{
		if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
		{
			CAN1_rxFlag = 1;
			
			/* Store data */
			CAN_Receive(CAN1, CAN_FIFO0, &CAN1_rxMessage);
			
			/* Call function */
			if(canAttach[0]) (*canInterrupt[0])();
		}
	}
	
	void CAN2_RX1_IRQHandler(void)
	{
		if(CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
		{
			CAN2_rxFlag = 1;
			
			/* Store data */
			CAN_Receive(CAN2, CAN_FIFO1, &CAN2_rxMessage);
			
			/* Call function */
			if(canAttach[1]) (*canInterrupt[1])();
		}
	}
}
