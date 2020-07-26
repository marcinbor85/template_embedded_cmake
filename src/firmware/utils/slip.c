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

#include "slip.h"

void slip_init(struct slip *self)
{
        self->state = SLIP_STATE_START;
        self->rx_index = 0;
}

bool slip_decode_frame(struct slip *self, uint8_t byte, uint8_t *output, uint32_t max_output_size, uint32_t *output_size)
{
        switch (self->state) {
        case SLIP_STATE_START:
                if (byte == SLIP_BYTE_START) {
                        self->rx_index = 0;
                        self->state = SLIP_STATE_DATABYTE;
                }
                break;
        case SLIP_STATE_DATABYTE:
                switch (byte) {
                case SLIP_BYTE_START:
                        self->rx_index = 0;
                        break;
                case SLIP_BYTE_END:
                        *output_size = self->rx_index;
                        self->state = SLIP_STATE_START;
                        return true;
                case SLIP_BYTE_ESCAPE:
                        self->state = SLIP_STATE_ESCAPE;
                        break;
                default:
                        if (self->rx_index >= max_output_size) {
                                self->state = SLIP_STATE_START;
                                break;
                        }
                        output[self->rx_index++] = byte;
                        break;
                }
                break;
        case SLIP_STATE_ESCAPE:
                switch (byte) {
                case SLIP_BYTE_ESCAPE_START:
                        if (self->rx_index >= max_output_size) {
                                self->state = SLIP_STATE_START;
                                break;
                        }
                        output[self->rx_index++] = SLIP_BYTE_START;
                        self->state = SLIP_STATE_DATABYTE;
                        break;
                case SLIP_BYTE_ESCAPE_END:
                        if (self->rx_index >= max_output_size) {
                                self->state = SLIP_STATE_START;
                                break;
                        }
                        output[self->rx_index++] = SLIP_BYTE_END;
                        self->state = SLIP_STATE_DATABYTE;
                        break;
                case SLIP_BYTE_ESCAPE_ESCAPE:
                        if (self->rx_index >= max_output_size) {
                                self->state = SLIP_STATE_START;
                                break;
                        }
                        output[self->rx_index++] = SLIP_BYTE_ESCAPE;
                        self->state = SLIP_STATE_DATABYTE;
                        break;
                default:
                        self->state = SLIP_STATE_START;
                        break;
                }
                break;
        }
        return false;
}

bool slip_encode_frame(uint8_t *output, uint32_t max_output_size, uint32_t *output_size, uint8_t *input, uint32_t input_size)
{
        uint8_t byte;
        uint32_t size = 0;

        if (size >= max_output_size)
                return false;
        output[size++] = SLIP_BYTE_START;

        while (input_size-- > 0) {
                byte = *input++;
                switch (byte) {
                case SLIP_BYTE_START:
                        if (size >= max_output_size)
                                return false;
                        output[size++] = SLIP_BYTE_ESCAPE;
                        if (size >= max_output_size)
                                return false;
                        output[size++] = SLIP_BYTE_ESCAPE_START;
                        break;
                case SLIP_BYTE_END:
                        if (size >= max_output_size)
                                return false;
                        output[size++] = SLIP_BYTE_ESCAPE;
                        if (size >= max_output_size)
                                return false;
                        output[size++] = SLIP_BYTE_ESCAPE_END;
                        break;
                case SLIP_BYTE_ESCAPE:
                        if (size >= max_output_size)
                                return false;
                        output[size++] = SLIP_BYTE_ESCAPE;
                        if (size >= max_output_size)
                                return false;
                        output[size++] = SLIP_BYTE_ESCAPE_ESCAPE;
                        break;
                default:
                        if (size >= max_output_size)
                                return false;
                        output[size++] = byte;
                        break;
                }
        }

        if (size >= max_output_size)
                return false;        
        output[size++] = SLIP_BYTE_END;
        *output_size = size;

        return true;
}
