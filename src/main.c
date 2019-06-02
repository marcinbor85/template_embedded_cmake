#include <stm32f4xx.h>

#include "stm32f4xx_conf.h"

#define LED_PIN GPIO_Pin_6

int main(void)
{
        GPIO_InitTypeDef  gpio;

        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

        gpio.GPIO_Pin = LED_PIN;
        gpio.GPIO_Mode = GPIO_Mode_OUT;
        gpio.GPIO_OType = GPIO_OType_PP;
        gpio.GPIO_Speed = GPIO_Speed_100MHz;
        gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOG, &gpio);

        while (1) {
                GPIOG->BSRRL = LED_PIN;
                GPIOG->BSRRH = LED_PIN;
        }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
        while (1) {};
}

#endif


