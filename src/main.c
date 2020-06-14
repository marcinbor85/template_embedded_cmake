#include <stm32f4xx.h>

#include "system/tick.h"

#include "hw/gpio.h"

#include "utils/timer.h"
#include "utils/button.h"

struct timer led_timer;
struct timer led_timer2;

struct button button;

static const struct gpio led[] = {
        {
                .reg = GPIOD,
                .pin = 14,
                .mode = GPIO_MODE_OUTPUT_PUSH_PULL,
                .speed = GPIO_SPEED_LOW,
                .pupd = GPIO_PUPD_DISABLE
        },
        {
                .reg = GPIOD,
                .pin = 15,
                .mode = GPIO_MODE_OUTPUT_PUSH_PULL,
                .speed = GPIO_SPEED_LOW,
                .pupd = GPIO_PUPD_DISABLE
        },
        {
                .reg = GPIOD,
                .pin = 13,
                .mode = GPIO_MODE_OUTPUT_PUSH_PULL,
                .speed = GPIO_SPEED_LOW,
                .pupd = GPIO_PUPD_DISABLE
        },
        {
                .reg = GPIOD,
                .pin = 12,
                .mode = GPIO_MODE_OUTPUT_PUSH_PULL,
                .speed = GPIO_SPEED_LOW,
                .pupd = GPIO_PUPD_DISABLE
        },
};

static const struct gpio user_button[] = {
        {
                .reg = GPIOA,
                .pin = 0,
                .mode = GPIO_MODE_INPUT,
                .speed = GPIO_SPEED_LOW,
                .pupd = GPIO_PUPD_DISABLE
        }
};

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
                break;
        case BUTTON_EVENT_PRESS:
                gpio_toggle_state(&led[3]);
                break;
        case BUTTON_EVENT_RELEASE:
                gpio_toggle_state(&led[1]);
                break;
        case BUTTON_EVENT_HOLD:
                gpio_toggle_state(&led[counter]);
                break;
        case BUTTON_EVENT_LONG_HOLD:
                if (counter == 0) {
                        timer_stop(&led_timer);
                } else {
                        timer_set_interval(&led_timer, 500, toggle_led);
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

        timer_register(&led_timer);
        timer_register(&led_timer2);

        // 
        timer_set_timeout(&led_timer2, 2000, enable_led);

        while (1) {
                switch (button_get_state(&button)) {
                case BUTTON_STATE_HOLD:
                        timer_change_period(&led_timer, 200);
                        break;
                case BUTTON_STATE_LONG_HOLD:
                        timer_change_period(&led_timer, 50);
                        break;
                default:
                        timer_change_period(&led_timer, 500);
                        break;
                }
                // gpio_set_state(&led[3], button_is_pressed_debounced(&button));
                timer_service();
                button_service();
        }
}
