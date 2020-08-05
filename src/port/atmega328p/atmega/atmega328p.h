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

#ifndef HW_ATMEGA328P_H
#define HW_ATMEGA328P_H

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>

#define __IO volatile

typedef struct {
    __IO uint8_t PIN;
    __IO uint8_t DDR;
    __IO uint8_t PORT; 
} GPIO_TypeDef;

typedef struct {
    __IO uint8_t UCSRA;
    __IO uint8_t UCSRB;
    __IO uint8_t UCSRC;
    __IO uint8_t _not_used_1;
    __IO uint16_t UBRR;
    __IO uint8_t UDR;
} USART_TypeDef;

#define USART_UCSRA_MPCM    (0x01)
#define USART_UCSRA_U2X     (0x02)
#define USART_UCSRA_UPE     (0x04)
#define USART_UCSRA_DOR     (0x08)
#define USART_UCSRA_FE      (0x10)
#define USART_UCSRA_UDRE    (0x20)
#define USART_UCSRA_TXC     (0x40)
#define USART_UCSRA_RXC     (0x80)

#define USART_UCSRB_TXB8    (0x01)
#define USART_UCSRB_RXB8    (0x02)
#define USART_UCSRB_UCSZ2   (0x04)
#define USART_UCSRB_TXEN    (0x08)
#define USART_UCSRB_RXEN    (0x10)
#define USART_UCSRB_UDRIE   (0x20)
#define USART_UCSRB_TXCIE   (0x40)
#define USART_UCSRB_RXCIE   (0x80)

#define USART_UCSRC_UCPOL   (0x01)
#define USART_UCSRC_UCSZ0   (0x02)
#define USART_UCSRC_UCPHA   (0x02)
#define USART_UCSRC_UCSZ1   (0x04)
#define USART_UCSRC_UDORD   (0x04)
#define USART_UCSRC_USBS    (0x08)
#define USART_UCSRC_UPM0    (0x10)
#define USART_UCSRC_UPM1    (0x20)
#define USART_UCSRC_UMSEL0  (0x40)
#define USART_UCSRC_UMSEL1  (0x80)

#define USART0_BASE  (0xC0)

#define USART0   ((USART_TypeDef *) USART0_BASE)

#define GPIOB_BASE  (0x23)
#define GPIOC_BASE  (0x26)
#define GPIOD_BASE  (0x29)

#define GPIOB   ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *) GPIOD_BASE)

#ifdef __cplusplus
}
#endif

#endif /* HW_ATMEGA328P_H */
