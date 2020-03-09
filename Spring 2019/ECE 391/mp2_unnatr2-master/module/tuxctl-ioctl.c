/*
 * tuxctl-ioctl.c
 *
 * Driver (skeleton) for the mp2 tuxcontrollers for ECE391 at UIUC.
 *
 * Mark Murphy 2006
 * Andrew Ofisher 2007
 * Steve Lumetta 12-13 Sep 2009
 * Puskar Naha 2013
 */

#include <asm/current.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/miscdevice.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
// #include <linux/spinlock.h>

#include "tuxctl-ld.h"
#include "tuxctl-ioctl.h"
#include "mtcp.h"

#define debug(str, ...) printk(KERN_DEBUG "%s: " str, __FUNCTION__, ##__VA_ARGS__)

// Macros for tux_buttons
#define NULL_PTR 0
#define LOW_4_BITS 0x0F
#define GET_BIT_2 0x02
#define GET_BIT_4 0x04
#define MASK_LEFT_DOWN_BITS 0x09
#define INVERT 0xFF

// Macro for tux_init
#define BUTTON_INIT 0x00

// Macros for tux_set_led
#define BIT_MASK 0x1
#define FOUR_BITMASK 0x000F
#define TWO_BYTEMASK 0xFFFF
#define MOVE_FOUR_BITS 4
#define MOVE_ONE_BIT 1
#define DECIMAL_ON 0x10
#define MOVE_TO_LED 16
#define MOVE_TO_DECIMAL 24
#define LOOP_ALL_LEDS 4
#define BLANK_LED 0x00
#define SET_ALL_LEDS 0x0F

// Declare functions
int tux_init(struct tty_struct *tty);
int tux_buttons(struct tty_struct *tty, unsigned long arg);
int tux_set_led(struct tty_struct *tty, unsigned long arg);
void mtcp_reset(struct tty_struct *tty);

// Declare static variables
static unsigned char button_packets[2];
static unsigned char tux_button_state;
static int save_left_bit;
static int save_down_bit;
static int led_save;
static int wait = 0;
static int reset = 0;

spinlock_t tux_lock;

// Set display hex values
unsigned char LED_DISPLAY_VALS[16] = {0xE7, 0x06, 0xCB, 0x8F, 0x2E, 0xAD, 0xED, 0x86, 0xEF, 0xAF, 0xEE, 0x6D, 0xE1, 0x4F, 0xE9, 0xE8};

/************************ Protocol Implementation *************************/

/* tuxctl_handle_packet()
 * IMPORTANT : Read the header for tuxctl_ldisc_data_callback() in
 * tuxctl-ld.c. It calls this function, so all warnings there apply
 * here as well.
 */
void tuxctl_handle_packet(struct tty_struct *tty, unsigned char *packet)
{
    unsigned a, b, c;

    a = packet[0]; /* Avoid printk() sign extending the 8-bit */
    b = packet[1]; /* values when printing them. */
    c = packet[2];

    switch (a)
    {
    case MTCP_BIOC_EVENT:
        // Set button packets
        spin_lock(&tux_lock);
        button_packets[0] = b;
        button_packets[1] = c;
        // Operate to store button state
        tux_button_state &= BUTTON_INIT; // 00000000
        b &= LOW_4_BITS;                 // 0000CBAS
        c &= LOW_4_BITS;                 // 0000RDLU
        save_left_bit = c & GET_BIT_2;   // 000000L0
        save_left_bit <<= MOVE_ONE_BIT;  // 00000L00
        save_down_bit = c & GET_BIT_4;   // 00000D00
        save_down_bit >>= MOVE_ONE_BIT;  // 000000D0
        c &= MASK_LEFT_DOWN_BITS;        // 0000R00U
        c |= save_left_bit;              // 0000RL0U
        c |= save_down_bit;              // 0000RLDU
        c <<= MOVE_FOUR_BITS;            // RLDU0000
        tux_button_state = b | c;        // RLDUCBAS
        tux_button_state ^= INVERT;
        spin_unlock(&tux_lock);
        break;
    case MTCP_ACK:
        // Update wait
        spin_lock(&tux_lock);
        wait = 0;
        spin_unlock(&tux_lock);
        break;
    case MTCP_RESET:
        // Reset tux
        mtcp_reset(tty);
        break;
    default:
        return;
    }
}

