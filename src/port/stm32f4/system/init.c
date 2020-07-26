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

#include "system/init.h"

#include <stm32f4xx.h>

uint32_t SystemCoreClock = 168000000UL;
uint32_t APB2Clock = 84000000UL;

static void init_fpu(void)
{
        SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));  // set fpu coprocessor CP10 and CP11 full access
}

static void init_clock(void)
{
        // Prepare and reset defaults
        RCC->CR |= RCC_CR_HSION;
        RCC->CFGR = 0;
        RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON);
        RCC->PLLCFGR = 0x24003010;
        RCC->CR &= ~RCC_CR_HSEBYP;
        RCC->CIR = 0x00000000;

        RCC->CR |= RCC_CR_HSEON; 	                // HSE on
        while ((RCC->CR & RCC_CR_HSERDY) == 0) {}; 	// wait until HSE ready

        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;

        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE)) | RCC_CFGR_HPRE_DIV1;        // set AHB = 168 MHz
        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PPRE1)) | RCC_CFGR_PPRE1_DIV4;      // set APB1 = 42 MHz
        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PPRE2)) | RCC_CFGR_PPRE2_DIV2;      // set APB2 = 84 MHz

        RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLP)) | (((2 >> 1) - 1) << 16);   // set PLLP = 2
        RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLM)) | 8;                        // set PLLM = 8
        RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLN)) | (336 << 6);               // set PLLN = 336
        RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLQ)) | (7 << 24);                // set PLLQ = 7
        RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLSRC)) | RCC_PLLCFGR_PLLSRC_HSE; // set PLL source to HSE

        RCC->CR |= RCC_CR_PLLON;                        // PLL on
        while ((RCC->CR & RCC_CR_PLLRDY) == 0) {};      // wait until PLL ready

        FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_PLL; 	// select PLL output as system clock
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {};      // wait until clock switched to PLL
}

static void init_isr(void)
{
        SCB->VTOR = FLASH_BASE;
}

void init_system(void)
{
        init_fpu();
        init_clock();
        init_isr();
}
