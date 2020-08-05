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

#include "system/tick.h"
#include "system/init.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint32_t cntr;

void tick_init(void)
{
        uint16_t period;
  
        period = ((uint32_t)(SystemCoreClock / 1000UL) / 8);

        TCCR1B |= (1 << WGM12) | (1 << CS11);

        OCR1AH = (period >> 8);
        OCR1AL = period;

        TIMSK1 |= 1 << OCIE1A;

        sei();
}

uint32_t tick_get_value(void)
{
        uint32_t ret;
        
        TIMSK1 &= ~(1 << OCIE1A); 
        ret = cntr;
        TIMSK1 |= 1 << OCIE1A; 

        return ret;
}

void tick_isr(void)
{
        cntr++;
}
