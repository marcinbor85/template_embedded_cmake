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

#include "timer.h"

#include <stddef.h>

static struct timer *timers = NULL;

void timer_register(struct timer *self)
{
        self->last_tick = 0;
        self->period = 0;
        self->callback = NULL;

        self->next = timers;
        timers = self;
}

void timer_unregister(struct timer *self)
{
        struct timer *timer = timers;
        struct timer *prev = NULL;

        while (timer != NULL) {
                if (timer == self) {
                        if (prev == NULL) {
                                timers = timer->next;
                        } else {
                                prev->next = timer->next;
                        }
                        break;
                }
                prev = timer;
                timer = timer->next;
        }
}

void timer_set_interval(struct timer *self, uint32_t period, timer_callback cb)
{
        self->period = period;
        self->callback = cb;
        self->last_tick = timer_port_get_current_tick();
        self->is_periodic = true;
}

void timer_set_timeout(struct timer *self, uint32_t timeout, timer_callback cb)
{
        self->period = timeout;
        self->callback = cb;
        self->last_tick = timer_port_get_current_tick();
        self->is_periodic = false;
}

void timer_change_period(struct timer *self, uint32_t period)
{
        self->period = period;
}

void timer_stop(struct timer *self)
{
        self->callback = NULL;
        self->period = 0;
}

static void handle_timer(struct timer *timer)
{
        timer_callback callback;

        if (timer->callback == NULL)
                return;
        if (timer_port_get_current_tick() - timer->last_tick < timer->period)
                return;

        timer->last_tick = timer_port_get_current_tick();
        callback = timer->callback;
        if (timer->is_periodic == false)
                timer->callback = NULL;
        callback(timer);
}

void timer_service(void)
{
        struct timer *timer = timers;

        while (timer != NULL) {
                handle_timer(timer);
                timer = timer->next;
        }
}