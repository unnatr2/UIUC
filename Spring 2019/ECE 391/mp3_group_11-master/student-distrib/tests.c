#include "tests.h"
#include "drivers/filesystem_driver.h"
#include "drivers/rtc_driver.h"
#include "drivers/terminal_driver.h"
#include "handlers/keyboard_handler.h"
#include "i8259.h"
#include "lib.h"
#include "syscalls.h"
#include "x86_desc.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER \
    printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result) \
    printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure() {
    /* Use exception #15 for assertions, otherwise
	   reserved by Intel */
    asm volatile("int $15");
}

/* 
   _____ _               _                _       _     __
  / ____| |             | |              (_)     | |   /_ |
 | |    | |__   ___  ___| | ___ __   ___  _ _ __ | |_   | |
 | |    | '_ \ / _ \/ __| |/ / '_ \ / _ \| | '_ \| __|  | |
 | |____| | | |  __/ (__|   <| |_) | (_) | | | | | |_   | |
  \_____|_| |_|\___|\___|_|\_\ .__/ \___/|_|_| |_|\__|  |_|
                             | |
                             |_| 
*/

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test() {
    TEST_HEADER;

    int i;
    int result = PASS;
    for (i = 0; i < 10; ++i) {
        if ((idt[i].offset_15_00 == NULL) &&
            (idt[i].offset_31_16 == NULL)) {
            assertion_failure();
            result = FAIL;
        }
    }

    return result;
}

/**
 * Tests division by zero exception handler. This is a breaking
 * test, meaning that if the test returns, the test has failed.
 * 
 * @coverage: Tests division by zero handler.
 * @files: handlers/exception_handlers.h/c
 * @effects: Exception stalls the machine on a pseudo-BSOD
 */
int test_div_by_zero() {
    TEST_HEADER;
    volatile int malicious = 0;
    int badMaths = 14 / malicious;
    badMaths = 0;
    return FAIL; /* Control should never reach this point */
}

/**
 * General testing for the rest of the exception handlers. Please
 * comment out interrupt call lines as needed for testing
 * 
 * @coverage: Testing wrapper for miscellaneous exception handlers.
 * @files: handlers/exception_handlers.h/c
 * @effects: Exception stalls the machine on a pseudo-BSOD
 */
int test_misc_exceptions() {
    TEST_HEADER;
    asm volatile("int $1"); /* Debug Exception */
    asm volatile("int $2"); /* Non Maskable Interrupt Exception */
    asm volatile("int $3"); /* Breakpoint Exception */
    return FAIL;            /* Control should never reach this point */
}

/**
 * Tests the general interrupt handler for non- "well-defined" interrupts.
 * Control should return from this interrupt if handled correctly, and thus,
 * this test should pass and not cause a hang like some of the exception handlers
 * 
 * @coverage: General interrupt handler
 * @files: handlers/interrupt_handlers.h/c
 * @effects: none
 */
int test_general_interrupt() {
    TEST_HEADER;
    asm volatile("int $0x0F"); /* 0x0F is not defined by Intel and is not used */
    return PASS;               /* Control should return */
}

/**
 * Ensures that trying to dereference a nonpresent page entry throws
 * a page_fault exception. Address 0x0 is not mapped by init_paging,
 * therefore, the expected behavior of this test will throw an exception.
 * 
 * @return FAIL
 * @coverage: init_paging
 * @files: paging.h/c
 */
int test_bad_deref() {
    TEST_HEADER;
    int badDeref = *(int*)0x0; /* Should throw an exception */
    /* Control should not reach this point */
    badDeref = 0;
    return FAIL;
}

/**
 * Ensures that video memory is mapped correctly into the kernel's first page
 * directory entry. The 4KB page starting at 0xB8001 is mapped by init_paging, 
 * therefore, the dereference in this test should succeed.
 * 
 * @return PASS/FAIL
 * @coverage: init_paging
 * @files: paging.h/c
 */
int test_vid_deref() {
    TEST_HEADER;
    /* This virtual address is mapped. Should succeed */
    int vidDeref = *(int*)(0xB8001);
    vidDeref = 0;
    return PASS;
}

/**
 * Ensures that a priviledged user can access the 4MB Kernel Page Segment
 * and that the init_paging() function in paging.c sets up the Kernel 
 * page correctly.
 * 
 * @return {int} PASS/FAIL
 * @coverage init_paging
 * @files: paging.h/c
 */
int test_kernel_deref() {
    TEST_HEADER;
    /* None of the following should throw an exception since all
       are mapped by the kernel page directory upon initialization */
    int kernStart = *(int*)(4 * MB);                /* 4MB address */
    int kernMid = *(int*)((4 * MB) + (4 * MB) / 2); /* 6MB Address */
    int kernEnd = *(int*)(((4 * MB) * 2) - 4);      /* 8MB Border */
    kernStart = kernMid = kernEnd;
    return PASS;
}

