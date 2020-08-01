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

#include "user_led.h"

#include "hw/gpio.h"

extern const struct gpio user_led_gpio_green;

void user_led_init(void)
{
        gpio_init(&user_led_gpio_green);
}

void user_led_set(user_led_id led, bool state)
{
        switch (led) {
        case USER_LED_ID_GREEN:
                gpio_set_state(&user_led_gpio_green, state);
                break;
        }
}

void user_led_toggle(user_led_id led)
{
        switch (led) {
        case USER_LED_ID_GREEN:
                gpio_toggle_state(&user_led_gpio_green);
                break;
        }
}
