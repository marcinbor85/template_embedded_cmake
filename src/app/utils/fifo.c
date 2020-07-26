#include "fifo.h"

#include <string.h>

void fifo_init(struct fifo *self, void *buf, uint32_t capacity, uint32_t item_size)
{
        self->buf = buf;
        self->capacity = capacity;
        self->item_size = item_size;

        self->count = 0;
        self->head = 0;
        self->tail = 0;
}

bool fifo_put(struct fifo *self, void *item, bool force)
{
        bool overflow = false;

        if (self->count >= self->capacity) {
                if (force == false)
                        return false;
                overflow = true;
        }

        memcpy(&((uint8_t*)self->buf)[self->tail * self->item_size], (uint8_t*)item, self->item_size);

        if (++self->tail >= self->capacity)
		self->tail = 0;

        if (overflow == false) {
                self->count++;
        } else {
                if (++self->head >= self->capacity)
		        self->head = 0;
        }

        return (overflow == false) ? true : false;
}

bool fifo_get(struct fifo *self, void *item)
{
	if (self->count == 0)
		return false;

	memcpy((uint8_t*)item, &((uint8_t*)self->buf)[self->head * self->item_size], self->item_size);

	if (++self->head >= self->capacity)
		self->head = 0;
	self->count--;

        return true;
}
