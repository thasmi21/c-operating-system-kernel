#ifndef _PROCESSUS_H_
#define _PROCESSUS_H_
#include <inttypes.h>

extern void ctx_sw(uint64_t a, uint64_t b);

void idle(void);

void proc1(void);

#endif