#include "button.h"

#include <stddef.h>

static struct button *buttons = NULL;

void button_register(struct button *self, const struct button_descriptor *desc)
{
        self->last_tick = 0;
        self->desc = desc;
        self->state = BUTTON_STATE_RELEASED;
        self->next = buttons;
        
        buttons = self;
}

void button_unregister(struct button *self)
{
        struct button *button = buttons;
        struct button *prev = NULL;

        while (button != NULL) {
                if (button == self) {
                        if (prev == NULL) {
                                buttons = button->next;
                        } else {
                                prev->next = button->next;
                        }
                        break;
                }
                prev = button;
                button = button->next;
        }
}

button_state button_get_state(struct button *self)
{
        return self->state;
}

static void handle_button(struct button *button)
{
        switch (button->state) {
        case BUTTON_STATE_RELEASED:
                if (button->desc->is_pressed(button) == false) {
                        button->last_tick = button_port_get_current_tick();
                        break;
                }
                if (button_port_get_current_tick() - button->last_tick < button->desc->debounce_timeout)
                        break;
                button->state = BUTTON_STATE_SHORT_PRESSED;
                button->last_tick = button_port_get_current_tick();
                break;
        case BUTTON_STATE_SHORT_PRESSED:
                if (button->desc->is_pressed(button) == false) {
                        button->state = BUTTON_STATE_DEBOUNCE;
                        button->last_tick = button_port_get_current_tick();
                        if (button->desc->short_press_callback)
                                button->desc->short_press_callback(button);
                        break;
                }
                if (button_port_get_current_tick() - button->last_tick < button->desc->long_timeout)
                        break;
                button->state = BUTTON_STATE_LONG_PRESSED;
                button->last_tick = button_port_get_current_tick();
                if (button->desc->long_press_callback)
                        button->desc->long_press_callback(button);
                break;
        case BUTTON_STATE_LONG_PRESSED:
                if (button->desc->is_pressed(button) == false) {
                        button->state = BUTTON_STATE_DEBOUNCE;
                        button->last_tick = button_port_get_current_tick();
                        break;
                }
                if (button_port_get_current_tick() - button->last_tick < button->desc->very_long_timeout)
                        break;
                button->state = BUTTON_STATE_VERY_LONG_PRESSED;
                button->last_tick = button_port_get_current_tick();
                if (button->desc->very_long_press_callback)
                        button->desc->very_long_press_callback(button);
                break;
        case BUTTON_STATE_VERY_LONG_PRESSED:
                if (button->desc->is_pressed(button) != false)
                        break;
                button->state = BUTTON_STATE_DEBOUNCE;
                button->last_tick = button_port_get_current_tick();
                break;
        case BUTTON_STATE_DEBOUNCE:
                if (button->desc->is_pressed(button) != false) {
                        button->last_tick = button_port_get_current_tick();
                        break;
                }
                if (button_port_get_current_tick() - button->last_tick < button->desc->debounce_timeout)
                        break;
                button->state = BUTTON_STATE_RELEASED;
                button->last_tick = button_port_get_current_tick();
                break;
        }
}

void button_service(void)
{
        struct button *button = buttons;

        while (button != NULL) {
                handle_button(button);
                button = button->next;
        }
}