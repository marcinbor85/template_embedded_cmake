#ifndef INIT_H
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;

void init_system(void);

#ifdef __cplusplus
}
#endif

#endif /* INIT_H */
