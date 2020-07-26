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

#include <stdint.h>
#include <stdbool.h>

#include <stm32f4xx.h>

extern uint32_t APB2Clock;

void uart_port_init(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;

        if (usart == USART1) {
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
                RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

                GPIOB->AFR[0] = (GPIOB->AFR[0] & (~(0x0F << (6 << 2)))) | (0x07 << (6 << 2));   // PB6 Uart1Tx
                GPIOB->AFR[0] = (GPIOB->AFR[0] & (~(0x0F << (7 << 2)))) | (0x07 << (7 << 2));   // PB7 Uart1Rx
                
                GPIOB->MODER = (GPIOB->MODER & (~(0x03 << (6 << 1)))) | (0x02 << (6 << 1));     
                GPIOB->MODER = (GPIOB->MODER & (~(0x03 << (7 << 1)))) | (0x02 << (7 << 1));     

                GPIOB->OTYPER = (GPIOB->OTYPER & (~(0x03 << (6 << 1)))) | (0x00 << (6 << 1));
                GPIOB->OTYPER = (GPIOB->OTYPER & (~(0x03 << (7 << 1)))) | (0x00 << (7 << 1));

                GPIOB->PUPDR = (GPIOB->PUPDR & (~(0x03 << (6 << 1)))) | (0x01 << (6 << 1));
                GPIOB->PUPDR = (GPIOB->PUPDR & (~(0x03 << (7 << 1)))) | (0x01 << (7 << 1));

                NVIC_EnableIRQ(USART1_IRQn);
        } else {
                return;
        }

        usart->BRR = APB2Clock / self->desc->baudrate;
        
        usart->CR1 |= USART_CR1_RXNEIE;
        usart->CR1 |= USART_CR1_TE | USART_CR1_RE;
        usart->CR1 |= USART_CR1_UE;
}

void uart_port_disable_rx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->CR1 &= ~USART_CR1_RXNEIE;
}

void uart_port_enable_rx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->CR1 |= USART_CR1_RXNEIE;
}

void uart_port_disable_tx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->CR1 &= ~USART_CR1_TXEIE;
}

void uart_port_enable_tx_interrupt(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;
        usart->CR1 |= USART_CR1_TXEIE;
}

void uart_port_rx_isr(struct uart *self)
{
        uint8_t byte;
        USART_TypeDef *usart = self->desc->reg;

        if ((usart->SR & USART_SR_RXNE) != 0) {
                byte = usart->DR;
                fifo_put(&self->fifo_rx, &byte, false);
        }
}

void uart_port_tx_isr(struct uart *self)
{
        uint8_t byte;
        USART_TypeDef *usart = self->desc->reg;

        if ((usart->SR & USART_SR_TXE) != 0) {
                if (fifo_get(&self->fifo_tx, &byte) == false) {
                        usart->CR1 &= ~USART_CR1_TXEIE;
                } else {
                        usart->DR = byte;
                }
                usart->SR &= ~USART_SR_TXE;
        }
}
