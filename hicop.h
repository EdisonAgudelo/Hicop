

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


#ifndef _HICOP_H_
#define _HICOP_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef HICOP_MAX_DATA_LENGTH
#define HICOP_MAX_DATA_LENGTH 250 // don't move, but it talks about maximum transmission lenght
#endif

#ifndef HICOP_STACK_SIZE
#define HICOP_STACK_SIZE 2  //FIFO length 
#endif

#ifndef HICOP_MAX_RESPONSE_TIME
#define HICOP_MAX_RESPONSE_TIME 100 //ms. Which is other mcu maximum response time
#endif

#ifndef HICOP_MAX_SEND_TRIES
#define HICOP_MAX_SEND_TRIES 10    //how many times hicop protocol will try to resend a failed transmission
#endif


//hicop header types
typedef enum
{
    kHicopHeaderData = 0x0, //>> This header is for data streaming. Hicop protocol do not resend it in case of fail transfer
    kHicopHeaderAlarm, //>> This header is for data streaming. Hicop protocol resend it in case of fail transfer. It has a limited attempts before discart it 
    kHicopHeaderConfig, //>> This header is for config high level user parameters. Hicop protocol resend it in case of fail transfer. It has a limited attempts before discart it 
    kHicopHeaderFlags //>> Hicop protocol flags. User must use it under his responsability. 
} HicopHeaders;
//kHicopHeaderAlarm and kHicopHeaderConfig have the same behaviour. User can use this for specific application purpose


//this function checks if other mcu has sent data. In case of available data, function 
//transffers those data to user buffer. Otherwise, return false.
bool HicopReadData(HicopHeaders *type, uint8_t *data, uint8_t *lengh);

//This function takes user data and saves it into internal buffer to send to the other mcu. 
//It allows user to accomulate a limited consecutive data for subsequent transffers.
//Funtion returns true if there are space in FIFO buffer else returns false.
bool HicopSendData(HicopHeaders type, uint8_t *data, uint8_t lengh);

//this function should be called fast enough to respond to other MCU and to have great performance. 
//this make all Hicop transaction and hicop protocols.
void HicopLoop(void);

//this function makes hicop initialization procedure. Only should be called one time in fw beginings
void HicopInit(void);

#endif