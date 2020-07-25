#include "system/tick.h"

#include "hw/gpio.h"

#include "bsp/led.h"
#include "bsp/button.h"

#include "utils/timer.h"
#include "utils/button.h"
#include "utils/pulse.h"

struct timer led1_timer;
struct timer led2_timer;

struct button button;

struct pulse pulse;

static void pulse_cb(struct pulse *pulse, bool state, int current_cycle)
{
        gpio_set_state(&led[0], state);
}

static void toggle_led(struct timer *timer)
{
        gpio_toggle_state(&led[0]);
}

static void enable_led(struct timer *timer)
{
        gpio_set_state(&led[1], true);
}

static bool is_button_pressed(struct button *button)
{
        return gpio_get_state(&user_button[0]);
}

static void event_callback(struct button *button, button_event event, int counter)
{
        switch (event) {
        case BUTTON_EVENT_CLICK:
                gpio_toggle_state(&led[2]);
                pulse_trigger(&pulse, counter, 1000, 50, 200, pulse_cb);
                break;
        case BUTTON_EVENT_PRESS:
                pulse_cancel(&pulse);
                gpio_toggle_state(&led[3]);
                break;
        case BUTTON_EVENT_RELEASE:
                gpio_toggle_state(&led[1]);
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

int main(void)
{
        tick_init();

        gpio_init(&led[0]);
        gpio_init(&led[1]);
        gpio_init(&led[2]);
        gpio_init(&led[3]);
        gpio_init(&user_button[0]);

        button_register(&button, &button_desc);

        pulse_register(&pulse);

        timer_register(&led1_timer);
        timer_register(&led2_timer);

        timer_set_timeout(&led2_timer, 2000, enable_led);

        while (1) {
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
                timer_service();
                button_service();
                pulse_service();
        }
}
