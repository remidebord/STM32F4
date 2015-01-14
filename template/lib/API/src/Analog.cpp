/*!
 * \file Analog.cpp
 * \brief Analog API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 8 janvier 2015
 *
 * Librairie Analog (ADC et DAC).
 *
 */

#include "Analog.h"

int AnalogIn_DMA::m_channels[ADC_MAX_INPUT] = {0};
int AnalogIn_DMA::m_inc = 1;

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe AnalogIn (ADC).
 *
 *  \param GPIO_c : ADC pin
 *
 */

AnalogIn :: AnalogIn(GPIO_common GPIO_c)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
	
	m_adc = ADC1; // STM32F4 have 3 ADC (ADC1 default)
	m_port = GPIO_c.port;
	m_pin = GPIO_c.pin;
	m_channel = GetChannel(GPIO_c.port, GPIO_c.pin);
	m_value = 0;
	
  /* GPIOx Periph clock enable */
	if(m_port == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_port == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_port == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_port == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_port == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* ADCx clock */
	if(m_adc == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	if(m_adc == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);	
	if(m_adc == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);	
  
	/* Configure "pin" in input mode */
  GPIO_InitStructure.GPIO_Pin = m_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(m_port, &GPIO_InitStructure);
	
	/* ADCx DeInit */
  //ADC_DeInit(m_adc); 
  
	/* ADC Common Init */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
	/* ADCx Init */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(m_adc, &ADC_InitStructure);

  /* Enable ADCx */
  ADC_Cmd(m_adc, ENABLE);
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe AnalogIn (ADC), permettant de choisir l'ADC (1, 2 ou 3).
 *
 *  \param GPIO_c : ADC pin
 *
 */

AnalogIn :: AnalogIn(ADC_TypeDef* adc, GPIO_common GPIO_c)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
	
	m_adc = adc; // STM32F4 have 3 ADC
	m_port = GPIO_c.port;
	m_pin = GPIO_c.pin;
	m_channel = GetChannel(GPIO_c.port, GPIO_c.pin);
	m_value = 0;
	
  /* GPIOx Periph clock enable */
	if(m_port == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_port == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_port == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_port == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_port == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* ADCx clock */
	if(m_adc == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	if(m_adc == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);	
	if(m_adc == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);	
  
	/* Configure "pin" in input mode */
  GPIO_InitStructure.GPIO_Pin = m_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(m_port, &GPIO_InitStructure);
	
	/* ADCx DeInit */
  //ADC_DeInit(m_adc); 
  
	/* ADC Common Init */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
	/* ADCx Init */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(m_adc, &ADC_InitStructure);

  /* Enable ADCx */
  ADC_Cmd(m_adc, ENABLE);
}

/*!
 *  \brief Read ADC value
 *
 *  Conversion de la valeur de la tension en entrée.
 *
 *  \return m_value: ADC value
 */

int AnalogIn :: read()
{
	__disable_irq();

	/* Channel configuration */
	ADC_RegularChannelConfig(m_adc, m_channel, 1, ADC_SampleTime_15Cycles);

	/* Start conversion on the selected channel */
	ADC_SoftwareStartConv(m_adc);
	
	/* Wait the end of conversion (EOC) */
	while((ADC_GetFlagStatus(m_adc, ADC_FLAG_EOC) == RESET) && (ADC_GetFlagStatus(m_adc, ADC_FLAG_OVR) == RESET));
	
	if(ADC_GetFlagStatus(m_adc, ADC_FLAG_OVR) != RESET)
	{
		ADC_ClearFlag(m_adc, ADC_FLAG_OVR);
		
		__enable_irq();
		
		return 0;
	}
	else
	{
		/* Automatic stop */
	
		/* Store value */
		m_value = ADC_GetConversionValue(m_adc);
		
		__enable_irq();
		
		/* Return the converted value */
		return m_value;
	}
}

/*!
 *  \brief Read ADC value (no conversion)
 *
 *  Récupération de la dernière conversion.
 *
 *  \return m_value: ADC value
 */

AnalogIn :: operator int()
{
	return m_value;
}

/*!
 *  \brief Constructor
 *
 *  AnalogIn_DMA constructor (ADC).
 *  Need to be declared in channel order !!!
 *
 *  \param GPIO_c : ADC pin
 *
 */

__IO uint16_t value[ADC_MAX_INPUT];

AnalogIn_DMA :: AnalogIn_DMA(GPIO_common GPIO_c)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	
	char i = 0;
 
	m_port = GPIO_c.port;
	m_pin = GPIO_c.pin;
	m_adc = ADC1; // STM32F4 have 3 ADC
	m_channel = GetChannel(GPIO_c.port, GPIO_c.pin);
	m_rank = m_inc;
	
	m_channels[m_rank-1] = m_channel;
 
	GPIO_StructInit(&GPIO_InitStructure);
	ADC_StructInit(&ADC_InitStructure);
	ADC_CommonStructInit(&ADC_CommonInitStructure);
	DMA_StructInit(&DMA_InitStructure);
	
	/* Disable ADCx */
  ADC_Cmd(m_adc, DISABLE);  
	
	/* ADCx DeInit */
  ADC_DeInit();
	
	/* Disable ADC_DMA */
  ADC_DMACmd(m_adc, DISABLE);
 
  /* GPIOx Periph clock enable */
	if(m_port == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_port == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_port == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_port == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	/* ADCx clock */
	if(m_adc == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	else if(m_adc == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);	
	else if(m_adc == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);	
	
	/* DMA2 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
 
	/* Configure "pin" in input mode */
  GPIO_InitStructure.GPIO_Pin = m_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(m_port, &GPIO_InitStructure);
	
	if(m_adc == ADC1) m_dma = DMA2_Stream0;
	else if(m_adc == ADC2) m_dma = DMA2_Stream2;
	else if(m_adc == ADC3) m_dma = DMA2_Stream1;
	
	/* DMA1 Channel1 disable */
  DMA_Cmd(m_dma, DISABLE);
 
	/* DMA2 Channelx Config */
  DMA_DeInit(m_dma);
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&m_adc->DR; 	// Source address
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&value; 			// Destination address
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = m_inc; //Buffer size
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // Source size - 16bit
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 				// Destination size = 16b
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(m_dma, &DMA_InitStructure);
  
	/* DMA2 Stream0 enable */
	DMA_Cmd(m_dma, ENABLE);
 
	/* Configure the ADC1 in continous mode with a resolution equal to 12 bits  */
	ADC_DeInit();
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // Continuous conversion
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = m_inc++;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(m_adc,&ADC_InitStructure);
 
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInit(&ADC_CommonInitStructure);
 
  /* Channel(s) configuration */
	for(i = 0; i < (m_inc-1); i++)
	{
		ADC_RegularChannelConfig(m_adc, m_channels[i], (i+1), ADC_SampleTime_56Cycles);
	}
 
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* ADC1 DMA enable */
	ADC_DMACmd(ADC1, ENABLE); 
  
	/* ADC1 enable */
	ADC_Cmd(ADC1, ENABLE);
 
  /* ADCx regular Software Start Conv */
	ADC_SoftwareStartConv(ADC1);
}

/*!
 *  \brief Read ADC value (blocking)
 *
 *  Convert voltage and get result.
 *
 *  \return m_value: ADC value
 */

int AnalogIn_DMA :: read_b()
{
	/* Test DMA2 TC flag */
	while((DMA_GetFlagStatus(m_dma, DMA_FLAG_TCIF0)) == RESET ); 
    
	/* Clear DMA2 TC flag */
	DMA_ClearFlag(m_dma, DMA_FLAG_TCIF0);
    
	return m_value = value[m_rank-1];
}

/*!
 *  \brief Read ADC value
 *
 *  Convert voltage and get result.
 *
 *  \return m_value: ADC value
 */

int AnalogIn_DMA :: read()
{
	return m_value = value[m_rank-1];
}

/*!
 *  \brief Read ADC value (no conversion)
 *
 *  Get last value converted.
 *
 *  \return m_value: ADC value
 */

AnalogIn_DMA :: operator int()
{
	return m_value = value[m_rank-1];
}

/*!
 *  \brief Get ADC channel
 *
 *  Get ADC channel.
 *
 *  \return m_value: ADC value
 */

int GetChannel(GPIO_TypeDef* port, int pin)
{
	int channel = 0;
	
	if(port == GPIOA)
	{
		switch(pin)
		{
			case GPIO_Pin_0: channel = ADC_Channel_0; break;
			case GPIO_Pin_1: channel = ADC_Channel_1; break;
			case GPIO_Pin_2: channel = ADC_Channel_2; break;
			case GPIO_Pin_3: channel = ADC_Channel_3; break;
			case GPIO_Pin_4: channel = ADC_Channel_4; break;
			case GPIO_Pin_5: channel = ADC_Channel_5; break;
			case GPIO_Pin_6: channel = ADC_Channel_6; break;
			case GPIO_Pin_7: channel = ADC_Channel_7; break;
		}
	}	
	else if(port == GPIOB)
	{
		switch(pin)
		{
			case GPIO_Pin_0: channel = ADC_Channel_8; break;
			case GPIO_Pin_1: channel = ADC_Channel_9; break;
		}
	}	
	else if(port == GPIOC)
	{
		switch(pin)
		{
			case GPIO_Pin_0: channel = ADC_Channel_10; break;
			case GPIO_Pin_1: channel = ADC_Channel_11; break;
			case GPIO_Pin_2: channel = ADC_Channel_12; break;
			case GPIO_Pin_3: channel = ADC_Channel_13; break;
			case GPIO_Pin_4: channel = ADC_Channel_14; break;
			case GPIO_Pin_5: channel = ADC_Channel_15; break;
		}
	}
	else if(port == GPIOF)
	{
		channel = ADC_Channel_Vrefint; 
	}
	
	return channel;
}

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe AnalogOut (DAC).
 *
 *  \param GPIO_c : DAC pin
 *
 */

AnalogOut :: AnalogOut(GPIO_common GPIO_c)
{
	DAC_InitTypeDef    DAC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

	// STM32F4 have Only 1 DAC
	m_port = GPIO_c.port;
	m_pin = GPIO_c.pin;
	
	if((m_port == GPIOA) && (m_pin == GPIO_Pin_4)) m_channel = DAC_Channel_1;
	else if((m_port == GPIOA) && (m_pin == GPIO_Pin_5)) m_channel = DAC_Channel_2;
	
  /* GPIOx Periph clock enable */
	if(m_port == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(m_port == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(m_port == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(m_port == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(m_port == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
	/* Configure "pin" in input mode */
  GPIO_InitStructure.GPIO_Pin = m_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(m_port, &GPIO_InitStructure);

  /* Enable DAC clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
		
  /* DAC channelx Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	
  /* DAC Channelx Init */
  DAC_Init(m_channel, &DAC_InitStructure);
  
  /* Enable DAC Channelx */
  DAC_Cmd(m_channel, ENABLE);
}

/*!
 *  \brief DAC write
 *
 *  Modification du niveau de tension.
 *
 *  \param n: DAC value
 */

void AnalogOut :: write(int n)
{
	if(m_channel == DAC_Channel_1) DAC_SetChannel1Data(DAC_Align_12b_R, n);
	else if(m_channel == DAC_Channel_2) DAC_SetChannel2Data(DAC_Align_12b_R, n);
}

/*!
 *  \brief DAC write (shorthand)
 *
 *  Modification du niveau de tension.
 *
 *  \param n: DAC value
 */

AnalogOut& AnalogOut ::  operator= (int n)
{
	if(m_channel == DAC_Channel_1) DAC_SetChannel1Data(DAC_Align_12b_R, n);
	else if(m_channel == DAC_Channel_2) DAC_SetChannel2Data(DAC_Align_12b_R, n);
	return *this;
}

/*!
 *  \brief DAC read
 *
 *  Lecture du niveau de tension.
 *
 *  \return DAC value
 */

int AnalogOut :: read()
{
	return DAC_GetDataOutputValue(m_channel);
}

/*!
 *  \brief DAC read (shorthand)
 *
 *  Lecture du niveau de tension.
 *
 *  \return DAC value
 */

AnalogOut :: operator int()
{
	return DAC_GetDataOutputValue(m_channel);
}
