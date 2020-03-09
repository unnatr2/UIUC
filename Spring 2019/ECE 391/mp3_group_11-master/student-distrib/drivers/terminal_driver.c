#include "terminal_driver.h"
#include "../PCB.h"
#include "../cursor.h"
#include "../handlers/keyboard_handler.h"
#include "../handlers/rtc_handler.h"
#include "../i8259.h"
#include "../lib.h"

/* terminal_init
 * Clears screen, sets start to (0,0). Set cursor to start. Enable IRQ for keyboard interrupts
 * Side Effects: Moves cursor and clears the screen buffer
 * Inputs: none
 * Outputs: none
 */
void terminal_init(void) {
    clear();
    screen_x_set(0);
    screen_y_set(0);
}

/* terminal_open
 * Open terminal and clear keyboard buffer
 * Inputs: filename -- unused for Checkpoint 2
 * Outputs: 0 if passed
 *          -1 if failed
 */
int32_t terminal_open(const uint8_t* filename) {
    cli();
    clear_keyboard_buffer();
    running_pcb()->kbuffer_height = SINGLE;
    sti();
    return 0;
}

/* terminal_close
 * Close terminal
 * Inputs: fd -- unused for Checkpoint 2
 * Outputs: 0 if passed
 *          -1 if failed
 */
int32_t terminal_close(int32_t fd) {
    cli();
    running_pcb()->kbuffer_height = SINGLE;
    sti();
    return 0;
}

/* terminal_write
 * Write to file fd
 * Inputs: fd -- unused for Checkpoint 2
 *         buf -- buffer to print to screen
 *         nbytes -- number of bytes to write
 * Outputs: number of bytes written
 *          -1 if failed
 */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes) {
    uint8_t* terminal_buf = (uint8_t*)buf;
    uint32_t override_copy;
    int i;

    /* Validate parameters */
    if (terminal_buf == NULL || nbytes < 0) return -1;

    /* Prevent spam writing */
    blockAtFreq(SYS_FREQUENCY);

    { /* Critical Section begin */
        cli();
        /* Print to running process, not displayed one (default) */
        override_copy = video_override_pid;
        video_override_pid = get_running_pid();
        for (i = 0; i < nbytes; i++) putc(terminal_buf[i]);
        video_override_pid = override_copy;
        sti();
    } /* Critical Section end */

    /* Return bytes written */
    return nbytes;
}

/* terminal_read
 * Read nbytes from keyboard buffer and save to buf
 * Inputs: fd -- unused for Checkpoint 2
 *         buf -- buffer to save bytes read
 *         nbytes -- unused for Checkpoint 2
 * Outputs: number of bytes read
 *          -1 if failed
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes) {
    uint32_t bytes_to_copy = (nbytes < KEYBOARD_BUFFER_SIZE) ? nbytes : KEYBOARD_BUFFER_SIZE;
    uint8_t* terminal_buf = (uint8_t*)buf;
    int bytes_read = 0;
    int i;

    /* Validate parameters */
    if (_keyboard_buffer == NULL || terminal_buf == NULL) return -1;

    /* Clear out provided Buffer */
    for (i = 0; i < nbytes; i++) terminal_buf[i] = NULL;

    /* Wait for enter to be pressed */
    while (running_pcb()->kbuffer_enter_flag == FALSE || running_pcb()->kbuffer_update_flag == TRUE) {
        cli();
        if (running_pcb()->kbuffer_update_flag == TRUE) {
            strncpy((int8_t*)terminal_buf, running_pcb()->kbuffer, bytes_to_copy);
            running_pcb()->kbuffer_update_flag = FALSE;
        }
        sti();
    }

    /* Reset enter flag and keyboard buffer */
    cli();
    running_pcb()->kbuffer_enter_flag = FALSE;
    running_pcb()->kbuffer_height = SINGLE;
    clear_keyboard_buffer();
    sti();

    /* Count the number of bytes read */
    while (bytes_read < bytes_to_copy && terminal_buf[bytes_read] != NULL) bytes_read++;

    // Return bytes read
    return bytes_read;
}
