/*
MIT License

Copyright (c) 2020 Marcin Borowicz

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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "utils/slip.h"

int main(int argc, char **argv)
{
        uint8_t buf_rx[32];
        uint8_t buf_tx[32];
        uint8_t data[32];
        uint8_t data_out[32];
        uint32_t data_size;
        uint32_t data_out_size;
        uint32_t buf_rx_size;
        uint32_t i;

        struct slip test_slip;

        slip_init(&test_slip);
        assert(test_slip.rx_index == 0);
        assert(test_slip.state == SLIP_STATE_START);

        data[0] = 0x00;
        data[1] = 0x01;
        data[2] = 0x02;
        data[3] = SLIP_BYTE_START;
        data[4] = 0x03;
        data[5] = SLIP_BYTE_END;
        data[6] = 0x04;
        data[7] = SLIP_BYTE_ESCAPE;
        data[8] = 0x05;
        data[9] = SLIP_BYTE_ESCAPE_START;
        data[10] = 0x06;
        data[11] = SLIP_BYTE_ESCAPE_END;
        data[12] = 0x07;
        data[13] = SLIP_BYTE_ESCAPE_ESCAPE;
        data[14] = 0x08;
        data[15] = 0x09;
        data_size = 16;
        assert(slip_encode_frame(buf_tx, sizeof(buf_tx), &buf_rx_size, data, data_size) == true);
        assert(buf_tx[0] == SLIP_BYTE_START);
        assert(buf_tx[1] == 0x00);
        assert(buf_tx[2] == 0x01);
        assert(buf_tx[3] == 0x02);
        assert(buf_tx[4] == SLIP_BYTE_ESCAPE);
        assert(buf_tx[5] == SLIP_BYTE_ESCAPE_START);
        assert(buf_tx[6] == 0x03);
        assert(buf_tx[7] == SLIP_BYTE_ESCAPE);
        assert(buf_tx[8] == SLIP_BYTE_ESCAPE_END);
        assert(buf_tx[9] == 0x04);
        assert(buf_tx[10] == SLIP_BYTE_ESCAPE);
        assert(buf_tx[11] == SLIP_BYTE_ESCAPE_ESCAPE);
        assert(buf_tx[12] == 0x05);
        assert(buf_tx[13] == SLIP_BYTE_ESCAPE_START);
        assert(buf_tx[14] == 0x06);
        assert(buf_tx[15] == SLIP_BYTE_ESCAPE_END);
        assert(buf_tx[16] == 0x07);
        assert(buf_tx[17] == SLIP_BYTE_ESCAPE_ESCAPE);
        assert(buf_tx[18] == 0x08);
        assert(buf_tx[19] == 0x09);
        assert(buf_tx[20] == SLIP_BYTE_END);
        assert(buf_rx_size == 21);

        data_out_size = 0;
        for (i = 0; i < buf_rx_size; i++)
                assert(slip_decode_frame(&test_slip, buf_tx[i], data_out, sizeof(data_out), &data_out_size) == ((i == (buf_rx_size - 1)) ? true : false));
        assert(data_out_size == data_size);
        for (i = 0; i < data_out_size; i++)
                assert(data[i] == data_out[i]);

        data_size = 0;
        assert(slip_encode_frame(buf_tx, sizeof(buf_tx), &buf_rx_size, data, data_size) == true);
        assert(buf_tx[0] == SLIP_BYTE_START);
        assert(buf_tx[1] == SLIP_BYTE_END);
        assert(buf_rx_size == 2);

        data_out_size = 0;
        for (i = 0; i < buf_rx_size; i++)
                assert(slip_decode_frame(&test_slip, buf_tx[i], data_out, sizeof(data_out), &data_out_size) == ((i == (buf_rx_size - 1)) ? true : false));
        assert(data_out_size == data_size);
        for (i = 0; i < data_out_size; i++)
                assert(data[i] == data_out[i]);
        
        data_out_size = 0;
        for (i = 0; i < buf_rx_size; i++)
                assert(slip_decode_frame(&test_slip, buf_tx[i], data_out, 0, &data_out_size) == ((i == (buf_rx_size - 1)) ? true : false));
        assert(data_out_size == data_size);
        for (i = 0; i < data_out_size; i++)
                assert(data[i] == data_out[i]);

        data[0] = 0xFF;
        data_size = 1;
        assert(slip_encode_frame(buf_tx, 3, &buf_rx_size, data, data_size) == true);
        assert(buf_tx[0] == SLIP_BYTE_START);
        assert(buf_tx[1] == 0xFF);
        assert(buf_tx[2] == SLIP_BYTE_END);
        assert(buf_rx_size == 3);

        data_out_size = 0;
        for (i = 0; i < buf_rx_size; i++)
                assert(slip_decode_frame(&test_slip, buf_tx[i], data_out, 1, &data_out_size) == ((i == (buf_rx_size - 1)) ? true : false));
        assert(data_out_size == data_size);
        for (i = 0; i < data_out_size; i++)
                assert(data[i] == data_out[i]);
        
        data_out_size = 0;
        for (i = 0; i < buf_rx_size; i++)
                assert(slip_decode_frame(&test_slip, buf_tx[i], data_out, 0, &data_out_size) == false);

        data[0] = 0xFF;
        data_size = 1;
        assert(slip_encode_frame(buf_tx, 2, &buf_rx_size, data, data_size) == false);

        data_out_size = 0;
        assert(slip_decode_frame(&test_slip, SLIP_BYTE_START, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, 0xFF, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, SLIP_BYTE_START, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, 0xAA, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, SLIP_BYTE_END, data_out, sizeof(data_out), &data_out_size) == true);
        assert(data_out_size == 1);
        assert(data_out[0] == 0xAA);        

        data_out_size = 0;
        assert(slip_decode_frame(&test_slip, SLIP_BYTE_START, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, SLIP_BYTE_ESCAPE, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, 0xAA, data_out, sizeof(data_out), &data_out_size) == false);
        assert(slip_decode_frame(&test_slip, SLIP_BYTE_END, data_out, sizeof(data_out), &data_out_size) == false);
        
	return 0;
}