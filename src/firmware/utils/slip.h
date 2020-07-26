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

#ifndef UTILS_SLIP_H
#define UTILS_SLIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define SLIP_BYTE_START         0xC0
#define SLIP_BYTE_END           0xC1
#define SLIP_BYTE_ESCAPE        0xDB
#define SLIP_BYTE_ESCAPE_START  0xDC
#define SLIP_BYTE_ESCAPE_END    0xDD
#define SLIP_BYTE_ESCAPE_ESCAPE 0xDE

typedef enum {
        SLIP_STATE_START = 0,
        SLIP_STATE_DATABYTE,
        SLIP_STATE_ESCAPE
} slip_state;

struct slip {
        uint32_t rx_index;
        slip_state state;
};

void slip_init(struct slip *self);
bool slip_decode_frame(struct slip *self, uint8_t byte, uint8_t *output, uint32_t max_output_size, uint32_t *output_size);

bool slip_encode_frame(uint8_t *output, uint32_t max_output_size, uint32_t *output_size, uint8_t *input, uint32_t max_input_size);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_SLIP_H */
