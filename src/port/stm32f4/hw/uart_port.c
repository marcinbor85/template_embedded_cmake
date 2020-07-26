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

#define USART1_PIN_TX   6
#define USART1_PIN_RX   7
#define USART1_GPIO     GPIOB

#define CONFIG_AFR(reg, pin, afr) reg->AFR[pin >> 3] = ((reg->AFR[pin >> 3] & (~(0x0F << ((pin & 0x07) << 2)))) | (afr << ((pin & 0x07) << 2)))
#define CONFIG_FIELD(reg, field, pin, mode) reg->field = ((reg->field & (~(0x03 << (pin << 1)))) | (mode << (pin << 1)))

static void config_uart_pins(GPIO_TypeDef *reg, uint8_t pin_rx, uint8_t pin_tx, uint8_t afr)
{
        CONFIG_AFR(reg, pin_tx, afr);
        CONFIG_AFR(reg, pin_rx, afr);
        CONFIG_FIELD(reg, MODER, pin_tx, 0x02);
        CONFIG_FIELD(reg, MODER, pin_rx, 0x02);
        CONFIG_FIELD(reg, OTYPER, pin_tx, 0x00);
        CONFIG_FIELD(reg, OTYPER, pin_rx, 0x00);
        CONFIG_FIELD(reg, PUPDR, pin_tx, 0x01);
        CONFIG_FIELD(reg, PUPDR, pin_rx, 0x01);
}

void uart_port_init(struct uart *self)
{
        USART_TypeDef *usart = self->desc->reg;

        if (usart == USART1) {
                RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
                RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

                config_uart_pins(USART1_GPIO, USART1_PIN_TX, USART1_PIN_RX, 0x07);

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
