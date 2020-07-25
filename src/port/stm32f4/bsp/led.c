#include <stm32f4xx.h>

#include "hw/gpio.h"

const struct gpio led[] = {
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
