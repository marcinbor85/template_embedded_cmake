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

#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

struct timer;

typedef void (*timer_callback)(struct timer *self);

struct timer {
        uint32_t last_tick;
        uint32_t period;
        timer_callback callback;
        bool is_periodic;

        struct timer *next;
};

void timer_register(struct timer *self);
void timer_unregister(struct timer *self);
void timer_set_interval(struct timer *self, uint32_t period, timer_callback cb);
void timer_set_timeout(struct timer *self, uint32_t timeout, timer_callback cb);
void timer_change_period(struct timer *self, uint32_t period);
void timer_stop(struct timer *self);

void timer_service(void);

extern uint32_t timer_port_get_current_tick(void);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_TIMER_H */
