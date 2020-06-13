#ifndef TICK_H
#define TICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void tick_init(void);
uint32_t tick_get_value(void);
void tick_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* TICK_H */