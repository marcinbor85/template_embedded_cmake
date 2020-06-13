#ifndef GPIO_H
#define GPIO_H

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

#endif /* GPIO_H */