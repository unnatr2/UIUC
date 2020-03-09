#ifndef EXCEPTION_HANDLERS
#define EXCEPTION_HANDLERS

extern void handler_except_div_by_zero(void);
extern void handler_except_debug_exception(void);
extern void handler_except_nmi(void);
extern void handler_except_breakpoint_exception(void);
extern void handler_except_overflow_exception(void);
extern void handler_except_bound_range_exceeded_exception(void);
extern void handler_except_invalid_opcode_exception(void);
extern void handler_except_device_not_available_exception(void);
extern void handler_except_double_fault_exception(void);
extern void handler_except_coprocessor_segment_overrun(void);
extern void handler_except_invalid_tss_exception(void);
extern void handler_except_segment_not_present(void);
extern void handler_except_stack_fault_exception(void);
extern void handler_except_general_protection_exception(void);
extern void handler_except_page_fault_exception(void);
extern void handler_except_FPU_floating_point_exception(void);
extern void handler_except_alignment_check_exception(void);
extern void handler_except_machine_check_exception(void);
extern void handler_except_SIMD_floating_point_exception(void);

#endif
