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

#include "button.h"

#include <stddef.h>

static struct button *buttons = NULL;

void button_register(struct button *self, const struct button_descriptor *desc)
{
        self->debounce_tick = 0;
        self->press_tick = 0;
        self->counter = 0;
        self->desc = desc;
        self->is_pressed = false;
        self->state = BUTTON_STATE_RELEASE;

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

bool button_is_pressed_debounced(struct button *self)
{
        return self->is_pressed;
}

static void call_event_callback(struct button *button, button_event event)
{
        if (button->desc->event_callback)
                button->desc->event_callback(button, event, button->counter);
}

static void debounce_button(struct button *button)
{
        if (button->desc->is_pressed(button) == button->is_pressed) {
                button->debounce_tick = button_port_get_current_tick();
        } else if (button_port_get_current_tick() - button->debounce_tick >= button->desc->debounce_timeout) {
                button->debounce_tick = button_port_get_current_tick();
                button->is_pressed = !button->is_pressed;
                call_event_callback(button, (button->is_pressed != false) ? BUTTON_EVENT_PRESS : BUTTON_EVENT_RELEASE);
        }
}

static void handle_button(struct button *button)
{
        debounce_button(button);

        switch (button->state) {
        case BUTTON_STATE_RELEASE:
                if (button->is_pressed == false) {
                        if ((button->counter != 0) && (button_port_get_current_tick() - button->press_tick > button->desc->click_timeout))
                                button->counter = 0;
                        break;
                }
                button->state = BUTTON_STATE_PRESS;
                button->press_tick = button_port_get_current_tick();
                break;
        case BUTTON_STATE_PRESS:
                if (button->is_pressed == false) {
                        button->counter++;
                        button->state = BUTTON_STATE_RELEASE;
                        call_event_callback(button, BUTTON_EVENT_CLICK);
                        break;
                }
                if (button_port_get_current_tick() - button->press_tick < button->desc->hold_timeout)
                        break;
                call_event_callback(button, BUTTON_EVENT_HOLD);
                button->press_tick = button_port_get_current_tick();
                button->state = BUTTON_STATE_HOLD;
                break;
        case BUTTON_STATE_HOLD:
                if (button->is_pressed == false) {
                        button->state = BUTTON_STATE_RELEASE;
                        button->counter = 0;
                        break;
                }
                if (button_port_get_current_tick() - button->press_tick < button->desc->long_hold_timeout)
                        break;
                call_event_callback(button, BUTTON_EVENT_LONG_HOLD);
                button->press_tick = button_port_get_current_tick();
                button->state = BUTTON_STATE_LONG_HOLD;
                break;
        case BUTTON_STATE_LONG_HOLD:
                if (button->is_pressed == false) {
                        button->state = BUTTON_STATE_RELEASE;
                        button->counter = 0;
                }
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
