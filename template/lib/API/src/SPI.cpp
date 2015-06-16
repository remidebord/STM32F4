/*!
 * \file SPI.cpp
 * \brief Serial Peripheral Interface API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 14 janvier 2015
 *
 * Librairie SPI.
 *
 */

#include "SPI.h"

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe SPI
 *
 *  \param SPIx : SPI number [SPI1:SPI6)
 *  \param GPIO_c_cs : chip select pin
 *  \param GPIO_c_sck : clock pin
 *  \param GPIO_c_mosi : data out pin
 *  \param GPIO_c_miso : data in pin
 *
 */

SPI :: SPI(SPI_TypeDef* SPIx, GPIO_common GPIO_c_cs, GPIO_common GPIO_c_sck, GPIO_common GPIO_c_mosi, GPIO_common GPIO_c_miso)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
	char afSpi;
	
	m_spi = SPIx;
	
	m_csPort = GPIO_c_cs.port;
	m_csPin = GPIO_c_cs.pin;
	
	m_sckPort = GPIO_c_sck.port;
	m_sckPin = GPIO_c_sck.pin;
	m_sckPinSource = GPIO_c_sck.pinSource;
	
	m_mosiPort = GPIO_c_mosi.port;
	m_mosiPin = GPIO_c_mosi.pin;
	m_mosiPinSource = GPIO_c_mosi.pinSource;
	
	m_misoPort = GPIO_c_miso.port;
	m_misoPin = GPIO_c_miso.pin;
	m_misoPinSource = GPIO_c_miso.pinSource;
	
	m_mode = 1;

  /* GPIO Periph clock enable */
  if(m_csPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_csPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_csPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_csPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_csPort == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	else if(m_csPort == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
  if(m_sckPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_sckPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_sckPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_sckPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_sckPort == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	else if(m_sckPort == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	if(m_mosiPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_mosiPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_mosiPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_mosiPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_mosiPort == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	else if(m_mosiPort == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	if(m_misoPort == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_misoPort == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_misoPort == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_misoPort == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_misoPort == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	else if(m_misoPort == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  /* SD_SPI Periph clock enable */
  if(m_spi == SPI1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	else if(m_spi == SPI2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	else if(m_spi == SPI3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	else if(m_spi == SPI4) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	else if(m_spi == SPI5) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
	else if(m_spi == SPI6) RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);

  /* Configure SCK pin */
  GPIO_InitStructure.GPIO_Pin = m_sckPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(m_sckPort, &GPIO_InitStructure);
	
  /* Configure MOSI pin */
  GPIO_InitStructure.GPIO_Pin = m_mosiPin;
  GPIO_Init(m_mosiPort, &GPIO_InitStructure);
	
  /* Configure MISO pin */
  GPIO_InitStructure.GPIO_Pin = m_misoPin;
  GPIO_Init(m_misoPort, &GPIO_InitStructure);

  /* Configure CS pin */
  GPIO_InitStructure.GPIO_Pin = m_csPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(m_csPort, &GPIO_InitStructure);

	if(m_spi == SPI1) afSpi = GPIO_AF_SPI1;
	else if(m_spi == SPI2) afSpi = GPIO_AF_SPI2;
	else if(m_spi == SPI3) afSpi = GPIO_AF_SPI3;
	else if(m_spi == SPI4) afSpi = GPIO_AF_SPI4;
	else if(m_spi == SPI5) afSpi = GPIO_AF_SPI5;

  /* Alternate function */
  GPIO_PinAFConfig(m_sckPort, m_sckPinSource, afSpi);
	GPIO_PinAFConfig(m_mosiPort, m_mosiPinSource, afSpi);
  GPIO_PinAFConfig(m_misoPort, m_misoPinSource, afSpi);
  
  /*!< SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 		//SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 	//SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 84 / 8 = 10.5 Mhz

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //SPI_FirstBit_LSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(m_spi, &SPI_InitStructure);
  
  //SPI_RxFIFOThresholdConfig(SPI, SPI_RxFIFOThreshold_QF);
	
	/* Disable CRC */
	SPI_CalculateCRC(m_spi, DISABLE);
	
	/* Disable SPIx NSS output for master mode */
  SPI_SSOutputCmd(m_spi, DISABLE);
  
  SPI_Cmd(m_spi, ENABLE); /*!< SPI enable */
}

/*!
 *  \brief SPI mode
 *
 *  Modifie le mode de fonctionnement (RxTX = 1 ou Tx only = 0).
 *
 *  \param x: cs state
 */

void SPI :: mode(char x)
{
	m_mode = x;
}

/*!
 *  \brief Change chip select state
 *
 *  Modifie l'état du chip select (CS).
 *
 *  \param x: cs state
 */

void SPI :: cs(char x)
{
	if(x == 0) GPIO_ResetBits(m_csPort, m_csPin);
	else GPIO_SetBits(m_csPort, m_csPin);
}

/*!
 *  \brief SPI write character (blocking)
 *
 *  Envoie d'un caractère sur le bus.
 *
 *  \param c: character to send
 */

char SPI :: write(char c)
{
	SPI_I2S_SendData(m_spi, c);
	
	/* Wait until the transmit buffer is empty */
  while (SPI_I2S_GetFlagStatus(m_spi, SPI_I2S_FLAG_TXE) == RESET);
	
	if(m_mode)
	{
		/* Wait until a data is received */
		while (SPI_I2S_GetFlagStatus(m_spi, SPI_I2S_FLAG_RXNE) == RESET);
	}
	
	/* Wait until periheral is free */
  while (SPI_I2S_GetFlagStatus(m_spi, SPI_I2S_FLAG_BSY));
	
	return SPI_I2S_ReceiveData(m_spi);
}

/*!
 *  \brief SPI write buffer (blocking)
 *
 *  Envoie de plusieurs caractères sur le bus.
 *
 *  \param buffer: characters to send
 *  \param length: number of characters
 */

void SPI :: write(char* buffer, int length)
{
	int i = 0;
	
	for(i = 0; i < length; i++)
	{
		this->write(buffer[i]);
	}
}

/*!
 *  \brief SPI read (blocking)
 *
 *  Lecture de plusieurs caractères.
 *
 */

void SPI :: read(char* buffer, int length)
{
	int i = 0;
	
	for(i = 0; i < length; i++)
	{
		buffer[i] = this->write(0xFF);
	}
}
