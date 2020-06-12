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

#include "hardware_interface.h"

//---------- Arduino implementation ----------//

#include <Arduino.h>

void (*uart_callback)(void);


void serialEvent() {
  if(uart_callback!=nullptr)
    uart_callback();
}

void __attribute__((weak)) UartConfigCallback(void (*callback)(void))
{
  uart_callback = callback;
}


void __attribute__((weak)) UartBegin(uint32_t baudrate)
{
  Serial.begin(baudrate);
}

void __attribute__((weak)) UartWrite(uint8_t data)
{
  Serial.write(data);
}

uint8_t __attribute__((weak)) UartRead(void)
{
  return Serial.read();
}

bool __attribute__((weak)) UartAvailable(void){
  return Serial.available();
}