/******** IMPORTANT NOTE: READ THIS BEFORE IMPLEMENTING THE IOCTLS ************
 *                                                                            *
 * The ioctls should not spend any time waiting for responses to the commands *
 * they send to the controller. The data is sent over the serial line at      *
 * 9600 BAUD. At this rate, a byte takes approximately 1 millisecond to       *
 * transmit; this means that there will be about 9 milliseconds between       *
 * the time you request that the low-level serial driver send the             *
 * 6-byte SET_LEDS packet and the time the 3-byte ACK packet finishes         *
 * arriving. This is far too long a time for a system call to take. The       *
 * ioctls should return immediately with success if their parameters are      *
 * valid.                                                                     *
 *                                                                            *
 ******************************************************************************/

int tuxctl_ioctl(struct tty_struct *tty, struct file *file,
                 unsigned cmd, unsigned long arg)
{
    // Check if can call
    spin_lock(&tux_lock);
    if (reset && !wait)
    {
        reset = 0;
        tux_set_led(tty, led_save);
    }
    if (!wait)
    {
        switch (cmd)
        {
        case TUX_INIT:
            // Call tux_init
            return tux_init(tty);
        case TUX_BUTTONS:
            // Call tux_buttons
            return tux_buttons(tty, arg);
        case TUX_SET_LED:
            // Call tux_set_led
            return tux_set_led(tty, arg);
        default:
            return -EINVAL;
        }
    }
    spin_unlock(&tux_lock);
    return 0;
}

int tux_init(struct tty_struct *tty)
{
    // Set MTCP_BIOC_ON and MTCP_LED_USR
    char buf[2] = {MTCP_BIOC_ON, MTCP_LED_USR};
    spin_lock_init(&tux_lock);

    // Send packet to TUX
    tuxctl_ldisc_put(tty, buf, 2);

    spin_lock(&tux_lock);
    // Initialize button state
    tux_button_state = BUTTON_INIT;

    // Wait for ACK
    wait = 1;
    spin_unlock(&tux_lock);

    // Return
    return 0;
}

int tux_buttons(struct tty_struct *tty, unsigned long arg)
{
    // Initialize variables
    int retval;

    // Check if arg is invalid
    if (arg == NULL_PTR)
    {
        return -EINVAL;
    }

    spin_lock(&tux_lock);
    // Send button state to user
    retval = copy_to_user((int *)arg, &tux_button_state, 1);
    spin_unlock(&tux_lock);

    // Return
    if (retval != 0)
    {
        return -EINVAL;
    }
    return 0;
}

int tux_set_led(struct tty_struct *tty, unsigned long arg)
{
    // Initialize variables
    unsigned char led_set_buf[6];
    unsigned long arg_save = arg;
    int led_values[4];
    int decimal_on_off;
    int led_on_off;
    int i;

    // Get decimal and led states
    decimal_on_off = ((arg >> MOVE_TO_DECIMAL) & FOUR_BITMASK);
    led_on_off = ((arg >> MOVE_TO_LED) & FOUR_BITMASK);

    // Set led states
    for (i = 0; i < LOOP_ALL_LEDS; i++)
    {
        led_values[i] = (arg & FOUR_BITMASK);
        arg = arg >> 4;
    }

    // Set led buffer values
    for (i = 0; i < LOOP_ALL_LEDS; i++)
    {
        // Check if led on
        if ((led_on_off & BIT_MASK))
        {
            // Check if decimal on
            if (decimal_on_off & BIT_MASK)
            {
                led_set_buf[i + 2] = LED_DISPLAY_VALS[led_values[i]] | DECIMAL_ON;
            }
            else
            {
                led_set_buf[i + 2] = LED_DISPLAY_VALS[led_values[i]];
            }
        }
        else
        {
            led_set_buf[i + 2] = BLANK_LED;
        }

        // Next
        led_on_off = led_on_off >> MOVE_ONE_BIT;
        decimal_on_off = decimal_on_off >> MOVE_ONE_BIT;
    }

    // Set led buffer
    led_set_buf[0] = MTCP_LED_SET;
    led_set_buf[1] = SET_ALL_LEDS;

    // Send packet to TUX
    tuxctl_ldisc_put(tty, led_set_buf, i + 2);

    spin_lock(&tux_lock);
    // Save current led state
    led_save = arg_save;

    // Wait for ACK
    wait = 1;
    spin_unlock(&tux_lock);

    // Return
    return 0;
}

void mtcp_reset(struct tty_struct *tty)
{
    // Reset MTCP_LED_USR and MTCP_BIOC_ON
    char reset_buf[2] = {MTCP_LED_USR, MTCP_BIOC_ON};

    spin_lock(&tux_lock);
    reset = 1;
    spin_unlock(&tux_lock);

    // Send packet to tux
    tuxctl_ldisc_put(tty, reset_buf, 2);
}
