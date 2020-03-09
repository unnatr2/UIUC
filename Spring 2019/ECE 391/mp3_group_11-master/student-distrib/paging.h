#ifndef PAGING
#define PAGING

#include "types.h"

#define PMEM_PROCESS_VIDEO_BUFFER_BUFFER_START (140 * MB)  // virtual address of beginning of video buffers
#define VMEM_EXEC_START_OFFSET 0x48000                     // offset of program image where it wil start executing
#define VMEM_EXEC_START (128 * MB)                         // virtual address of beginning of the program page
#define VMEM_EXEC_END (VMEM_EXEC_START + 4 * MB)           // virtual address of the end of th the program page
#define VMEM_PROCESS_VIDEO_START (136 * MB)                // virtual address of the user-level video memory mappings
#define VMEM_KERNEL_START 4 * MB                           // beginning address of kernel memory
#define VMEM_KERNEL_END (VMEM_KERNEL_START + 4 * MB)       // end address of kernel memory
#define PMEM_PROCESS_VIDEO_BUFFER(pid) (PMEM_PROCESS_VIDEO_BUFFER_BUFFER_START + (4 * KB * pid))
#define VMEM_PROCESS_VIDEO(pid) (VMEM_PROCESS_VIDEO_START + (4 * KB * pid))

/* Convenient structure definition for both page directory entry
   structures and page table entry structures */
typedef struct __attribute__((packed)) page_struct_t {
    uint8_t present : 1;
    uint8_t rw : 1;
    uint8_t unprivileged : 1;
    uint8_t write_through : 1;
    uint8_t cache_disable : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1; /* Only for page table entries */
    uint8_t large : 1; /* Only for page directory entries */
    uint8_t global : 1;
    uint8_t avail : 3;
    uint32_t phys_base : 20;
} page_struct_t;

typedef enum video_route_t {
    DISPLAYED,
    BUFFERED
} video_route_t;

/* Function Declarations */
void flushTLB(void);
void init_paging(void);
void set_process_page(uint32_t pid);
void route_process_video(uint32_t pid, video_route_t routing);
void map_virt_to_phys(uint32_t virt, uint32_t phys, page_struct_t options);

#endif
