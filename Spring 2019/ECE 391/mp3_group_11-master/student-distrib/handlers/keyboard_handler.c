#include "keyboard_handler.h"
#include "../PCB.h"
#include "../i8259.h"
#include "../lib.h"
#include "syscall_handlers.h"

#define keyboardIsReady (inb(KEYBOARD_STATUS_PORT) & 0x01)
#define get_scancode() (inb(KEYBOARD_DATA_PORT))
#define valid_scancode(scancode) (scancode >= 0 && scancode <= CODE_MAP_SIZE)
#define is_letter(character) (character >= 0x61 && character <= 0x7A)

static kstate keyboard_state = (kstate){
    .shift = FALSE,
    .alt = FALSE,
    .ctrl = FALSE,
    .caps = FALSE,
};
static int preboot_keyboard_buffer_count;
char preboot_keyboard_buffer[KEYBOARD_BUFFER_SIZE] = {NULL};

/* Maps scan codes to appropriate character */
const char code_map[CODE_MAP_SIZE] = {
    /* Initialize to '0' */
    [0x0 ... ZERO_INDEX(CODE_MAP_SIZE)] = '0',
    /* Insert scan codes */
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = '\n',
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x2B] = '\\',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0F] = ' ',
    [0x29] = '`',
    [0x39] = ' ',
};

const char to_upper[CODE_MAP_SIZE] = {
    /* Initialize to '0' */
    [0x0 ... ZERO_INDEX(CODE_MAP_SIZE)] = '0',
    /* Insert scan codes */
    [0x02] = '!',
    [0x03] = '@',
    [0x04] = '#',
    [0x05] = '$',
    [0x06] = '%',
    [0x07] = '^',
    [0x08] = '&',
    [0x09] = '*',
    [0x0A] = '(',
    [0x0B] = ')',
    [0x0C] = '_',
    [0x0D] = '+',
    [0x0F] = ' ',
    [0x10] = 'Q',
    [0x11] = 'W',
    [0x12] = 'E',
    [0x13] = 'R',
    [0x14] = 'T',
    [0x15] = 'Y',
    [0x16] = 'U',
    [0x17] = 'I',
    [0x18] = 'O',
    [0x19] = 'P',
    [0x1A] = '{',
    [0x1B] = '}',
    [0x1C] = '\n',
    [0x1E] = 'A',
    [0x1F] = 'S',
    [0x20] = 'D',
    [0x21] = 'F',
    [0x22] = 'G',
    [0x23] = 'H',
    [0x24] = 'J',
    [0x25] = 'K',
    [0x26] = 'L',
    [0x27] = ':',
    [0x28] = '"',
    [0x29] = '~',
    [0x2C] = 'Z',
    [0x2D] = 'X',
    [0x2E] = 'C',
    [0x2F] = 'V',
    [0x30] = 'B',
    [0x31] = 'N',
    [0x32] = 'M',
    [0x33] = '<',
    [0x34] = '>',
    [0x35] = '?',
    [0x2B] = '|',
    [0x39] = ' ',
};

/**
 * Keyboard initialization.
 * Enables IRQ1 port
 * @return {void}
 */
void init_keyboard() {
    enable_irq(IRQ_KEYBOARD);
}

/*
    clear_keyboard_buffer -- clears the keyboard buffer 
    INPUT: NONE
    OUTPUT: NONE
*/
void clear_keyboard_buffer() {
    int i;
    cli();
    // reset keyboard_buffer with all spaces
    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++) _keyboard_buffer_assign(i, NULL);
    _keyboard_buffer_count_assign(0);
    sti();
}

