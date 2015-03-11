/*!
 * \file LCD.cpp
 * \brief ILI9431 LCD library.
 * \author Rémi.Debord (based on AdaFruit library)
 * \version 1.0
 * \date 2 mars 2015
 *
 * Manage LCD functions (init, draw, ...).
 *
 */

#include "LCD.h"
#include "font.h"

/*!
 *  \brief Constructor
 *
 *  LCD class constructor.
 *
 *  \param spix : SPI object (address)
 *  \param dc : DC pin (Data/Command or DCX)
 *  \param reset : Reset pin
 */

LCD :: LCD(SPI* spix, GPIO_common dc, GPIO_common reset): m_dc(dc), m_reset(reset)
{
	m_spi = spix;
	
	m_height = TFTHEIGHT;
	m_width = TFTWIDTH;
	
	m_text.y = 0;
	m_text.x = 0;
	m_text.color = WHITE;
	m_text.bg = BLACK;
	m_text.size = 1;
	
	// Tx only
	m_spi->mode(0);
}

/*!
 *  \brief LCD init
 *
 *  LCD initialization.
 *
 */

void LCD :: init(void)
{
	this->write(0x00, CMD);
	
	Delay(10);
	
	this->write(0xEF, CMD);
  this->write(0x03, DATA);
  this->write(0x80, DATA);
  this->write(0x02, DATA);

  this->write(0xCF, CMD);  
  this->write(0x00, DATA);
  this->write(0XC1, DATA); 
  this->write(0X30, DATA);; 

	this->write(0xED, CMD);  
  this->write(0x64, DATA); 
  this->write(0x03, DATA); 
  this->write(0X12, DATA); 
  this->write(0X81, DATA); 
 
	this->write(0xE8, CMD);  
  this->write(0x85, DATA); 
  this->write(0x00, DATA); 
  this->write(0x78, DATA); 

	this->write(0xCB, CMD);  
  this->write(0x39, DATA); 
  this->write(0x2C, DATA); 
  this->write(0x00, DATA); 
  this->write(0x34, DATA); 
  this->write(0x02, DATA); 
 
	this->write(0xF7, CMD);  	
  this->write(0x20, DATA); 

  this->write(0xEA, CMD);
  this->write(0x00, DATA); 
  this->write(0x00, DATA); 
	
	// Power control 
  this->write(PWCTR1, CMD); 
  this->write(0x23, DATA);   //VRH[5:0] 
	
	// Power control 
  this->write(PWCTR2, CMD);    
  this->write(0x10, DATA);  //SAP[2:0];BT[3:0] 
	
	// VCM control 
  this->write(VMCTR1, CMD);    
  this->write(0x3E, DATA);
  this->write(0x28, DATA);
	
  //VCM control 2 
  this->write(VMCTR2, CMD);    
  this->write(0x86, DATA);  //--
	
	// Memory Access Control 
  this->write(MADCTL, CMD);    
  this->write(0x48, DATA);

  this->write(PIXFMT, CMD);    
  this->write(0x55, DATA);
  
  this->write(FRMCTR1, CMD);    
  this->write(0x00, DATA);  
  this->write(0x18, DATA);
	
	// Display Function Control 
  this->write(DFUNCTR, CMD);    
  this->write(0x08, DATA); 
  this->write(0x82, DATA);
  this->write(0x27, DATA); 
 
	// 3Gamma Function Disable 
  this->write(0xF2, CMD);
  this->write(0x00, DATA);
	
	// Gamma curve selected 
  this->write(GAMMASET, CMD);    
  this->write(0x01, DATA); 

	// Set Gamma 
  this->write(GMCTRP1, CMD);    
  this->write(0x0F, DATA); 
  this->write(0x31, DATA);
  this->write(0x2B, DATA);
  this->write(0x0C, DATA); 
  this->write(0x0E, DATA); 
  this->write(0x08, DATA); 
  this->write(0x4E, DATA);
  this->write(0xF1, DATA); 
  this->write(0x37, DATA);
  this->write(0x07, DATA);
  this->write(0x10, DATA);
  this->write(0x03, DATA); 
  this->write(0x0E, DATA); 
  this->write(0x09, DATA); 
  this->write(0x00, DATA); 
  
	// Set Gamma 
  this->write(GMCTRN1, CMD);    
  this->write(0x00, DATA); 
  this->write(0x0E, DATA); 
  this->write(0x14, DATA); 
  this->write(0x03, DATA); 
  this->write(0x11, DATA); 
  this->write(0x07, DATA); 
  this->write(0x31, DATA); 
  this->write(0xC1, DATA); 
  this->write(0x48, DATA); 
  this->write(0x08, DATA); 
  this->write(0x0F, DATA); 
  this->write(0x0C, DATA); 
  this->write(0x31, DATA); 
  this->write(0x36, DATA); 
  this->write(0x0F, DATA);
	
	// Exit Sleep
	this->write(SLPOUT, CMD); 
	
  Delay(120);
	
	// Display on 
  this->write(DISPON, CMD);

	Delay(10);
}

