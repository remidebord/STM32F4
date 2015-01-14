#include "GPS.h"
#include <stdio.h>
#include <string.h>

GPS :: GPS(Serial* serialx)
{
	m_serial = serialx;
}

void GPS :: update(void)
{
	char buffer[MAX_USART_RX_BUF_LEN] = {};
	char sentence[128] = {};
	char field[16] = {};
	int length;
		
	length = m_serial->read(buffer);
	
	// Data received ?
	if(length > 0)
	{
		// Parse GPGGA
		if(this->get_sentence(buffer, (char*)"$GPGGA", sentence))
		{
			// Verify checksum
			if(this->get_checksum(sentence) == this->calculate_checksum(sentence))
			{
				// Get time
				if(this->get_field(sentence, 1, field))
				{
					if(sscanf(field,"%6d", &m_time) <= 0)
					{
						m_time = 0;
					}
				}
				
				// Get latitude
				if(this->get_field(sentence, 2, field))
				{
					if(sscanf(field,"%f", &m_latitude) <= 0)
					{
						m_latitude = 0;
					}
					else
					{
						m_latitude = ((int)m_latitude / 100) + ((((int)m_latitude % 100) + (m_latitude - (int)m_latitude)) / 60);
					}
				}
				
				// North or south
				if(this->get_field(sentence, 3, field))
				{
					m_latitude = (field[0] == 'N') ? m_latitude : -m_latitude;
				}
				
				// Get longitude
				if(this->get_field(sentence, 4, field))
				{
					if(sscanf(field,"%f", &m_longitude) <= 0)
					{
						m_longitude = 0;
					}
					else
					{
						m_longitude = ((int)m_longitude / 100) + ((((int)m_longitude % 100) + (m_longitude - (int)m_longitude)) / 60);
					}
				}
				
				// East or west
				if(this->get_field(sentence, 5, field))
				{
					m_longitude = (field[0] == 'E') ? m_longitude : -m_longitude;
				}
				
				// Get fix
				if(this->get_field(sentence, 6, field))
				{
					m_fix = field[0] - 0x30;
				}
				
				// Get satellite
				if(this->get_field(sentence, 7, field))
				{
					if(sscanf(field,"%2d", &m_satellite) <= 0)
					{
						m_satellite = 0;
					}
				}
				
				// Get altitude
				if(this->get_field(sentence, 9, field))
				{
					if(sscanf(field,"%f", &m_altitude) <= 0)
					{
						m_altitude = 0;
					}
				}
			}
		}
		
		// Parse GPRMC
		if(this->get_sentence(buffer, (char*)"$GPRMC", sentence))
		{
			// Verify checksum
			if(this->get_checksum(sentence) == this->calculate_checksum(sentence))
			{
				// Get navigation state (A = ok, V = warning) 
				if(this->get_field(sentence, 2, field))
				{
					m_valid = field[0];
				}
				
				// Get speed (knots)
				if(this->get_field(sentence, 7, field))
				{
					if(sscanf(field,"%f", &m_speed) <= 0)
					{
						m_speed = 0;
					}
				}
				
				// Get track (heading)
				if(this->get_field(sentence, 8, field))
				{
					if(sscanf(field,"%f", &m_track) <= 0)
					{
						m_track = 0;
					}
				}
				
				// Get date
				if(this->get_field(sentence, 9, field))
				{
					if(sscanf(field,"%6d", &m_date) <= 0)
					{
						m_date = 0;
					}
				}
			}
		}
	}
}

int GPS :: time(char part)
{
	char hour = m_time / 10000;
	char minute = (m_time / 100) % 100;
	char second = m_time % 100;
	int tmp = 0;
	
	if((part & HOUR) != 0)
	{
		tmp = hour;
	}
	if((part & MINUTE) != 0)
	{
		tmp = (tmp > 0) ? (tmp * 100) + minute : tmp + minute;
	}
	if((part & SECOND) != 0)
	{
		tmp = (tmp > 0) ? (tmp * 100) + second : tmp + second;
	}
	
	return tmp;
}

float GPS :: latitude(void)
{
	return m_latitude;
}

float GPS :: longitude(void)
{
	return m_longitude;
}

char GPS :: fix(void)
{
	return m_fix;
}

int GPS :: satellite(void)
{
	return m_satellite;
}

float GPS :: altitude(void)
{
	return m_altitude;
}

float GPS :: speed(char units)
{
	float speed = 0;
	
	if(units == KMH)
	{
		speed = m_speed * KNOT;
	}
	else if(units == SMPH)
	{
		speed = m_speed;
	}
	
	return speed;
}

float GPS :: track(void)
{
	return m_track;
}

int GPS :: date(char part)
{
	char day = m_date / 10000;
	char month = (m_date / 100) % 100;
	char year = m_date % 100;
	int tmp = 0;
	
	if((part & DAY) != 0)
	{
		tmp = day;
	}
	if((part & MONTH) != 0)
	{
		tmp = (tmp > 0) ? (tmp * 100) + month : tmp + month;
	}
	if((part & YEAR) != 0)
	{
		tmp = (tmp > 0) ? (tmp * 100) + year : tmp + year;
	}
	
	return tmp;
}

char GPS :: valid(void)
{
	if(m_valid == 'A')
	{
		return 1;
	}
	else return 0;
}

char GPS :: calculate_checksum(char* sentence)
{
	char checksum = 0;
	char* ptr1 = sentence;
	char* ptr2;
	char length, i;
	
	// Search '*'
	ptr2 = strchr(sentence, '*');
	
	if(ptr2 != NULL)
	{
		length = ptr2 - ptr1;
		
		// Calculate checksum (all characters between $ and *) 
		for(i = 1; i < length; i++)
		{
			checksum ^= sentence[i];
		}
	}
	
	return checksum;	
}

char GPS :: get_checksum(char* sentence)
{
	char checksum = 0;
	char* ptr;
	
	ptr = strchr(sentence, '*');
	
	if(ptr != NULL)
	{
		// Convert characters into hexa values
		checksum |= (ptr[1] >= 'A') ? (ptr[1] - 0x37) << 4 : (ptr[1] - 0x30) << 4;
		checksum |= (ptr[2] >= 'A') ? (ptr[2] - 0x37) : (ptr[2] - 0x30);
	}
	
	return checksum;	
}

char GPS :: get_field(char *in, char field, char* out)
{
	int length, i;
	char* ptr1 = in;
	char* ptr2;
	
	// Pass undesired field
	for(i = 0; i < field; i++)
	{
		ptr1 = strchr(ptr1+1, ',');
	}
	
	// ',' exist ?
	if(ptr1 != NULL)
	{
		// Search second ','
		ptr2 = strchr(ptr1+1, ',');

		// ',' exist ?
		if(ptr2 != NULL)
		{
			length = ptr2 - ptr1;
			
			// Copy field
			strncpy(out, ptr1+1, length-1);
			
			return length-1;
		}
	}
	
	return 0;
}

char GPS :: get_sentence(char *in, char* sentence, char* out)
{
	int length;
	char* ptr1;
	char* ptr2;
	
	// Get sub string
	ptr1 = strstr(in, sentence);
	
	// Sub string exist ?
	if(ptr1 != NULL)
	{
		// Search CR
		ptr2 = strchr(ptr1, 0x0D);
		
		// CR exist ?
		if(ptr2 != NULL)
		{
			length = ptr2 - ptr1;
			
			// Copy sub string
			strncpy(out, ptr1, length);
			
			return length;
		}
	}
	
	return 0;
}