/*
    handle_backspace -- if the key pressed was a backspace, this function is called to correctly backspace
    INPUTS: NONE
    OUTPUTS: NONE
*/
void handle_backspace() {
    int curr_x = screen_x_get();
    int curr_y = screen_y_get();
    int8_t will_wrap = (curr_x == 0 && _keyboard_buffer_count != 0);

    /* Dont backspace if buffer is empty */
    if (_keyboard_buffer_count != 0) {
        if (will_wrap) {
            screen_x_set(ZERO_INDEX(NUM_COLS));
            screen_y_set(curr_y - 1);
            putc(' ');
            screen_x_set(ZERO_INDEX(NUM_COLS));
            screen_y_set(curr_y - 1);
            cursor_pcb->kbuffer_height = SINGLE;
        } else {
            screen_x_set(curr_x - 1);
            putc(' ');
            screen_x_set(curr_x - 1);
        }
        cli();
        _keyboard_buffer_count_assign(_keyboard_buffer_count - 1);
        _keyboard_buffer_assign(_keyboard_buffer_count, NULL);
        display_pcb()->kbuffer_update_flag = TRUE;
        sti();
    }
}

/**
 * Handles keyboard.
 * Reads in scan code from port and prints the corresponding char to the screen
 * @return {void}
 */
void handle_keyboard() {
    send_eoi(1); /* Write the EOI */
    if (keyboardIsReady) {
        int scancode = get_scancode(); /* Variable for scancode */
        uint8_t isCapital = (keyboard_state.shift ^ (keyboard_state.caps && is_letter(code_map[scancode])));
        uint8_t character = (scancode == PRESS(TAB)) ? ' ' : (isCapital) ? to_upper[scancode] : code_map[scancode];
        uint8_t numPrints = (scancode == PRESS(TAB)) ? 4 : 1;
        /* Handle scancode */
        switch (scancode) {
            case PRESS(LSHIFT):
            case PRESS(RSHIFT):
                return (void)(keyboard_state.shift = TRUE);
            case RELEASE(LSHIFT):
            case RELEASE(RSHIFT):
                return (void)(keyboard_state.shift = FALSE);
            case PRESS(CAPS):
                return (void)(keyboard_state.caps = !keyboard_state.caps);
            case PRESS(ALT):
                return (void)(keyboard_state.alt = TRUE);
            case RELEASE(ALT):
                return (void)(keyboard_state.alt = FALSE);
            case PRESS(CTRL):
                return (void)(keyboard_state.ctrl = TRUE);
            case RELEASE(CTRL):
                return (void)(keyboard_state.ctrl = FALSE);
            case PRESS(BACKSPACE):
                return (void)handle_backspace();
            default: {
                /* Handle Ctrl Hotkeys */
                if (keyboard_state.ctrl)
                    switch (scancode) {
                        case PRESS(L):
                            return (void)scroll_screen_up(screen_y_get() - display_pcb()->kbuffer_height + 1);
                        case PRESS(C):
                            return (void)(display_pcb()->halt_flag = TRUE);
                    }
                /* Handle Alt Hotkeys */
                if (keyboard_state.alt)
                    switch (scancode) {
                        case PRESS(FN1):
                            return (void)set_displayed_pid(SHELL_1_PID);
                        case PRESS(FN2):
                            return (void)set_displayed_pid(SHELL_2_PID);
                        case PRESS(FN3):
                            return (void)set_displayed_pid(SHELL_3_PID);
                    }
                /* Handle regular key prints */
                if (valid_scancode(scancode)) {
                    cli();
                    if (_keyboard_buffer_count < KEYBOARD_BUFFER_SIZE - 1) {
                        while (numPrints-- > 0) {
                            _keyboard_buffer_assign(_keyboard_buffer_count, character);
                            _keyboard_buffer_count_assign(_keyboard_buffer_count + 1);
                            display_pcb()->kbuffer_update_flag = TRUE;
                            putc(character);
                        }
                    }
                    if (character == '\n' || character == '\r') {
                        display_pcb()->kbuffer_enter_flag = TRUE;
                        display_pcb()->kbuffer_count = 0;
                        cursor_pcb->kbuffer_height = SINGLE;
                    }
                    sti();
                }
            }
        }
    }
}