/**
 * Tests the input validation of the enable and disable irq mask
 * functions. Expected behavior is to return -1 when the irq is
 * out of bounds
 * 
 * @return {int} PASS/FAIL
 * @coverage: enable_irq and disable_irq
 * @files: i8258.h/c
 */
int test_irq_out_of_bounds() {
    TEST_HEADER;
    int result = PASS;
    if (enable_irq(-1) != -1 || enable_irq(16) != -1) {
        result = FAIL;
    }
    if (disable_irq(-1) != -1 || disable_irq(16) != -1) {
        result = FAIL;
    }
    return result;
}

/* 
   _____ _               _                _       _     ___
  / ____| |             | |              (_)     | |   |__ \ 
 | |    | |__   ___  ___| | ___ __   ___  _ _ __ | |_     ) |
 | |    | '_ \ / _ \/ __| |/ / '_ \ / _ \| | '_ \| __|   / /
 | |____| | | |  __/ (__|   <| |_) | (_) | | | | | |_   / /_
  \_____|_| |_|\___|\___|_|\_\ .__/ \___/|_|_| |_|\__| |____|
                             | |
                             |_|
*/

/*
 * Test what read directory should print.
 * 
 * @return none
 * @param {const uint32_t*} file - the file name to print 
 * @coverage: dir_read
 * @files: filesystem.h/c
 */
void test_read_dir() {
    void* buf;
    dir_read(0, buf, 0);
}

/*
 * Test what read file should print.
 * 
 * @coverage: file_open & file_read
 * @param {const uint32_t*} file - the file name to print 
 * @files: filesystem.h/c
 * @return visual
 */
void test_read_file(const uint8_t* file) {
    uint8_t buf[10000];
    uint32_t file_size;
    int i;
    terminal_init();
    if (file_open(file)) return;
    file_size = size_of_file();
    file_read(0, buf, file_size);
    for (i = 0; i < file_size; i++) {
        terminal_write(0, (uint8_t*)&buf[i], 1);
    }
    file_close(NULL);
}

/**
 * Test printing a file's contents by index from boot block
 * 
 * @return visual
 * @param {uint32_t} index - index from boot block
 * @coverage: file_open_index & file_read
 * @files: filesystem.h/c
 * @return visual
 */
void test_read_file_by_index(uint32_t index) {
    uint8_t buf[10000];
    uint32_t file_size;
    int i;
    terminal_init();
    if (file_open_index(index)) return;
    file_size = size_of_file();
    file_read(0, buf, file_size);
    for (i = 0; i < file_size; i++) {
        terminal_write(0, (uint8_t*)&buf[i], 1);
    }
    file_close(NULL);
}

/*
 * Test what read file should print.
 * 
 * @return visual
 * @coverage: file_open & file_read & file_close
 * @files: filesystem.h/c
 */
void test_read_file_after_close(const uint8_t* file) {
    uint8_t buf[10000];
    uint32_t file_size;
    int i;
    clear();
    file_open(file);
    file_close(NULL); /* Since fd not used for CP2 */
    file_size = size_of_file();
    file_read(0, buf, file_size);
    for (i = 0; i < file_size; i++) {
        terminal_write(0, (uint8_t*)&buf[i], 1);
    }
}

/**
 * Ensures that the rtc read and write functions are performing as expected.
 * Write should return 0 after having set the frequency to 2.
 * Read should print test! so long as there are no rtc interrupts.
 * 
 * @return PASS/FAIL (test is visually verified)
 * @coverage: rtc_driver
 * @sideEffects: clears the screen
 * @files: rtc_driver.h/c
 */
int test_rtc_driver() {
    TEST_HEADER;
    uint32_t i;
    uint32_t counter;
    uint32_t basePrint = 3;
    uint32_t baseFreq = 2;

    /* Test range of frequencies */
    for (i = baseFreq; i <= 128; i <<= 1) {
        terminal_init();

        /* Write updated frequency */
        counter = i * basePrint;
        rtc_write(0, 0, i);
        /* Print a 1 'counter' times at specified frequency */
        while (!rtc_read(0, 0, 0) && counter > 0) {
            terminal_write(0, "1", 1);
            counter--;
        }
    }
    terminal_init();

    return PASS;
}

/**
 * Tests terminal read and write functions.
 * 
 * @return 0
 * @coverage: terminal_read,terminal_write
 * @files: terminal_driver.h/c
 */
int test_terminal_read_write() {
    terminal_init();
    uint8_t buf[128] = {' '};
    terminal_read(0, buf, 10);
    terminal_write(0, buf, 128);
    return 0;
}

