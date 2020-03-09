#include "exception_handlers.h"
#include "../PCB.h"
#include "../lib.h"
#include "syscall_handlers.h"

/**
 * Handler for divide by zero OS exceptions.
 * @return {void}
 */
void handler_except_div_by_zero(void) {
    printf("div_by_zero exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for debug exceptions.
 * @return {void}
 */
extern void handler_except_debug_exception(void) {
    printf("debug_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for NMI (nonmaskable interrupt) exceptions.
 * @return {void}
 */
extern void handler_except_nmi(void) {
    printf("nmi exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for breakpoint exceptions.
 * @return {void}
 */
extern void handler_except_breakpoint_exception(void) {
    printf("breakpoint_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for overflow exceptions.
 * @return {void}
 */
extern void handler_except_overflow_exception(void) {
    printf("overflow_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for bound range exceeded exceptions.
 * @return {void}
 */
extern void handler_except_bound_range_exceeded_exception(void) {
    printf("bound_range_exceeded_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for invalid opcode exceptions.
 * @return {void}
 */
extern void handler_except_invalid_opcode_exception(void) {
    printf("invalid_opcode_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for device not available exceptions.
 * @return {void}
 */
extern void handler_except_device_not_available_exception(void) {
    printf("device_not_available_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for double fault exceptions.
 * @return {void}
 */
extern void handler_except_double_fault_exception(void) {
    printf("double_fault_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for coprocessor segment overrun exceptions.
 * @return {void}
 */
extern void handler_except_coprocessor_segment_overrun(void) {
    printf("coprocessor_segment_overrun exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for invalid tss exceptions.
 * @return {void}
 */
extern void handler_except_invalid_tss_exception(void) {
    printf("invalid_tss_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for segment not present exceptions.
 * @return {void}
 */
extern void handler_except_segment_not_present(void) {
    printf("segment_not_present exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for stack fault exceptions.
 * @return {void}
 */
extern void handler_except_stack_fault_exception(void) {
    printf("stack_fault_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for general protection exceptions.
 * @return {void}
 */
extern void handler_except_general_protection_exception(void) {
    printf("general_protection_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for page fault exceptions.
 * @return {void}
 */
extern void handler_except_page_fault_exception(void) {
    printf("page_fault_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for FPU floating point exceptions.
 * @return {void}
 */
extern void handler_except_FPU_floating_point_exception(void) {
    printf("FPU_floating_point_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for alignment check exceptions.
 * @return {void}
 */
extern void handler_except_alignment_check_exception(void) {
    printf("alignment_check_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for machine check exceptions.
 * @return {void}
 */
extern void handler_except_machine_check_exception(void) {
    printf("machine_check_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}

/**
 * Handler for SIMD floating point exceptions.
 * @return {void}
 */
extern void handler_except_SIMD_floating_point_exception(void) {
    printf("SIMD_floating_point_exception exception\n");
    if (isUserProgam) {
        syscall_handler_halt(255);
    } else {
        spin();
    }
}
