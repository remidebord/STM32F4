/*!
 * \file I2C.cpp
 * \brief I2C API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 14 janvier 2015
 *
 * Librairie I2C (400kHz).
 *
 */

#include "I2C.h"

I2C_variables I2C1_var;
I2C_variables I2C2_var;
I2C_variables I2C3_var;

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe I2C
 *
 *  \param I2Cx : I2C number (I2C1 or I2C2)
 *  \param GPIO_c_sda : data pin
 *  \param GPIO_c_scl : clock pin
 *
 */

I2C :: I2C(I2C_TypeDef* I2Cx, GPIO_common GPIO_c_sda, GPIO_common GPIO_c_scl)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	
	m_I2C = I2Cx;
	
	m_sdaPort = GPIO_c_sda.port;
	m_sdaPin = GPIO_c_sda.pin;
	m_sdaSource = GPIO_c_sda.pinSource;
	
	m_sclPort = GPIO_c_scl.port;
	m_sclPin = GPIO_c_scl.pin;
	m_sclSource = GPIO_c_scl.pinSource;
	
	/* Assign address to pointer */
	if(m_I2C == I2C1) m_var = &I2C1_var;
	else if(m_I2C == I2C2) m_var = &I2C2_var;
	else if(m_I2C == I2C3) m_var = &I2C3_var;

	/* Initialize values */
	m_var->busy = 0;
	m_var->index = 0;
	m_var->length = 0;
	
  /* I2C Peripheral clock enable */
  if(m_I2C == I2C1) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	else if(m_I2C == I2C2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	else if(m_I2C == I2C3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
  
  /* SDA/SCL GPIO clock enable */
	if(m_sdaPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_sdaPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_sdaPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_sdaPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	if(m_sclPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_sclPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_sclPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_sclPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
	/* Connect pins to I2C2 Alternate Function mapping */
	if(m_I2C == I2C1) 
	{
		GPIO_PinAFConfig(m_sdaPort, m_sdaSource, GPIO_AF_I2C1);
		GPIO_PinAFConfig(m_sclPort, m_sclSource, GPIO_AF_I2C1);
	}
	else if(m_I2C == I2C2) 
	{
		GPIO_PinAFConfig(m_sdaPort, m_sdaSource, GPIO_AF_I2C2);
		GPIO_PinAFConfig(m_sclPort, m_sclSource, GPIO_AF_I2C2);
	}
	else if(m_I2C == I2C3) 
	{
		GPIO_PinAFConfig(m_sdaPort, m_sdaSource, GPIO_AF_I2C3);
		GPIO_PinAFConfig(m_sclPort, m_sclSource, GPIO_AF_I2C3);
	}
	
	/* GPIO Configuration SDA and SCL */
  GPIO_InitStructure.GPIO_Pin = m_sdaPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(m_sdaPort, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = m_sclPin;
  GPIO_Init(m_sclPort, &GPIO_InitStructure);
	
	/* Enable I2C interrupt */
	if(m_I2C == I2C1) NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	else if(m_I2C == I2C2) NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
	else if(m_I2C == I2C3) NVIC_InitStructure.NVIC_IRQChannel = I2C3_EV_IRQn;
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	if(m_I2C == I2C1) NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
	else if(m_I2C == I2C2) NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
	else if(m_I2C == I2C3) NVIC_InitStructure.NVIC_IRQChannel = I2C3_ER_IRQn;	

	NVIC_Init(&NVIC_InitStructure);

	/* I2C De-initialize (!important)*/
  I2C_DeInit(m_I2C);
	
  /* I2C configuration */  
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;					/* I2C mode (I2C, SMBus, ...) */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	/* Standard I2C mode (Tlow/Thigh = 2) */
	I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS;/* The first device own address */
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;					/* Acknowledge enable */
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;				/* Speed (400 KHz max) */ 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* 7-bit address is acknowledged */

  /* I2C Initialization */
  I2C_Init(m_I2C, &I2C_InitStructure);
  
	/* Enable Error Interrupt */
	I2C_ITConfig(I2Cx, I2C_IT_ERR , ENABLE);
	
  /* I2C ENABLE */
  I2C_Cmd(m_I2C, ENABLE);
}

/*!
 *  \brief I2C Read (non blocking call)
 *
 *  Demande de lecture (non bloquant).
 *
 *  \param address : device address
 *  \param command : command byte(s)
 *  \param size : number of command byte 
 *
 *  \return SUCCESS or ERROR
 */

char I2C :: read(char address, char* command, char size)
{
	int timeout = size * I2C_TIMEOUT;
	char i;
	
	/* Periheral busy ? */
	if(I2C_GetFlagStatus(m_I2C, I2C_FLAG_BUSY) || m_var->busy)
	{
		return ERROR;
	}
	
	/* Initialize values */
	m_var->readWrite = I2C_READ;
	m_var->address = address;
	m_var->length = size;
	m_var->index = 0;
	m_var->busy = 1;

	/* Enable Error and Buffer Interrupts */
	I2C_ITConfig(m_I2C, (I2C_IT_EVT | I2C_IT_BUF), ENABLE);

	/* Generate the Start condition */
	I2C_GenerateSTART(m_I2C, ENABLE);
	
	while(m_var->index != size)
	{
		if((timeout--) == 0)
		{
			// Release the bus
			this->release();
			
			m_var->busy = 0;
			return ERROR;
		}
	}
	
	m_var->busy = 0;
	
	/* Copy data */
	for(i = 0; i < size; i++, command++)
	{
		*command = m_var->buffer[i];
	}
	
	return SUCCESS;
}

/*!
 *  \brief I2C Write (non blocking call)
 *
 *  Envoi de données au périphérique sélectionné (non bloquant).
 *
 *  \param address : device address
 *  \param command : command byte(s)
 *  \param size : number of command byte 
 *
 *  \return SUCCESS or ERROR
 */

char I2C :: write(char address, char* command, char size)
{
	char i = 0;
	
	/* Periheral busy ? */
	if(I2C_GetFlagStatus(m_I2C, I2C_FLAG_BUSY) || m_var->busy)
	{
		return ERROR;
	}
	
	/* Initialize values */	
	m_var->readWrite = I2C_WRITE;
	m_var->address = address;
	m_var->length = size;
	m_var->index = 0;
	m_var->busy = 1;
	
	/* Copy data to i2c buffer */
	for(i = 0; i < size; i++, command++)
	{
		m_var->buffer[i] = *command;
	}
	
	/* Enable Error and Buffer Interrupts */
	I2C_ITConfig(m_I2C, (I2C_IT_EVT | I2C_IT_BUF), ENABLE);
	
	/* Generate the Start condition */
	I2C_GenerateSTART(m_I2C, ENABLE);
	
	return SUCCESS;
}

/*!
 *  \brief I2C Write (blocking call)
 *
 *  Envoi de données au périphérique sélectionné (bloquant).
 *
 *  \param address : device address
 *  \param command : command byte(s)
 *  \param size : number of command byte 
 *
 *  \return SUCCESS or ERROR
 */
 
char I2C :: write_b(char address, char* command, char size)
{
	int timeout = size * I2C_TIMEOUT;
	char i = 0;
	
	/* Periheral busy ? */
	if(I2C_GetFlagStatus(m_I2C, I2C_FLAG_BUSY) || m_var->busy)
	{
		return ERROR;
	}
	
	/* Initialize values */
	m_var->readWrite = I2C_WRITE;
	m_var->address = address;
	m_var->length = size;
	m_var->index = 0;
	m_var->busy = 1;
	
	/* Copy data to i2c buffer */
	for(i = 0; i < size; i++, command++)
	{
		m_var->buffer[i] = *command;
	}

	/* Enable Error and Buffer Interrupts */
	I2C_ITConfig(m_I2C, (I2C_IT_EVT | I2C_IT_BUF), ENABLE);

	/* Generate the Start condition */
	I2C_GenerateSTART(m_I2C, ENABLE);
	
	while((m_var->busy) || I2C_GetFlagStatus(m_I2C, I2C_FLAG_BUSY))
	{
		if((timeout--) == 0)
		{
			// Release the bus
			this->release();
			
			m_var->busy = 0;
			return ERROR;
		}
	}
	
	return SUCCESS;
}

/*!
 *  \brief I2C bus release
 *
 *  Libère le bus de donnée (SDA et SCL à 1).
 *
 */

void I2C :: release(void)
{
	/* Send STOP condition */
	I2C_GenerateSTOP(m_I2C, ENABLE);	
}

/*!
 *  \brief I2C IRQ handler
 */

extern "C"
{
	void I2C1_EV_IRQHandler(void)
	{
		switch (I2C_GetLastEvent(I2C1))
		{
			/* EV5 */
			case I2C_EVENT_MASTER_MODE_SELECT:
				
				/* Enable acknowledgement */ 
				I2C_AcknowledgeConfig(I2C1, ENABLE);
				
				/* Send slave address for read or write */
				if(I2C1_var.readWrite == I2C_WRITE)
				{
					I2C_Send7bitAddress(I2C1, (uint8_t)I2C1_var.address, I2C_Direction_Transmitter);
				}
				else
				{
					I2C_Send7bitAddress(I2C1, (uint8_t)I2C1_var.address, I2C_Direction_Receiver);
				}
				break;

			/* EV6 */
			case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
				/* Send data */
				I2C_SendData(I2C1, I2C1_var.buffer[I2C1_var.index++]);
				break;
				
			case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
				break;

			/* EV7 */
			case I2C_EVENT_MASTER_BYTE_RECEIVED:
				/* Store data received */
				I2C1_var.buffer[I2C1_var.index] = I2C_ReceiveData(I2C1);
				
				if(I2C1_var.length == 1)
				{
					I2C1_var.busy = 0;
					
					/* Disable acknowledgement */
					I2C_AcknowledgeConfig(I2C1, DISABLE);
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C1, ENABLE);
					I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				else if(I2C1_var.index == (I2C1_var.length - 1))
				{
					/* Disable acknowledgement */
					I2C_AcknowledgeConfig(I2C1, DISABLE);
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C1, ENABLE);
				}
				else if(I2C1_var.index == I2C1_var.length)
				{
					I2C1_var.busy = 0;
					
					I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				
				I2C1_var.index++;
				break;			

			/* EV8 */
			case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
			case I2C_EVENT_MASTER_BYTE_TRANSMITTED:      
				if (I2C1_var.index == I2C1_var.length)
				{
					I2C1_var.busy = 0;
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C1, ENABLE);
					I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				else
				{
					/* Transmit data */
					I2C_SendData(I2C1, I2C1_var.buffer[I2C1_var.index++]); 
				}
				break;
			
			default:
				break;
		}
	}
	
	void I2C1_ER_IRQHandler(void)
	{
		/* Read SR1 register to get I2C error */
		if ((I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0xFF00) != 0x00)
		{
			I2C1_var.busy = 0;
			
			/* Clears errors flags */
			I2C1->SR1 &= 0x00FF;
		}
	}
	
	void I2C2_EV_IRQHandler(void)
	{
		switch (I2C_GetLastEvent(I2C2))
		{
			/* EV5 */
			case I2C_EVENT_MASTER_MODE_SELECT:
				
				/* Enable acknowledgement */ 
				I2C_AcknowledgeConfig(I2C2, ENABLE);
				
				/* Send slave address for read or write */
				if(I2C2_var.readWrite == I2C_WRITE)
				{
					I2C_Send7bitAddress(I2C2, (uint8_t)I2C2_var.address, I2C_Direction_Transmitter);
				}
				else
				{
					I2C_Send7bitAddress(I2C2, (uint8_t)I2C2_var.address, I2C_Direction_Receiver);
				}
				break;

			/* EV6 */
			case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
				/* Send data */
				I2C_SendData(I2C2, I2C2_var.buffer[I2C2_var.index++]);
				break;
				
			case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
				break;

			/* EV7 */
			case I2C_EVENT_MASTER_BYTE_RECEIVED:
				/* Store data received */
				I2C2_var.buffer[I2C2_var.index] = I2C_ReceiveData(I2C2);
				
				if(I2C2_var.length == 1)
				{
					I2C2_var.busy = 0;
					
					/* Disable acknowledgement */
					I2C_AcknowledgeConfig(I2C2, DISABLE);
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C2, ENABLE);
					I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				else if(I2C2_var.index == (I2C2_var.length - 1))
				{
					/* Disable acknowledgement */
					I2C_AcknowledgeConfig(I2C2, DISABLE);
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C2, ENABLE);
				}
				else if(I2C2_var.index == I2C2_var.length)
				{
					I2C2_var.busy = 0;
					
					I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				
				I2C2_var.index++;
				break;			

			/* EV8 */
			case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
			case I2C_EVENT_MASTER_BYTE_TRANSMITTED:      
				if (I2C2_var.index == I2C2_var.length)
				{
					I2C2_var.busy = 0;
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C2, ENABLE);
					I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				else
				{
					/* Transmit data */
					I2C_SendData(I2C2, I2C2_var.buffer[I2C2_var.index++]); 
				}
				break;
			
			default:
				break;
		}
	}
	
	void I2C2_ER_IRQHandler(void)
	{
		/* Read SR1 register to get I2C error */
		if ((I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0xFF00) != 0x00)
		{
			I2C2_var.busy = 0;
			
			/* Clears errors flags */
			I2C2->SR1 &= 0x00FF;
		}
	}
	
	void I2C3_EV_IRQHandler(void)
	{
		switch (I2C_GetLastEvent(I2C3))
		{
			/* EV5 */
			case I2C_EVENT_MASTER_MODE_SELECT:
				
				/* Enable acknowledgement */ 
				I2C_AcknowledgeConfig(I2C3, ENABLE);
				
				/* Send slave address for read or write */
				if(I2C3_var.readWrite == I2C_WRITE)
				{
					I2C_Send7bitAddress(I2C3, (uint8_t)I2C3_var.address, I2C_Direction_Transmitter);
				}
				else
				{
					I2C_Send7bitAddress(I2C3, (uint8_t)I2C3_var.address, I2C_Direction_Receiver);
				}
				break;

			/* EV6 */
			case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
				/* Send data */
				I2C_SendData(I2C3, I2C3_var.buffer[I2C3_var.index++]);
				break;
				
			case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
				break;

			/* EV7 */
			case I2C_EVENT_MASTER_BYTE_RECEIVED:
				/* Store data received */
				I2C3_var.buffer[I2C3_var.index] = I2C_ReceiveData(I2C3);
				
				if(I2C3_var.length == 1)
				{
					I2C3_var.busy = 0;
					
					/* Disable acknowledgement */
					I2C_AcknowledgeConfig(I2C3, DISABLE);
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C3, ENABLE);
					I2C_ITConfig(I2C3, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				else if(I2C3_var.index == (I2C3_var.length - 1))
				{
					/* Disable acknowledgement */
					I2C_AcknowledgeConfig(I2C3, DISABLE);
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C3, ENABLE);
				}
				else if(I2C3_var.index == I2C3_var.length)
				{
					I2C3_var.busy = 0;
					
					I2C_ITConfig(I2C3, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				
				I2C3_var.index++;
				break;			

			/* EV8 */
			case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
			case I2C_EVENT_MASTER_BYTE_TRANSMITTED:      
				if (I2C3_var.index == I2C3_var.length)
				{
					I2C3_var.busy = 0;
					
					/* Send STOP condition */
					I2C_GenerateSTOP(I2C3, ENABLE);
					I2C_ITConfig(I2C3, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				}
				else
				{
					/* Transmit data */
					I2C_SendData(I2C3, I2C3_var.buffer[I2C3_var.index++]); 
				}
				break;
			
			default:
				break;
		}
	}
	
	void I2C3_ER_IRQHandler(void)
	{
		/* Read SR1 register to get I2C error */
		if ((I2C_ReadRegister(I2C3, I2C_Register_SR1) & 0xFF00) != 0x00)
		{
			I2C3_var.busy = 0;
			
			/* Clears errors flags */
			I2C3->SR1 &= 0x00FF;
		}
	}
}
