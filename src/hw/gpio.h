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

#ifndef HW_GPIO_H
#define HW_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_PUSH_PULL,
    GPIO_MODE_OUTPUT_OPEN_DRAIN,
} gpio_mode;

typedef enum {
    GPIO_PUPD_DISABLE = 0,
    GPIO_PUPD_PULL_UP,
    GPIO_PUPD_PULL_DOWN
} gpio_pupd;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH
} gpio_speed;

struct gpio {
        void *reg;
        uint8_t pin;

        gpio_mode mode;
        gpio_pupd pupd;
        gpio_speed speed;
};

void gpio_init(const struct gpio *self);
void gpio_set_state(const struct gpio *self, bool state);
void gpio_toggle_state(const struct gpio *self);
bool gpio_get_state(const struct gpio *self);

extern void gpio_port_enable_clock(const struct gpio *self);   
extern void gpio_port_set_mode(const struct gpio *self);
extern void gpio_port_set_pupd(const struct gpio *self);
extern void gpio_port_set_speed(const struct gpio *self);
extern void gpio_port_set_level(const struct gpio *self, bool state);
extern void gpio_port_toggle_state(const struct gpio *self);
extern bool gpio_port_get_level(const struct gpio *self);

#ifdef __cplusplus
}
#endif

#endif /* HW_GPIO_H */
