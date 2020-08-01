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

#include "logic.h"

#include "bsp/user_led.h"
#include "bsp/user_button.h"

#include "utils/pulse.h"
#include "utils/button.h"
#include "utils/timer.h"

#include "bsp/user_uart.h"

struct timer led_start_timer;
struct timer led_blink_timer;

struct button button;

struct pulse pulse;

static void pulse_cb(struct pulse *pulse, bool state, int current_cycle)
{
        user_led_set(USER_LED_ID_GREEN, state);
}

static void toggle_led(struct timer *timer)
{
        user_led_toggle(USER_LED_ID_GREEN);
        user_uart_write_byte('?');
}

static bool is_button_pressed(struct button *button)
{
        return user_button_is_pressed(USER_BUTTON_ID_1);
}

static void event_callback(struct button *button, button_event event, int counter)
{
        switch (event) {
        case BUTTON_EVENT_CLICK:
                if (counter == 1) {
                        timer_set_interval(&led_blink_timer, 1000, toggle_led);
                } else if (counter == 2) {
                        timer_set_interval(&led_blink_timer, 500, toggle_led);
                } else if (counter == 3) {
                        timer_set_interval(&led_blink_timer, 200, toggle_led);
                }
                break;
        case BUTTON_EVENT_PRESS:
                break;
        case BUTTON_EVENT_RELEASE:
                break;
        case BUTTON_EVENT_HOLD:
                timer_stop(&led_blink_timer);
                pulse_trigger(&pulse, counter + 1, 0, 50, 200, pulse_cb);
                break;
        case BUTTON_EVENT_LONG_HOLD:
                break;
        }
}

static const struct button_descriptor button_desc = {
        .debounce_timeout = 20,
        .click_timeout = 500,
        .hold_timeout = 1000,
        .long_hold_timeout = 2000,
        .is_pressed = is_button_pressed,
        .event_callback = event_callback,
};

void logic_init(void)
{
        user_led_init();
        user_button_init();

        button_register(&button, &button_desc);
        pulse_register(&pulse);

        timer_register(&led_blink_timer);

        timer_set_interval(&led_blink_timer, 500, toggle_led);
}

void logic_service(void)
{
        
}
