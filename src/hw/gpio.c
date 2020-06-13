#include "gpio.h"

void gpio_init(const struct gpio *self)
{
        gpio_port_enable_clock(self);
        gpio_port_set_mode(self);
        gpio_port_set_pupd(self);
        gpio_port_set_speed(self);        
}

void gpio_set_state(const struct gpio *self, bool state)
{
        gpio_port_set_level(self, state);
}

void gpio_toggle_state(const struct gpio *self)
{
        gpio_port_toggle_state(self);
}

bool gpio_get_state(const struct gpio *self)
{
        return gpio_port_get_level(self);
}
