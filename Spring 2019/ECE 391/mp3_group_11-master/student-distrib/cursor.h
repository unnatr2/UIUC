#ifndef CURSOR
#define CURSOR
#include "types.h"

void cursor_enable(uint8_t start, uint8_t end);
void cursor_disable();
void refresh_cursor();

#endif
