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

#ifndef _HARDWARE_INTERFACE_H_
#define _HARDWARE_INTERFACE_H_


#include <stdint.h>
#include <stdbool.h>

//Uart Functions

//call any function when data is available on serial buffer
void UartConfigCallback(void (*callback)(void));

//init uart module
void UartBegin(uint32_t baudrate);

//check if there is available data on serial buffer
bool UartAvailable(void);

// Send data through uart module
void UartWrite(uint8_t data);

// Get data through uart module
uint8_t UartRead(void);



#endif