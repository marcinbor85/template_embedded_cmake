#include <stm32f4xx.h>

#include "system/tick.h"

#include "hw/gpio.h"

#include "utils/timer.h"
#include "utils/button.h"

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
        timer_unregister(timer);
}

static bool is_button_pressed(struct button *button)
{
        return gpio_get_state(&user_button[0]);
}

static void short_press(struct button *button)
{
        gpio_toggle_state(&led[2]);
}

static const struct button_descriptor button_desc = {
        .debounce_timeout = 20,
        .long_timeout = 1000,
        .very_long_timeout = 2000,
        .is_pressed = is_button_pressed,
        .short_press_callback = short_press,
};

int main(void)
{
        struct timer led_timer;
        struct timer led_timer2;

        struct button button;

        tick_init();

        gpio_init(&led[0]);
        gpio_init(&led[1]);
        gpio_init(&led[2]);
        gpio_init(&user_button[0]);

        button_register(&button, &button_desc);

        timer_register(&led_timer);
        timer_register(&led_timer2);

        timer_set_interval(&led_timer, 500, toggle_led);
        timer_set_timeout(&led_timer2, 2000, enable_led);

        while (1) {
                switch (button_get_state(&button)) {
                case BUTTON_STATE_LONG_PRESSED:
                        timer_change_period(&led_timer, 200);
                        break;
                case BUTTON_STATE_VERY_LONG_PRESSED:
                        timer_change_period(&led_timer, 50);
                        break;
                default:
                        timer_change_period(&led_timer, 500);
                        break;
                }
                timer_service();
                button_service();
        }
}
