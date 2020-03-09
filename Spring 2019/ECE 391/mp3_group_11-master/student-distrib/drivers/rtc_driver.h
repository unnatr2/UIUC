#ifndef RTC_DRIVER
#define RTC_DRIVER

#include "../handlers/rtc_handler.h"
#include "../lib.h"
#include "../types.h"

#define UPPER_MASK 0xF0

int32_t rtc_open(const uint8_t* RTC_DRIVER);

int32_t rtc_close(int32_t fd);

int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);

#endif
