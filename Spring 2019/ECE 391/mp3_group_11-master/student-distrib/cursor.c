#include "cursor.h"
#include "PCB.h"
#include "lib.h"

// https://wiki.osdev.org/Text_Mode_Cursor
#define NUM_COLS 80
#define NUM_ROWS 25
#define VGA_ADDRESS_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
#define CURSOR_START_REG 0x0A
#define CURSOR_END_REG 0x0B
#define BITMASK_76 0xC0          // 11000000
#define BITMASK_765 0xE0         // 11100000
#define CURSOR_DISABLE_BIT 0x20  // 00100000
#define CURSOR_LOCATION_LOW_REG 0x0F
#define CURSOR_LOCATION_HIGH_REG 0x0E
#define BYTE_MASK 0xFF  // 11111111
#define BYTE_SHIFT 8

/*
    cursor_enable
    INPUTS: start -- starting scanline of the cursor
            end -- ending scanline of the cursor
    OUTPUTS: NONE
*/
void cursor_enable(uint8_t start, uint8_t end) {
    outb(CURSOR_START_REG, VGA_ADDRESS_PORT);
    outb((inb(VGA_DATA_PORT) & BITMASK_76) | start, VGA_DATA_PORT);

    outb(CURSOR_END_REG, VGA_ADDRESS_PORT);
    outb((inb(VGA_DATA_PORT) & BITMASK_765) | end, VGA_DATA_PORT);
}

/*
    cursor_disable -- disables the cursos
    INPUTS: NONE
    OUTPUTS: NONE
*/
void cursor_disable() {
    outb(CURSOR_START_REG, VGA_ADDRESS_PORT);
    outb(CURSOR_DISABLE_BIT, VGA_DATA_PORT);
}

/*
    set_cursor -- set the cursor position
    INPUTS: x -- x coordinate
            y -- y coordinate
    OUTPUTS: NONE
*/
void refresh_cursor() {
    cli();
    /* Save and reset video override to move cursor to display position */
    int override_copy = video_override_pid;
    video_override_pid = ERRPID;
    uint16_t pos = _screen_y * NUM_COLS + _screen_x;
    outb(CURSOR_LOCATION_LOW_REG, VGA_ADDRESS_PORT);
    outb((uint8_t)(pos & BYTE_MASK), VGA_DATA_PORT);
    outb(CURSOR_LOCATION_HIGH_REG, VGA_ADDRESS_PORT);
    outb((uint8_t)((pos >> BYTE_SHIFT) & BYTE_MASK), VGA_DATA_PORT);
    /* Reset video override */
    video_override_pid = override_copy;
    sti();
}
