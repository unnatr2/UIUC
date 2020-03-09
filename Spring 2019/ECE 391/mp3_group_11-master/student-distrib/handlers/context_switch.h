#ifndef CONTEXT_SWITCH
#include "../types.h"

/* Performs the context switch required in syscall_handlers_execute() */
extern int32_t context_switch(uint32_t user_ds, uint32_t user_cs, uint32_t entry_addrs, void* pcb_bp_addrs, void* pcb_sp_addrs);
extern int32_t halt_ret(uint32_t prev_esp, uint32_t prev_ebp, uint8_t status);

#endif
