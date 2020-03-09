#ifndef RTC_HANDLER
#define RTC_HANDLER

#include "../types.h"

#define MIN_FREQUENCY 2
#define DEFAULT_FREQUENCY 2
#define MAX_FREQUENCY 8192
#define SYS_FREQUENCY 512
#define CMOS_PORT 0x71
#define RTC_PORT 0x70
#define REG_A 0x0A
#define REG_B 0x0B
#define REG_C 0x0C

#define MS_TO_HZ(ms) (1000 / ms)

void init_rtc(void);
void handle_rtc(void);
void blockAtFreq(uint32_t hz);

#endif
