
#ifndef _HICOP_CONFIG_H_
#define _HICOP_CONFIG_H_

//high layer

#ifndef HICOP_MAX_DATA_LENGTH
#define HICOP_MAX_DATA_LENGTH 250 // don't move, but it talks about maximum transmission lenght
#endif

#ifndef HICOP_STACK_SIZE
#define HICOP_STACK_SIZE 5  //FIFO length 
#endif

#ifndef HICOP_MAX_RESPONSE_TIME
#define HICOP_MAX_RESPONSE_TIME 100 //ms. Which is other mcu maximum response time
#else
#warning HICOP_MAX_RESPONSE_TIME was changed
#endif

#ifndef HICOP_MAX_SEND_TRIES
#define HICOP_MAX_SEND_TRIES 10    //how many times hicop protocol will try to resend a failed transmission
#endif

//lower layer


#ifndef HICOP_LL_BUFFER_LENGTH
#define HICOP_LL_BUFFER_LENGTH 255
#endif

#ifndef HICOP_LL_SERIAL_BAUDRATE
#define HICOP_LL_SERIAL_BAUDRATE 115200
#endif

#ifndef HICOP_LL_INVALID_HEADER
#define HICOP_LL_INVALID_HEADER 0xff
#endif

#ifndef HICOP_LL_MAX_RX_TIME
#define HICOP_LL_MAX_RX_TIME 10 //mss
#else 
#warning HICOP_LL_MAX_RX_TIME was changed
#endif

#ifndef HICOP_LL_CRC8_POLY
#define HICOP_LL_CRC8_POLY 0x31
#endif

#ifndef HICOP_LL_CRC8_INIT
#define HICOP_LL_CRC8_INIT 0x00
#endif


#include "hicop.h"
#include "hicop_ll.h"

#endif