#include "fifo.h"

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
        // if (self->tail >= self->capacity)

        // memcpy(&((uint8_t*) queue->buf)[fifo->tail * queue->item_size], (uint8_t*) item, queue->item_size);
        return false;
}

bool fifo_get(struct fifo *self, void *item)
{
        return false;
}
