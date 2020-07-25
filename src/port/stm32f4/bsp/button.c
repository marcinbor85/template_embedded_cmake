#include <stm32f4xx.h>

#include "hw/gpio.h"

const struct gpio user_button[] = {
        {
                .reg = GPIOA,
                .pin = 0,
                .mode = GPIO_MODE_INPUT,
                .speed = GPIO_SPEED_LOW,
                .pupd = GPIO_PUPD_DISABLE
        }
};
