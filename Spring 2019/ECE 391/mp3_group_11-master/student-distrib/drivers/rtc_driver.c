#include "rtc_driver.h"
#include "../PCB.h"
#include "../lib.h"

/**
 * Opens the RTC
 * 
 * @param {uint8_t} filename - File to open (syntactical/unused)
 * @return {int32_t} 0 for success and -1 for failure
 */
int32_t rtc_open(const uint8_t* filename) {
    return 0;
}

/**
 * Closes the RTC file descriptor.
 * 
 * @param {int32_t} fd - file descriptor to close
 * @return {int32_t} 0 for success
 */
int32_t rtc_close(int32_t fd) {
    return 0;
}

/**
 * Blocks until the next RTC interrupt is received.
 * 
 * @param {int32_t} fd - file descriptor (unused/syntactical)
 * @param {void*} buf - read buffer (unused/syntactical)
 * @param {int32_t} nbytes - number of bytes in the supplied buffer
 */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes) {
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Process frequency stored in file_position */
    uint32_t freq = process->fd_array[fd].file_position;

    /* Block execution for one interval at specified frequency */
    blockAtFreq(freq);

    return 0;
}

/**
 * Sets the frequency of the RTC.
 * 
 * @param {int32_t} fd - file descriptor (unused/syntactical)
 * @param {void*} buf - read buffer (unused/syntactical)
 * @param {int32_t} nbytes - number of bytes to read from buf
 */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes) {
    uint32_t freq;
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Extract frequency from buf by casting to size of nbytes */
    switch (nbytes) {
        case sizeof(uint8_t):
            freq = *(uint8_t*)buf;
            break;
        case sizeof(uint16_t):
            freq = *(uint16_t*)buf;
            break;
        case sizeof(uint32_t):
            freq = *(uint32_t*)buf;
            break;
        default:
            /* Size of frequency pointer not supported */
            return -1;
    }

    /* Set desired frequency of the process under file_position (not used otherwise) */
    process->fd_array[fd].file_position = freq;

    return 0;
}
