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
