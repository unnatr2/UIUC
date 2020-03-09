#define NUM_PCALLS 256
#ifndef CI_ASM
#include "../macro_unrolling.h"
#include "../types.h"

/* Extern declare all pcalls 0...NUM_PCALLS */
#define EXTERNAL_PCALL(num) \
    extern void pcall_interrupt_##num(void);
M_REPEAT_CALL(NUM_PCALLS, EXTERNAL_PCALL)

#endif
