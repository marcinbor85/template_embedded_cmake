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

#ifndef UTILS_PULSE_H
#define UTILS_PULSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

struct pulse;

typedef void (*pulse_callback)(struct pulse *self, bool state, int current_cycle);

struct pulse {
        pulse_callback callback;

        uint32_t last_tick;
        uint32_t init_delay;
        uint32_t high_duration;
        uint32_t low_duration;
        int total_cycles;
        int current_cycle;
        bool state;

        struct pulse *next;
};

void pulse_register(struct pulse *self);
void pulse_unregister(struct pulse *self);
void pulse_trigger(struct pulse *self, int total_cycles, uint32_t init_delay, uint32_t high_duration, uint32_t low_duration, pulse_callback cb);
void pulse_cancel(struct pulse *self);
bool pulse_get_state(struct pulse *self);

void pulse_service(void);

extern uint32_t pulse_port_get_current_tick(void);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_PULSE_H */
