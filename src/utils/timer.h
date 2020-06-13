#ifndef TIMER_H
#define TIMER_H

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

#endif /* TIMER_H */