/* 
   _____ _               _                _       _     ____
  / ____| |             | |              (_)     | |   |___ \ 
 | |    | |__   ___  ___| | ___ __   ___  _ _ __ | |_    __) |
 | |    | '_ \ / _ \/ __| |/ / '_ \ / _ \| | '_ \| __|  |__ <
 | |____| | | |  __/ (__|   <| |_) | (_) | | | | | |_   ___) |
  \_____|_| |_|\___|\___|_|\_\ .__/ \___/|_|_| |_|\__| |____/
                             | |
                             |_|
*/

int test_invalid_software_interrupt() {
    int32_t lower_bound_ret = 0;
    int32_t upper_bound_ret = 0;

    asm volatile(
        "                \n\
        movl $0, %%eax   \n\
        int $0x80        \n\
        orl %%eax, %0    \n\
        "
        : "=r"(lower_bound_ret)
        :
        : "eax");

    asm volatile(
        "                 \n\
        movl $11, %%eax   \n\
        int $0x80         \n\
        orl %%eax, %0     \n\
        "
        : "=r"(upper_bound_ret)
        :
        : "eax");

    return (lower_bound_ret == -1 && upper_bound_ret == -1) ? PASS : FAIL;
}

/* 
   _____ _               _                _       _     _  _
  / ____| |             | |              (_)     | |   | || |
 | |    | |__   ___  ___| | ___ __   ___  _ _ __ | |_  | || |_
 | |    | '_ \ / _ \/ __| |/ / '_ \ / _ \| | '_ \| __| |__   _|
 | |____| | | |  __/ (__|   <| |_) | (_) | | | | | |_     | |
  \_____|_| |_|\___|\___|_|\_\ .__/ \___/|_|_| |_|\__|    |_|
                             | |
                             |_|
*/

/* 
    _____ _               _                _       _     _____
  / ____| |             | |              (_)     | |   | ____|
 | |    | |__   ___  ___| | ___ __   ___  _ _ __ | |_  | |__
 | |    | '_ \ / _ \/ __| |/ / '_ \ / _ \| | '_ \| __| |___ \ 
 | |____| | | |  __/ (__|   <| |_) | (_) | | | | | |_   ___) |
  \_____|_| |_|\___|\___|_|\_\ .__/ \___/|_|_| |_|\__| |____/
                             | |
                             |_|
*/

/* Test suite entry point */
void launch_tests() {
    clear();

    /* 
               _                           _______        _
         /\   | |                         |__   __|      | |
        /  \  | |_      ____ _ _   _ ___     | | ___  ___| |_
       / /\ \ | \ \ /\ / / _` | | | / __|    | |/ _ \/ __| __|
      / ____ \| |\ V  V / (_| | |_| \__ \    | |  __/\__ \ |_
     /_/    \_\_| \_/\_/ \__,_|\__, |___/    |_|\___||___/\__|
                                __/ |
                               |___/
     */

    // TEST_OUTPUT("idt_test", idt_test());
    // TEST_OUTPUT("test_irq_out_of_bounds", test_irq_out_of_bounds());
    // TEST_OUTPUT("test_general_interrupt", test_general_interrupt());
    // TEST_OUTPUT("test_kernel_deref", test_kernel_deref());
    // TEST_OUTPUT("test_vid_deref", test_vid_deref());

    /*
       _______                _        _______        _       
     |__   __|              | |      |__   __|      | |      
        | | ___   __ _  __ _| | ___     | | ___  ___| |_ ___ 
        | |/ _ \ / _` |/ _` | |/ _ \    | |/ _ \/ __| __/ __|
        | | (_) | (_| | (_| | |  __/    | |  __/\__ \ |_\__ \
        |_|\___/ \__, |\__, |_|\___|    |_|\___||___/\__|___/
                  __/ | __/ |                                
                 |___/ |___/                                  
    */
    /* These tests will either throw an exception when they succeed, or
       halt system execution in some other manner. As such, they should
       only be turned on to test one at a time */

    /* Checkpoint 1 Toggles: */
    // TEST_OUTPUT("test_bad_deref", test_bad_deref());
    // TEST_OUTPUT("test_div_by_zero", test_div_by_zero());
    // TEST_OUTPUT("test_misc_exceptions", test_misc_exceptions());

    /* Checkpoint 2 Toggles: */
    // test_read_dir();
    // test_read_file_by_index(10);
    // test_read_file((const uint8_t*)"frame0.txt");
    // test_read_file((const uint8_t*)"frame1.txt");
    // test_read_file((const uint8_t*)"verylargetextwithverylongname.tx");
    // test_read_file((const uint8_t*)"verylargetextwithverylongname.txt");
    // test_read_file_after_close((uint8_t*)"frame0.txt");
    // TEST_OUTPUT("test_rtc_driver", test_rtc_driver());
    // test_terminal_read_write();

    /* Checkpoint 3 Toggles: */
    // TEST_OUTPUT("test_invalid_software_interrupt", test_invalid_software_interrupt());
}