/*!
 *  \brief Line
 *
 *  Draw a line.
 *
 *  \param x1 : x coordinate (start point)
 *  \param y1 : y coordinate (start point)
 *  \param x2 : x coordinate (end point)
 *  \param y2 : y coordinate (end point)
 */

void LCD :: line(int x1, int y1, int x2, int y2, int color)
{
  int tmp;
  int x,y;
  int dx, dy;
  int err;
  int ystep;

  char swapxy = 0;

  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;

  if ( dy > dx ) 
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 ) 
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
	
  err = dx >> 1;
	
  if ( y2 > y1 ) ystep = 1;
	else ystep = -1;
  y = y1;
	
  for( x = x1; x <= x2; x++ )
  {
    if ( swapxy == 0 ) this->pixel(x, y, color); 
    else this->pixel(y, x, color); 
    
		err -= (char)dy;
    
		if ( err < 0 )
    {
      y += (int)ystep;
      err += (int)dx;
    }
  }
}

/*!
 *  \brief Vertical line
 *
 *  Draw a vertical line (more faster than line).
 *
 *  \param x : x coordinate (start point)
 *  \param y : y coordinate (start point)
 *  \param h : height
 *  \param color : line color
 */

void LCD :: FastVLine(int x, int y, int h, int color)
{ 
	char hi = color >> 8;
	char lo = color;
  
	if((x >= m_width) || (y >= m_height)) return;

  if((y+h-1) >= TFTHEIGHT)
	{
    h = TFTHEIGHT - y;
	}

  this->address(x, y, x, y+h-1);

	m_dc = 1;
	
	m_spi->cs(0);

  while(h--)
	{
    m_spi->write(hi);
    m_spi->write(lo);
  }
	
	m_spi->cs(1);
}

/*!
 *  \brief Horizontal line
 *
 *  Draw an horizontal line (more faster than line).
 *
 *  \param x : x coordinate (start point)
 *  \param y : y coordinate (start point)
 *  \param w : width
 *  \param color : line color
 */

void LCD :: FastHLine(int x, int y, int w, int color)
{
	char hi = color >> 8;
	char lo = color;
	
	if((x >= m_width) || (y >= m_height)) return;
	
  if((x+w-1) >= TFTWIDTH)  w = TFTWIDTH-x;
	
  this->address(x, y, x+w-1, y);

	m_dc = 1;
	
	m_spi->cs(0);
	
  while (w--)
	{
    m_spi->write(hi);
    m_spi->write(lo);
  }
	
	m_spi->cs(1);
}

/*!
 *  \brief Circle
 *
 *  Draw a circle.
 *
 *  \param x0 : x coordinate (circle center)
 *  \param y0 : y coordinate (circle center)
 *  \param r : radius
 *  \param color : circle color
 */

