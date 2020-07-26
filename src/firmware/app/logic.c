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

struct timer led1_timer;
struct timer led2_timer;

struct button button;

struct pulse pulse;

static void pulse_cb(struct pulse *pulse, bool state, int current_cycle)
{
        user_led_set(USER_LED_ID_RED, state);
}

static void toggle_led(struct timer *timer)
{
        user_led_toggle(USER_LED_ID_RED);
}

static void enable_led(struct timer *timer)
{
        user_led_toggle(USER_LED_ID_BLUE);
}

static bool is_button_pressed(struct button *button)
{
        return user_button_is_pressed();
}

static void event_callback(struct button *button, button_event event, int counter)
{
        switch (event) {
        case BUTTON_EVENT_CLICK:
                user_led_toggle(USER_LED_ID_GREEN);
                pulse_trigger(&pulse, counter, 1000, 50, 200, pulse_cb);
                break;
        case BUTTON_EVENT_PRESS:
                pulse_cancel(&pulse);
                user_led_toggle(USER_LED_ID_ORANGE);
                break;
        case BUTTON_EVENT_RELEASE:
                user_led_toggle(USER_LED_ID_BLUE);
                break;
        case BUTTON_EVENT_HOLD:
                pulse_trigger(&pulse, counter + 1, 0, 50, 200, pulse_cb);
                break;
        case BUTTON_EVENT_LONG_HOLD:
                if (counter == 0) {
                        timer_stop(&led1_timer);
                } else {
                        timer_set_interval(&led1_timer, 500, toggle_led);
                }
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

        timer_register(&led1_timer);
        timer_register(&led2_timer);

        timer_set_timeout(&led2_timer, 2000, enable_led);
}

void logic_service(void)
{
        switch (button_get_state(&button)) {
        case BUTTON_STATE_HOLD:
                timer_change_period(&led1_timer, 200);
                break;
        case BUTTON_STATE_LONG_HOLD:
                timer_change_period(&led1_timer, 50);
                break;
        default:
                timer_change_period(&led1_timer, 500);
                break;
        }
}
