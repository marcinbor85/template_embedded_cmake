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

#include "uart.h"

void uart_init(struct uart *self, const struct uart_descriptor *desc)
{
        self->desc = desc;

        fifo_init(&self->fifo_rx, self->desc->buf_rx, self->desc->buf_rx_size, 1);
        fifo_init(&self->fifo_tx, self->desc->buf_tx, self->desc->buf_tx_size, 1);

        uart_port_init(self);
}

bool uart_read_byte(struct uart *self, uint8_t *byte)
{
        bool status;

        uart_port_disable_rx_interrupt(self);
        status = fifo_get(&self->fifo_rx, byte);
        uart_port_enable_rx_interrupt(self);

        return status;
}

bool uart_write_byte(struct uart *self, uint8_t byte)
{
        bool status;

        uart_port_disable_tx_interrupt(self);
        status = fifo_put(&self->fifo_tx, &byte, false);
        uart_port_enable_tx_interrupt(self);

        return status;
}