void LCD :: circle(int x0, int y0, int r, int color)
{
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  this->pixel(x0, y0+r, color);
  this->pixel(x0, y0-r, color);
  this->pixel(x0+r, y0, color);
  this->pixel(x0-r, y0, color);

  while (x<y)
	{
    if (f >= 0)
		{
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;
  
    this->pixel(x0 + x, y0 + y, color);
    this->pixel(x0 - x, y0 + y, color);
    this->pixel(x0 + x, y0 - y, color);
    this->pixel(x0 - x, y0 - y, color);
    this->pixel(x0 + y, y0 + x, color);
    this->pixel(x0 - y, y0 + x, color);
    this->pixel(x0 + y, y0 - x, color);
    this->pixel(x0 - y, y0 - x, color);
  }
}

/*!
 *  \brief Rectangle
 *
 *  Draw a rectangle.
 *
 *  \param x : x coordinate (start point)
 *  \param y : y coordinate (start point)
 *  \param h : height
 *  \param color : rectangle color
 *  \param fill : 1 (fill rectangle) | 0 (contour) 
 */

void LCD :: rect(int x, int y, int w, int h, int color, char fill)
{
	int i = 0;
	
	if(fill)
	{
		for(i = x; i < x+w; i++)
		{
			this->FastVLine(i, y, h, color);
		}
	}
	else
	{
		this->FastHLine(x, y, w, color);
		this->FastHLine(x, y+h-1, w, color);
		this->FastVLine(x, y, h, color);
		this->FastVLine(x+w-1, y, h, color);
	}
}

/*!
 *  \brief Text
 *
 *  Draw text.
 *
 *  \param buffer : string to display
 *  \param length : string length
 */

void LCD :: txt(char* buffer, int length)
{
	int i = 0;
	char c = 0;
	
	for(i = 0; i < length; i++)
	{
		c = buffer[i];
		
		if (c == '\n')
		{
			m_text.y += m_text.size * 8;
			m_text.x  = 0;
		}
		else if (c == '\r')
		{
			// skip em
		}
		else
		{
			this->character(m_text.x, m_text.y, c, m_text.color, m_text.bg, m_text.size);
			
			m_text.x += m_text.size * 6;
			
			if (m_text.x > (m_width - m_text.size * 6))
			{
				m_text.y += m_text.size * 8;
				m_text.x = 0;
			}
		}
	}
}

/*!
 *  \brief Text
 *
 *  Draw text.
 *
 *  \param buffer : string to display
 *  \param length : string length
 *  \param size : text size
 */

void LCD :: txt(char* buffer, int length, int size)
{
	m_text.size = size;
	
	this->txt(buffer, length);
}

/*!
 *  \brief Text
 *
 *  Draw text.
 *
 *  \param buffer : string to display
 *  \param length : string length
 *  \param color : text color
 *  \param bg : background color
 */

void LCD :: txt(char* buffer, int length, int color, int bg)
{
	m_text.color = color;
	m_text.bg = bg;
	
	this->txt(buffer, length);
}

/*!
 *  \brief Text
 *
 *  Draw text.
 *
 *  \param buffer : string to display
 *  \param length : string length
 *  \param size : text size
 *  \param color : text color
 *  \param bg : background color
 */

void LCD :: txt(char* buffer, int length, int size, int color, int bg)
{
	m_text.size = size;
	m_text.color = color;
	m_text.bg = bg;
	
	this->txt(buffer, length);
}

/*!
 *  \brief Character
 *
 *  Draw a character.
 *
 *  \param x : x coordinate
 *  \param y : y coordinate
 *  \param c : character
 *  \param color : text color
 *  \param bg : background color
 *  \param size : text size
 */

void LCD :: character(int x, int y, unsigned char c, int color, int bg, char size)
{
	char i, j;
	char line;
	
  if((x >= m_width)            	|| // Clip right
     (y >= m_height)           	|| // Clip bottom
     ((x + 6 * size - 1) < 0)		|| // Clip left
     ((y + 8 * size - 1) < 0))   	 // Clip top
    return;

  for (i = 0; i < 6; i++)
	{
    if (i == 5) line = 0x0;
    else line = pgm_read_byte(font+(c*5)+i);

    for (j = 0; j < 8; j++)
		{
      if (line & 0x1)
			{
        if (size == 1) this->pixel(x+i, y+j, color); // default size         
        else this->rect(x+(i*size), y+(j*size), size, size, color, 1); // big size
      }
			else if (bg != color)
			{
        if (size == 1) this->pixel(x+i, y+j, bg); // default size
        else this->rect(x+i*size, y+j*size, size, size, bg, 1); // big size	
      }
      line >>= 1;
    }
  }
}

/*!
 *  \brief Cursor
 *
 *  Set cursor position.
 *
 *  \param x : x coordinate
 *  \param y : y coordinate
 */

void LCD :: cursor(int x, int y)
{
	if((x >= m_width) || (y >= m_height)) return;
	
	m_text.x = x;
	m_text.y = y;
}

/*!
 *  \brief Invert
 *
 *  Invert screen colors.
 *
 *  \param i : 1 (invert), 0 (normal)
 */

void LCD :: invert(char i)
{
	this->write(i ? INVON : INVOFF, CMD);
}

/*!
 *  \brief Rotate
 *
 *  Rotate screen (90°, 180°, 270° and 360°).
 *
 *  \param x : 1 (90°), 2 (180°), ... .
 */

void LCD :: rotate(char x)
{
	char rotation;
	
  this->write(MADCTL, CMD);
	
  rotation = x % 4; // can't be higher than 3
	
	switch(rotation)
	{
		case 0:
			this->write(MADCTL_MX | MADCTL_BGR, DATA);
			m_width  = TFTWIDTH;
			m_height = TFTHEIGHT;
			break;
		case 1:
			this->write(MADCTL_MV | MADCTL_BGR, DATA);
			m_width  = TFTHEIGHT;
			m_height = TFTWIDTH;
			break;
		case 2:
			this->write(MADCTL_MY | MADCTL_BGR, DATA);
			m_width  = TFTWIDTH;
			m_height = TFTHEIGHT;
			break;
		case 3:
			this->write(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR, DATA);
			m_width  = TFTHEIGHT;
			m_height = TFTWIDTH;
			break;
  }
}

/*!
 *  \brief Pixel
 *
 *  Draw pixel (base method).
 *
 *  \param x : x coordinate
 *  \param y : y coordinate
 *  \param color : pixel color
 */

void LCD :: pixel(short int x, short int y, unsigned short int color)
{
	if((x < 0) ||(x >= m_width) || (y < 0) || (y >= m_height))
	{
		return;
	}
	
	this->address(x, y, x+1, y+1);
	
	m_dc = 1;
	
	m_spi->cs(0);
	
	m_spi->write(color >> 8);
	m_spi->write(color);
	
	m_spi->cs(1);
}

/*!
 *  \brief Address
 *
 *  "Unlock" pixel(s) (memory access).
 *
 *  \param x0 : x coordinate (start)
 *  \param y0 : y coordinate (start)
 *  \param x1 : x coordinate (end)
 *  \param y1 : y coordinate (end)
 */

void LCD :: address(unsigned short int x0, unsigned short int y0,	unsigned short int x1, unsigned short int y1)
{
	// Column addr set
	this->write(CASET, CMD);
  this->write(x0 >> 8, DATA);
  this->write(x0 & 0xFF, DATA);
	this->write(x1 >> 8, DATA);
  this->write(x1 & 0xFF, DATA);

	// Row addr set
  this->write(PASET, CMD);
  this->write(y0 >> 8, DATA);
  this->write(y0, DATA);
  this->write(y1 >> 8, DATA);
  this->write(y1, DATA);

  this->write(RAMWR, CMD); // write to RAM
}

/*!
 *  \brief Write
 *
 *  Send command or data to LCD.
 *
 *  \param c : value to send
 *  \param dc : data(1), cmd(0)
 */

void LCD :: write(char c, char dc)
{
	// 0 (CMD), 1 (DATA)
	m_dc = dc;
	
	m_spi->cs(0);
	
	m_spi->write(c);
	
	m_spi->cs(1);
}
