
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

#include "hicop_ll.h"

#include "resources/hardware_interface.h"
#include "resources/time.h"
#include "resources/crc8.h"

struct HicopLLManagement
{
    uint16_t prev_pointer_last; //do some backup to know how many data was before buffer clean up
    uint16_t pointer_last;      //saves the actual buffer pos
    uint32_t time_stamp;
    union {

        //buffer transaction
        struct
        {
            uint8_t header; //type of data
            uint8_t length;
            uint8_t payload[HICOP_LL_BUFFER_LENGTH - 2];
        };
        uint8_t exchange[HICOP_LL_BUFFER_LENGTH]; //general buffer declaration
    };

} g_hicop_ll_tx, g_hicop_ll_rx;

static void HicopLLReceptionEvent(void);
static bool HicopLLSendBuffer(void);
static bool HicopLLWriteBuffer(uint8_t data_out);

void HicopLLBegin(void)
{
    UartBegin(HICOP_LL_SERIAL_BAUDRATE);
    UartConfigCallback(HicopLLReceptionEvent);
    g_hicop_ll_rx.pointer_last = 0;
    g_hicop_ll_tx.pointer_last = 0;
    g_hicop_ll_rx.time_stamp=0;
    g_hicop_ll_tx.time_stamp=0;
}

///////////// Rx Methods ////////////

bool HicopLLIsReceptionComplete(void)
{
    //if no data, no reception
    if (g_hicop_ll_rx.pointer_last < 2)
        return false;

    //if there is a complete frame
    if (g_hicop_ll_rx.pointer_last >= (g_hicop_ll_rx.length + 4))
        return true;

    //if no more data is received
    if (GetDiffTime(Millis(), g_hicop_ll_rx.time_stamp) > HICOP_LL_MAX_RX_TIME)
        return true;

    return false;
}

void HicopLLCleanBufferRx(void)
{
    //reset buffer pointer
    g_hicop_ll_rx.prev_pointer_last = g_hicop_ll_rx.pointer_last;
    g_hicop_ll_rx.pointer_last = 0;
}

uint8_t HicopLLGetHeader(void)
{
    //return header info if there are enough buffer data
    if (g_hicop_ll_rx.pointer_last < 1)
        return HICOP_LL_INVALID_HEADER;

    return g_hicop_ll_rx.header;
}

uint8_t HicopLLGetLength(void)
{
    //return length info if there are enough buffer data
    if (g_hicop_ll_rx.pointer_last < 2)
        return 0;
    return g_hicop_ll_rx.length;
}

uint8_t *HicopLLGetPayload(void)
{
    //return data on payloda under user responsibility
    return &g_hicop_ll_rx.payload[0];
}

bool HicopLLIsValidData(void)
{
    //check if there is data
    if (g_hicop_ll_rx.pointer_last < 4)
        return false;

    // check if there is enough data
    if ((g_hicop_ll_rx.length + 4) != g_hicop_ll_rx.pointer_last)
        return false;

    //check if the last character is the expected
    if (g_hicop_ll_rx.exchange[g_hicop_ll_rx.pointer_last - 1] != 0xff)
        return false;

    //then check if CRC is OK
    CRC8Configure(HICOP_LL_CRC8_POLY, HICOP_LL_CRC8_INIT);
    if (g_hicop_ll_rx.exchange[g_hicop_ll_rx.pointer_last - 2] != CRC8Calculate(&g_hicop_ll_rx.exchange[0], g_hicop_ll_rx.length + 2))
        return false;

    //if everything is OK
    return true;
}

///////////// Tx Methods ////////////

void HicopLLSetHeader(uint8_t header)
{
    //new data will be loaded
    g_hicop_ll_tx.length = 0;
    //confing requested header
    g_hicop_ll_tx.header = header;
    //update buffer
    g_hicop_ll_tx.pointer_last = 2;
}

bool HicopLLAddPayload(uint8_t *payload, uint8_t length)
{
    uint16_t i = 0;

    //do anything
    if (length == 0)
        return false;

    while (length--)
    {
        if (HicopLLWriteBuffer(payload[i++]))
            g_hicop_ll_tx.length++;
        else
            return false; //if no more data can be added
    }

    return true;
}

bool HicopLLAddPayload(uint8_t payload)
{
    HicopLLAddPayload(&payload, 1);
}

bool HicopLLSendPayload(void)
{
    //compute data from de begining to actual date
    CRC8Configure(HICOP_LL_CRC8_POLY, HICOP_LL_CRC8_INIT);
    if (!HicopLLWriteBuffer(CRC8Calculate(&g_hicop_ll_tx.exchange[0], g_hicop_ll_tx.pointer_last)))
        return false;

    //add end byte
    if (!HicopLLWriteBuffer(0xff))
        return false;

    g_hicop_ll_tx.prev_pointer_last = g_hicop_ll_tx.pointer_last;
    HicopLLSendBuffer();

    return true;
}

bool HicopLLResendPayload(void)
{
    //make sure tha there was a previous frame
    if (g_hicop_ll_tx.prev_pointer_last < 4)
        return false;

    g_hicop_ll_tx.pointer_last = g_hicop_ll_tx.prev_pointer_last;

    HicopLLSendBuffer();
    return true;
}

////////////// Raw Methods ///////////////

bool HicopLLWriteBuffer(uint8_t data_out)
{
    if (g_hicop_ll_tx.pointer_last >= HICOP_LL_BUFFER_LENGTH)
        return false;

    g_hicop_ll_tx.exchange[g_hicop_ll_tx.pointer_last++] = data_out;

    return true;
}

bool HicopLLSendBuffer(void)
{
    uint16_t i = 0;

    if (0 == g_hicop_ll_tx.pointer_last)
        return false;

    g_hicop_ll_tx.time_stamp = Millis();
    while (g_hicop_ll_tx.pointer_last--)
    {
        UartWrite(g_hicop_ll_tx.exchange[i++]);
    }
    return true;
}

static void HicopLLReceptionEvent(void)
{
    g_hicop_ll_rx.time_stamp = Millis();
    while (UartAvailable())
    {
        g_hicop_ll_rx.exchange[g_hicop_ll_rx.pointer_last++] = UartRead();
    }
}
