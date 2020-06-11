
/*
MIT License

Copyright (c) 2020 Edison Agudelo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdint.h>

#include "crc8.h"
/*
this function calculates crc 8 for a given data.  
*/

uint8_t __attribute__((weak)) g_crc_polynomial=0x31; //default value
uint8_t __attribute__((weak)) g_crc_inital = 0xff; //default value


uint8_t __attribute__((weak)) CRC8Calculate(uint8_t *data, uint8_t length)
{ 
  uint8_t crc = g_crc_inital;	
  uint8_t index;
  uint8_t bit;
  
  //calculates 8-Bit checksum with given polynomial
  for (index = 0; index < length; ++index)
  {
	crc ^= (data[index]);
    for ( bit = 8; bit > 0; --bit)
    {
	if (crc & 0x80) 
		crc = (crc << 1) ^ g_crc_polynomial;
    else 
		crc = (crc << 1);
    }
  }
  return crc;
}

uint8_t __attribute__((weak)) CRC8Calculate(uint16_t data)
{ 
  uint8_t buffer[] = {(uint8_t)(data>>8), (uint8_t)(data&0xff)};
  return CRC8Calculate(buffer, 2);
}

/*
configure crc parameters used to calculate crc value
*/
uint8_t __attribute__((weak)) CRC8Configure(uint8_t poly, uint8_t initial)
{
	g_crc_polynomial = poly;
	g_crc_inital = initial;
}