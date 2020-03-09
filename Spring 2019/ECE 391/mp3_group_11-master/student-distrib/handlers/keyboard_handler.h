#ifndef KEYBOARD_HANDLER
#define KEYBOARD_HANDLER

#include "../types.h"

#define KEYBOARD_BUFFER_SIZE 128
#define IRQ1_ENABLE_MASK 0xFD      // 11111101; enables IRQ1 on the PIC
#define KEYBOARD_STATUS_PORT 0x64  // port that provides status of keyboard
#define KEYBOARD_DATA_PORT 0x60    // port that provides data of the pressed key
#define CODE_MAP_SIZE 0x3A         // maximum size of code_map
#define NUM_COLS 80                // cols of screen
#define NUM_ROWS 25                // rows of screen

#define PRESS(scancode) (scancode)
#define RELEASE(scancode) (scancode + 0x80)

#define BACKSPACE 0x0E
#define CTRL 0x1D
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define CAPS 0x3A
#define ALT 0x38

#define L 0x26
#define C 0x2E
#define FN1 0x3B
#define FN2 0x3C
#define FN3 0x3D
#define TAB 0x0F

/* Toggles for sourcing where to fetch process variables */
#define keyboard_buffer_pcb display_pcb()
#define keyboard_buffer_count_pcb display_pcb()

/* Definitions for process variables */
#define _keyboard_buffer (hasBooted ? keyboard_buffer_pcb->kbuffer : preboot_keyboard_buffer)
#define _keyboard_buffer_count (hasBooted ? keyboard_buffer_count_pcb->kbuffer_count : preboot_keyboard_buffer_count)
#define _keyboard_buffer_assign(index, value)        \
    if (hasBooted)                                   \
        keyboard_buffer_pcb->kbuffer[index] = value; \
    else                                             \
        preboot_keyboard_buffer[index] = value;
#define _keyboard_buffer_count_assign(val)              \
    if (hasBooted)                                      \
        keyboard_buffer_count_pcb->kbuffer_count = val; \
    else                                                \
        preboot_keyboard_buffer_count = val;

/* Structure for keeping track of keyboard state */
typedef struct kstate {
    uint8_t shift;
    uint8_t alt;
    uint8_t ctrl;
    uint8_t caps;
} kstate;

/* Enum containing possible keyboard action types */
typedef enum KACTION_TYPE {
    INSERT,
    REMOVE
} KACTION_TYPE;

/* Structure keeping track of actions on the keyboard */
typedef struct kaction {
    KACTION_TYPE type;
    uint8_t character;
    uint32_t pos;
} kaction;

enum terminal_lines { SINGLE = 1,
                      DOUBLE = 2 };

extern kaction* kaction_ll;

extern char preboot_keyboard_buffer[KEYBOARD_BUFFER_SIZE];

void init_keyboard(void);
void handle_keyboard(void);
void clear_keyboard_buffer(void);
void handle_backspace(void);
#endif
