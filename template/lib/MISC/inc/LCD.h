#ifndef __LCD_H
#define __LCD_H

/* includes ---------------------------------------------------------------- */
#include "SPI.h"
#include "Digital.h"
#include "Delay.h"

/* defines ----------------------------------------------------------------- */
#define CMD 	0
#define DATA 	1

#define TFTWIDTH  240
#define TFTHEIGHT 320

// ILI9341 registers
#define NOP     0x00
#define SWRESET 0x01
#define RDDID   0x04
#define RDDST   0x09

#define SLPIN   0x10
#define SLPOUT  0x11
#define PTLON   0x12
#define NORON   0x13

#define RDMODE  0x0A
#define RDMADCTL  0x0B
#define RDPIXFMT  0x0C
#define RDIMGFMT  0x0A
#define RDSELFDIAG  0x0F

#define INVOFF  0x20
#define INVON   0x21
#define GAMMASET 0x26
#define DISPOFF 0x28
#define DISPON  0x29

#define CASET   0x2A
#define PASET   0x2B
#define RAMWR   0x2C
#define RAMRD   0x2E

#define PTLAR   0x30
#define MADCTL  0x36
#define PIXFMT  0x3A

#define FRMCTR1 0xB1
#define FRMCTR2 0xB2
#define FRMCTR3 0xB3
#define INVCTR  0xB4
#define DFUNCTR 0xB6

#define PWCTR1  0xC0
#define PWCTR2  0xC1
#define PWCTR3  0xC2
#define PWCTR4  0xC3
#define PWCTR5  0xC4
#define VMCTR1  0xC5
#define VMCTR2  0xC7

#define RDID1   0xDA
#define RDID2   0xDB
#define RDID3   0xDC
#define RDID4   0xDD

#define GMCTRP1 0xE0
#define GMCTRN1 0xE1

// Rotate
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

// Color
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F

/* macros ------------------------------------------------------------------ */
#define pgm_read_byte(addr) (*(const char *)(addr))

/* struct ------------------------------------------------------------------ */
struct text
{
	int x;
	int y;
	int color;
	int bg;
	int size;
};

/* class ------------------------------------------------------------------- */
class LCD
{
	private:
		
		SPI* m_spi;
		DigitalOut m_dc;
		DigitalOut m_reset;
	
		int m_height;
		int m_width;
		text m_text;
	
		void write(char c, char dc);
		void address(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1);
		void FastVLine(int x, int y, int h, int color);
		void FastHLine(int x, int y, int w, int color);
	
	public:
		
		LCD(SPI* spix, GPIO_common dc, GPIO_common reset);
		void init(void);
	
		void line(int x1, int y1, int x2, int y2, int color);
		void circle(int x0, int y0, int r, int color);
		void rect(int x, int y, int w, int h, int color, char fill);
	
		void txt(char* buffer, int length);
		void txt(char* buffer, int length, int size);
		void txt(char* buffer, int length, int color, int bg);
		void txt(char* buffer, int length, int size, int color, int bg);
		//void txt(char* buffer, int length, int size, int color = WHITE, int bg = BLACK);
		void character(int x, int y, unsigned char c, int color, int bg, char size);
		void cursor(int x = 0, int y = 0);

		void invert(char i);
		void rotate(char x);
	
		void pixel(short int x, short int y, unsigned short int color);
};

#endif
