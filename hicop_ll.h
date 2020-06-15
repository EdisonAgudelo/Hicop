
/*
MIT License

Copyright (c) 2020 Edison Agudelo and Mateo Garcia

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

#ifndef _HICOP_LL_H_
#define _HICOP_LL_H_

#include <stdint.h>
#include <stdbool.h>

#include "hicop_config.h"

//this library is the low level human interface comunitacion protocol
//it does all frame verfication, reception and transmision task. 

//start hicop_ll protocol
void HicopLLBegin(void);

//check if other MCU has finish or not 
bool HicopLLIsReceptionComplete(void);

//Prepare reception buffer for another tx
void HicopLLCleanBufferRx(void);

//get received header
uint8_t HicopLLGetHeader(void);

//get payload length
uint8_t HicopLLGetLength(void);

//get a pointer where is stored payload
uint8_t *HicopLLGetPayload(void);

// check if data integrity 
bool HicopLLIsValidData(void);

// start a new tx frame
void HicopLLSetHeader(uint8_t header);

// add data to tx frame
bool HicopLLAddPayload(uint8_t *payload, uint8_t length);
bool HicopLLAddPayload(uint8_t payload);

// send data to other MCU
bool HicopLLSendPayload(void);

// if the same buffer will be send again, user should use this aferter the first time.
bool HicopLLResendPayload(void);



#endif