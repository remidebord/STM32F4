/*!
 * \file Average.cpp
 * \brief Average library.
 * \author R�mi.Debord
 * \version 1.0
 * \date 14 octobre 2013
 *
 * La classe average permet en plus de faire la moyenne des �chantillons ins�r�s,
 * de r�cup�rer le maximum, le minimum, ainsi que la valeur moyenne
 *
 */

#include "Average.h"
//#include <stdlib.h>

/*!
 *  \brief Constructor
 *
 *  Constructeur de la classe Average.
 *
 *  \param maxSamples : max samples number 
 */

Average :: Average(int maxSamples)
{
	// Dynamic allocation (with memory init to 0 != malloc) 
	//m_values = (int*) calloc(maxSamples, sizeof(int));
	
	m_maxSamples = maxSamples;
	m_samples = 0;
	m_maximum = 0;
}

/*!
 *  \brief insert a sample in the array
 *
 *  Insertion d'une valeur dans le tableau.
 *
 *  \param value : sample value 
 */

void Average :: insert(int value)
{
	m_values[m_samples] = value;
	m_samples = (m_samples + 1) % m_maxSamples;
}


/*!
 *  \brief Get average value of the array
 *
 *  Retourne la valeur moyenne du tableau.
 *
 *  \return average : average value
 */

int Average :: get(void)
{
	int average = 0;
	int i = 0;
	
	for(i = 0; i < m_maxSamples; i++)
	{
		average += m_values[i];
	}
	
	average = average / m_maxSamples;
	
	return average;
}

/*!
 *  \brief Fill array with one value
 *
 *  Remplit le tableau avec la valeur pass�e en param�tre.
 *
 * \param value : sample value
 */

void Average :: fill_array(int value)
{
	int i;
	
	for(i = 0; i < m_maxSamples; i++)
	{
		m_values[i] = value;
	}
}

/*!
 *  \brief Clear average array
 *
 *  Remet � z�ro tous les �l�ments du tableau.
 *
 */

void Average :: clear(void)
{
	this->fill_array(0);
	
	m_samples = 0;
}


/*!
 *  \brief Array size
 *
 *  Retourne le nombre d'�l�ments contenu dans le tableau.
 *
 *	\return m_maxSamples : array size
 */

int Average :: size(void)
{
	return m_maxSamples;
}

/*!
 *  \brief Get maximum value of the array
 *
 *  Retourne la valeur max trouv�e dans le tableau.
 *
 *	\return maximum : maximum value
 */

int Average :: get_maximum(void)
{
	int maximum = 0;
	
	for(int i = 0; i < m_maxSamples; i++)
	{
		if(maximum < m_values[i])
		{
			maximum = m_values[i];
		}
	}
	
	return maximum;
}

/*!
 *  \brief Get minimum value of the array
 *
 *  Retourne la valeur min trouv�e dans le tableau.
 *
 *	\return minimum : minimum value
 */

int Average :: get_minimum(void)
{
	int minimum = 0x7FFFFFFF;
	
	for(int i = 0; i < m_maxSamples; i++)
	{
		if(minimum > m_values[i])
		{
			minimum = m_values[i];
		}
	}
	
	return minimum;
}

/*!
 *  \brief Destructor
 *
 *  Destructeur de la classe Average.
 *
 */

Average :: ~Average(void)
{
	//free(m_values);
}
