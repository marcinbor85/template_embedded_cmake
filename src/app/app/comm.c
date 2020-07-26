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

#include "comm.h"

#include "bsp/user_uart.h"
#include "bsp/user_led.h"

#include "utils/timer.h"

struct timer uart_timer;

static void send_heartbeat(struct timer *timer)
{
        static uint8_t byte = 0;

        user_uart_write_byte(byte++);
        user_led_toggle(USER_LED_ID_ORANGE);
}

static void echo_service(void)
{
        uint8_t byte;

        if (user_uart_read_byte(&byte) == false)
                return;

        user_uart_write_byte(byte);
}

void comm_init(void)
{
        user_uart_init();
        
        timer_register(&uart_timer);
        timer_set_interval(&uart_timer, 1000, send_heartbeat);
}

void comm_service(void)
{
        echo_service();
}
