#ifndef __GPS_H
#define __GPS_H

/* includes ---------------------------------------------------------------- */
#include "Serial.h"

/* defines ----------------------------------------------------------------- */
#define KNOT 1,852 // km/h

#define KMH 	1
#define SMPH 	2

#define HOUR 		1
#define MINUTE 	2
#define SECOND 	4

#define DAY 	1
#define MONTH 2
#define YEAR	4

/* class ------------------------------------------------------------------- */
class GPS
{
	private:
		
		Serial* m_serial;
		int m_time;
		float m_latitude;
		float m_longitude;
		char m_fix;
		int m_satellite;
		float m_altitude;
		float m_speed;
		float m_track; // heading
		int m_date;
		char m_valid;
	
	public:
		
		GPS(Serial* serialx);
		void update(void);
		int time(char part = (HOUR | MINUTE | SECOND));
		float latitude(void);
		float longitude(void);
		char fix(void);
		int satellite(void);
		float altitude(void);
		float speed(char units = KMH);
		float track(void);
		int date(char part = (YEAR | MONTH | DAY));
		char valid(void);
		char calculate_checksum(char* sentence);
		char get_checksum(char* sentence);
		char get_field(char *in, char field, char* out);
		char get_sentence(char *in, char* sentence, char* out);
};

#endif
