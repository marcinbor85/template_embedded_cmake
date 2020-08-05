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

#include "hw/uart.h"

#include "system/init.h"

#include <stdint.h>
#include <stdbool.h>

#include <atmega328p.h>

void uart_port_init(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;

        if (usart == USART0) {

                GPIOD->DDR &= ~0x01; //RX
                GPIOD->PORT |= 0x01;

                GPIOD->DDR |= 0x02; //TX
                GPIOD->PORT |= 0x02;

                usart->UBRR = (SystemCoreClock / (self->desc->baudrate * 8UL) - 1UL);
               
                usart->UCSRA = USART_UCSRA_U2X;
                usart->UCSRC = USART_UCSRC_UCSZ0 | USART_UCSRC_UCSZ1;
                usart->UCSRB = USART_UCSRB_RXEN | USART_UCSRB_TXEN | USART_UCSRB_RXCIE;
        } else {
                return;
        }
}

void uart_port_disable_rx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->UCSRB &= ~USART_UCSRB_RXCIE;
}

void uart_port_enable_rx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->UCSRB |= USART_UCSRB_RXCIE;
}

void uart_port_disable_tx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->UCSRB &= ~USART_UCSRB_UDRIE;
}

void uart_port_enable_tx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->UCSRB |= USART_UCSRB_UDRIE;
}

void uart_port_rx_isr(struct uart *self)
{
        uint8_t byte;
        USART_TypeDef *usart = self->desc->reg;

        byte = usart->UDR;
        fifo_put(&self->fifo_rx, &byte, false);
}

void uart_port_tx_isr(struct uart *self)
{
        uint8_t byte;
        USART_TypeDef *usart = self->desc->reg;

        if (fifo_get(&self->fifo_tx, &byte) == false) {
                usart->UCSRB &= ~USART_UCSRB_UDRIE;
        } else {
                usart->UDR = byte;
        }
}
