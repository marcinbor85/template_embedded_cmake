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

#include <stm32f0xx.h>

uint32_t SystemCoreClock = 48000000UL;
uint32_t APB2Clock = 24000000UL;

static void init_clock(void)
{
        // Prepare and reset defaults
        RCC->CR |= RCC_CR_HSION;
        RCC->CFGR = 0;
        RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON);
        RCC->CR &= ~RCC_CR_HSEBYP;
        RCC->CIR = 0x00000000;

        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE)) | RCC_CFGR_HPRE_DIV1;       // set AHB = 48 MHz
        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PPRE)) | RCC_CFGR_PPRE_DIV2;       // set APB = 24 MHz

        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLMUL)) | ((12 - 2) << RCC_CFGR_PLLMUL_Pos);   // set PLLMUL = 12 (div = 2)
        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC)) | RCC_CFGR_PLLSRC_HSI_DIV2; // set PLL source to HSI

        RCC->CR |= RCC_CR_PLLON;                        // PLL on
        while ((RCC->CR & RCC_CR_PLLRDY) == 0) {};      // wait until PLL ready

        FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

        RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_PLL; 	// select PLL output as system clock
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {};      // wait until clock switched to PLL
}

void SystemInit(void)
{
        init_clock();
}
