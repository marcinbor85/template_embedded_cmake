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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "utils/fifo.h"

int main(int argc, char **argv)
{
        int buf[4];
        struct fifo test_fifo;
        int item;

        fifo_init(&test_fifo, buf, sizeof(buf)/sizeof(buf[0]), sizeof(buf[0]));

        assert(test_fifo.buf == buf);
        assert(test_fifo.capacity == sizeof(buf)/sizeof(buf[0]));
        assert(test_fifo.item_size == sizeof(buf[0]));

        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 0);
        
        item = 1;
        assert(fifo_put(&test_fifo, &item, false) == true);
        assert(test_fifo.count == 1);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 0);
        item = 2;
        assert(fifo_put(&test_fifo, &item, false) == true);
        assert(test_fifo.count == 2);
        assert(test_fifo.tail == 2);
        assert(test_fifo.head == 0);
        item = 3;
        assert(fifo_put(&test_fifo, &item, false) == true);
        assert(test_fifo.count == 3);
        assert(test_fifo.tail == 3);
        assert(test_fifo.head == 0);
        item = 4;
        assert(fifo_put(&test_fifo, &item, false) == true);
        assert(test_fifo.count == 4);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 0);
        item = 5;
        assert(fifo_put(&test_fifo, &item, false) == false);
        assert(test_fifo.count == 4);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 0);
        
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 1);
        assert(test_fifo.count == 3);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 1);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 2);
        assert(test_fifo.count == 2);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 2);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 3);
        assert(test_fifo.count == 1);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 3);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 4);
        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 0);
        assert(fifo_get(&test_fifo, &item) == false);
        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 0);

        item = 11;
        assert(fifo_put(&test_fifo, &item, true) == true);
        assert(test_fifo.count == 1);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 0);
        item = 12;
        assert(fifo_put(&test_fifo, &item, true) == true);
        assert(test_fifo.count == 2);
        assert(test_fifo.tail == 2);
        assert(test_fifo.head == 0);
        item = 13;
        assert(fifo_put(&test_fifo, &item, true) == true);
        assert(test_fifo.count == 3);
        assert(test_fifo.tail == 3);
        assert(test_fifo.head == 0);
        item = 14;
        assert(fifo_put(&test_fifo, &item, true) == true);
        assert(test_fifo.count == 4);
        assert(test_fifo.tail == 0);
        assert(test_fifo.head == 0);
        item = 15;
        assert(fifo_put(&test_fifo, &item, true) == false);
        assert(test_fifo.count == 4);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 1);

        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 12);
        assert(test_fifo.count == 3);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 2);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 13);
        assert(test_fifo.count == 2);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 3);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 14);
        assert(test_fifo.count == 1);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 0);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 15);
        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 1);
        assert(fifo_get(&test_fifo, &item) == false);
        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 1);
        assert(test_fifo.head == 1);

        item = 21;
        assert(fifo_put(&test_fifo, &item, false) == true);
        assert(test_fifo.count == 1);
        assert(test_fifo.tail == 2);
        assert(test_fifo.head == 1);
        assert(fifo_get(&test_fifo, &item) == true);
        assert(item == 21);
        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 2);
        assert(test_fifo.head == 2);
        assert(fifo_get(&test_fifo, &item) == false);
        assert(test_fifo.count == 0);
        assert(test_fifo.tail == 2);
        assert(test_fifo.head == 2);
        
	return 0;
}