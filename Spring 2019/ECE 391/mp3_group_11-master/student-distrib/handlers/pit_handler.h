#ifndef PIT_HANDLER
#define PIT_HANDLER

#define PIT_FREQ 75
#define PIT_OPFREQ 1193180
#define PIT_COMMAND_PORT 0x43
#define PIT_DATA_PORT 0x40
#define PIT_CONTROL_WORD 0x34
#define BYTEMASK 0xFF
#define BYTESHIFT 8

#include "../types.h"

void init_pit(uint32_t frequency);
void handle_pit(void);

#endif
