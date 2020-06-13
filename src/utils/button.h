#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

struct button;

typedef bool (*button_is_pressed)(struct button *self);
typedef void (*button_press_callback)(struct button *self);

typedef enum {
        BUTTON_STATE_RELEASED = 0,
        BUTTON_STATE_SHORT_PRESSED,
        BUTTON_STATE_LONG_PRESSED,
        BUTTON_STATE_VERY_LONG_PRESSED,
        BUTTON_STATE_DEBOUNCE,
} button_state;

struct button_descriptor {
        button_is_pressed is_pressed;
        
        button_press_callback short_press_callback;
        button_press_callback long_press_callback;
        button_press_callback very_long_press_callback;

        uint32_t debounce_timeout;
        uint32_t long_timeout;
        uint32_t very_long_timeout;
};

struct button {
        struct button_descriptor const *desc;
        uint32_t last_tick;
        button_state state;

        struct button *next;
};

void button_register(struct button *self, const struct button_descriptor *desc);
void button_unregister(struct button *self);
button_state button_get_state(struct button *self);

void button_service(void);

extern uint32_t button_port_get_current_tick(void);

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */