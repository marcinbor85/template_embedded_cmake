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

#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

#include <stm32f4xx.h>

void gpio_port_enable_clock(const struct gpio *self)
{
        GPIO_TypeDef *gpio = (self->reg);
        uint32_t offset = (uint32_t)GPIOB - (uint32_t)GPIOA;
        uint32_t index = ((uint32_t)gpio - (uint32_t)GPIOA) / offset;
        RCC->AHB1ENR |= (1 << index);
}

void gpio_port_set_mode(const struct gpio *self)
{
        GPIO_TypeDef *gpio = (self->reg);
        gpio->MODER &= ~(0x03 << (self->pin << 1));

        if (self->mode == GPIO_MODE_INPUT)
                return;
        
        gpio->MODER |= (0x01 << (self->pin << 1));

        if (self->mode == GPIO_MODE_OUTPUT_PUSH_PULL) {
                gpio->OTYPER &= ~(0x01 << self->pin);
        } else {
                gpio->OTYPER |= (0x01 << self->pin);
        }
}

void gpio_port_set_pupd(const struct gpio *self)
{
        GPIO_TypeDef *gpio = (self->reg);
        gpio->PUPDR = (gpio->PUPDR & (~(0x03 << (self->pin << 1)))) | (self->pupd << (self->pin << 1));
}

void gpio_port_set_speed(const struct gpio *self)
{
        GPIO_TypeDef *gpio = (self->reg);
        gpio->OSPEEDR = (gpio->OSPEEDR & (~(0x03 << (self->pin << 1)))) | (self->speed << (self->pin << 1));
}

void gpio_port_set_level(const struct gpio *self, bool state)
{
        GPIO_TypeDef *gpio = (self->reg);
        volatile uint16_t *bsrr = (state != false) ? &(gpio->BSRRL) : &gpio->BSRRH;
        *bsrr = (1 << self->pin);
}

void gpio_port_toggle_state(const struct gpio *self)
{
        GPIO_TypeDef *gpio = (self->reg);
        gpio->ODR ^= (1 << self->pin);
}

bool gpio_port_get_level(const struct gpio *self)
{
        GPIO_TypeDef *gpio = self->reg;
        return ((gpio->IDR & (1 << self->pin)) != 0) ? true : false;
}
