#include "paging.h"
#include "PCB.h"
#include "lib.h"

#define LOW_TEN_BITS 0x03FF
#define NUM_PAGE_TABLES 512

/* Global storage for page directories and tables... */
static page_struct_t page_directory[KB] __attribute__((aligned(4 * KB))) = {{0}};
static page_struct_t ptables[NUM_PAGE_TABLES][KB] __attribute__((aligned(4 * KB))) = {{{0}}};

/**
 * Initializes paging on the processor by pointing the Page Directory Base Register
 * to point to a globally-scoped page-directory (see above).
 * 
 * @ref: (asm) https://wiki.osdev.org/paging
 * @return {void}
 */
void init_paging() {
    /*  Paging is setup for the kernel as such, Video memory should be
        Mapped accordingly into first 4MB block (table)
         ___________
        |___________|  0MB - Table 0 - 4KB Blocks
        |__Vid Mem__|
        |           |
        |___________|
        |           |  4MB - Table 1 - 4MB Block
        |   Kernel  |
        |           |
        |___________|
        |           |  8MB... - Tables 2... - 4KB Blocs
        |    Not    |  (Already initialized as such statically)
        |  Present  |
        |___________|
    */

    /* Set video memory in virtual address space */
    page_struct_t videoOpts = {
        .present = TRUE,
        .rw = TRUE};
    map_virt_to_phys(VIDEO, VIDEO, videoOpts);

    /* Map all video buffers with one-to-one correspondence for direct
       access in kernel mode (used in route_process_video) */
    uint32_t pid;
    for (pid = 0; pid < MAX_PROCESSES; pid++) {
        uint32_t bufferAddr = PMEM_PROCESS_VIDEO_BUFFER(pid);
        map_virt_to_phys(bufferAddr, bufferAddr, videoOpts);
    }

    /* Set Large 4MB Kernel page */
    page_struct_t kernelOpts = {
        .present = TRUE,
        .large = TRUE,
        .rw = TRUE,
    };
    map_virt_to_phys(VMEM_KERNEL_START, VMEM_KERNEL_START, kernelOpts);

    /* Sets Page Size Extensions Option (bit 4 of CR4)
       and enables paging via CR3 See x86 ISA 3.7.2 */
    asm volatile(
        "                          \n\
        movl    %0, %%cr3          \n\
        movl    %%cr4, %%eax       \n\
        orl     $0x010, %%eax      \n\
        movl    %%eax, %%cr4       \n\
        movl    %%cr0, %%eax       \n\
        orl     $0x80000001, %%eax \n\
        movl    %%eax, %%cr0       \n\
        "
        :
        : "r"((uint32_t)page_directory)
        : "eax");

    return;
}

/**
 * Flushes the Translation lookaside buffer by reassigning
 * to the register CR3
 * 
 * @return {void} - none
 */
void flushTLB(void) {
    asm volatile(
        "                          \n\
        movl    %%cr3, %%eax       \n\
        movl    %%eax, %%cr3       \n\
        "
        :
        :
        : "eax");
}

/**
 * Sets up the program's page at address 132MB-136MB in virtual memory
 * and flushes the TLB to make changes effective.
 * 
 * @param {uint32_t} pid - the identifier of process to route paging for
 * @return {void} - none
 */
void set_process_page(uint32_t pid) {
    /* Create page directory entry for process */
    map_virt_to_phys(
        VMEM_EXEC_START,
        VMEM_KERNEL_END + (pid * (4 * MB)),
        (page_struct_t){
            .cache_disable = TRUE,
            .unprivileged = TRUE,
            .present = TRUE,
            .large = TRUE,
            .rw = TRUE,
        });
}

/**
 * Routes virtual video memory space of a process either directly to VGA MMO 
 * or to pid-specific cache location depending on the routing argument.
 * 
 * @param {uint32_t} pid - the process identifier of process to route
 * @param {video_route_t} routing - routing method for process (DISPLAYED/BUFFERED)
 */
void route_process_video(uint32_t pid, video_route_t routing) {
    uint32_t page_offset = pid * 4 * KB;
    uint32_t phys_direct_addrs = VIDEO;
    uint32_t phys_buffer_addrs = PMEM_PROCESS_VIDEO_BUFFER_BUFFER_START + page_offset;
    uint32_t virtual_address = VMEM_PROCESS_VIDEO_START + page_offset;

    /* Map virtual address */
    page_struct_t options = {
        .unprivileged = TRUE,
        .present = TRUE,
        .rw = TRUE,
    };
    map_virt_to_phys(virtual_address, (routing == DISPLAYED) ? phys_direct_addrs : phys_buffer_addrs, options);
}

/**
 * Maps a provided virtual address to the provided physical memory address with
 * given options. For clarity, when the large option is selected, a 4MB page will
 * be mapped starting from the virtual rounded down to the nearest 4MB aligned
 * address, and when a small page is selected, it will be rounded down to the
 * nearest 4KB aligned address.
 * 
 * https://courses.engr.illinois.edu/ece391/sp2019/secure/references/descriptors.pdf
 * 
 * @param {uint32_t} virt - virtual address to map
 * @param {uint32_t} phys - virtual addresses are mapped to this physical address
 * @param {page_struct_t} options - options for the page translation
 * @sideEffect - flushes the TLB
 * @return {void} - none
 */
void map_virt_to_phys(uint32_t virt, uint32_t phys, page_struct_t options) {
    uint32_t pde_idx = ((uint32_t)virt / (4 * MB)) & LOW_TEN_BITS;
    uint32_t pte_idx = ((uint32_t)virt / (4 * KB)) & LOW_TEN_BITS;
    page_struct_t pde = options, pte = options;

    /* Add pde to page-directory */
    pde.phys_base = ((options.large) ? phys : (uint32_t)ptables[pde_idx]) / (4 * KB);
    page_directory[pde_idx] = pde;

    /* Add pte to page-table if necessary */
    if (!options.large) {
        pte.phys_base = phys / (4 * KB);
        ptables[pde_idx][pte_idx] = pte;
    }

    /* Clear the translation-lookaside buffer */
    flushTLB();
}
