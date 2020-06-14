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

#include "pulse.h"

#include <stddef.h>

static struct pulse *pulses = NULL;

void pulse_register(struct pulse *self)
{
        self->total_cycles = 0;
        self->current_cycle = 0;
        self->state = 0;
        self->last_tick = 0;
        self->high_duration = 0;
        self->low_duration = 0;
        self->init_delay = 0;
        self->callback = NULL;

        self->next = pulses;
        pulses = self;
}

void pulse_unregister(struct pulse *self)
{
        struct pulse *pulse = pulses;
        struct pulse *prev = NULL;

        while (pulse != NULL) {
                if (pulse == self) {
                        if (prev == NULL) {
                                pulses = pulse->next;
                        } else {
                                prev->next = pulse->next;
                        }
                        break;
                }
                prev = pulse;
                pulse = pulse->next;
        }
}

void pulse_trigger(struct pulse *self, int total_cycles, uint32_t init_delay, uint32_t high_duration, uint32_t low_duration, pulse_callback cb)
{
        if (total_cycles <= 0)
                return;

        self->total_cycles = total_cycles;
        self->current_cycle = 0;
        self->init_delay = init_delay;
        self->high_duration = high_duration;
        self->low_duration = low_duration;
        self->callback = cb;
        self->state = false;
        self->last_tick = pulse_port_get_current_tick();
}

void pulse_cancel(struct pulse *self)
{
        self->total_cycles = 0;
}

bool pulse_get_state(struct pulse *self)
{
        return self->state;
}

static void handle_pulse(struct pulse *pulse)
{
        uint32_t duration;
        
        if (pulse->total_cycles == 0)
                return;

        duration = (pulse->state == false) ? ((pulse->current_cycle == 0) ? pulse->init_delay : pulse->low_duration) : pulse->high_duration;

        if (pulse_port_get_current_tick() - pulse->last_tick < duration)
                return;

        pulse->state = !pulse->state;
        pulse->last_tick = pulse_port_get_current_tick();
        if (pulse->callback != NULL)
                pulse->callback(pulse, pulse->state, pulse->current_cycle);

        if (pulse->state != false)
                return;

        pulse->current_cycle++;
        if (pulse->current_cycle >= pulse->total_cycles)
                pulse->total_cycles = 0;
}

void pulse_service(void)
{
        struct pulse *pulse = pulses;

        while (pulse != NULL) {
                handle_pulse(pulse);
                pulse = pulse->next;
        }
}
