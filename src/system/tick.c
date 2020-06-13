#include "tick.h"
#include "init.h"

#include <stm32f4xx.h>

static volatile uint32_t cntr;

void tick_init(void)
{
        SysTick_Config(SystemCoreClock / 1000);
}

uint32_t tick_get_value(void)
{
        return cntr;
}

void tick_isr(void)
{
        cntr++;
}