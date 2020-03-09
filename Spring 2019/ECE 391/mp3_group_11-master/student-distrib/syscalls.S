#define SYSCALL_ASM 1

/* Saves callee-saved registers to the stack */
#define SAVE_REGS()             \
    pushl %ebx;                 \
    pushl %esi;                 \
    pushl %edi;                 \
    pushl %edx;
/* Restores callee-saved registers from the stack */
#define RESTORE_REGS()          \
    popl %edx;                  \
    popl %edi;                  \
    popl %esi;                  \
    popl %ebx;

/* Copies N arguments from the stack into syscall registers */
#define COPY_ARGS(N) COPY_ARGS_##N
#define COPY_ARGS_1 movl 20(%esp), %ebx;
#define COPY_ARGS_2 COPY_ARGS_1 movl 24(%esp), %ecx;
#define COPY_ARGS_3 COPY_ARGS_2 movl 28(%esp), %edx;

/* Helper macro to move syscall number into %eax and throw interrupt */
#define TRIGGER_SYSCALL_INT(N)   \
    movl $##N, %eax;            \
    int $0x80;

/* Handles the return value of the system call and returns result */
#define HANDLE_DESCRIPTOR_AND_RET() \
    cmpl $0xFFFFF001, %eax;         \
    jb syscall_common_done;         \
    xorl %edx, %edx;                \
    subl %eax, %edx;                \
    /* %edx has positive err # */   \
    /* TODO: Handle Error Number */ \
    orl $0xFFFFFFFF, %eax;           \
    jmp syscall_common_done;


.text
.globl halt
.globl execute
.globl read
.globl write
.globl open
.globl close
.globl getargs

/**
 * Terminates a process, returning the specified value to its parent process.
 * The system call handler itself is responsible for expanding the 8-bit argument 
 * from BL into the 32-bit return value to the parent program’s execute system call.
 * Be careful not to return all 32 bits from EBX. This call should never return to the caller.
 * 
 * @param {uint8_t} status -- 8 bit argument in BL 
 * @return {uint32_t} never
 *
*/
halt:
    SAVE_REGS()
    COPY_ARGS(1)
    TRIGGER_SYSCALL_INT(0x01)
    HANDLE_DESCRIPTOR_AND_RET()

/**
 * Tries to load and execute a new program, handing off the processor 
 * to the new program until it terminates. The command is a space-separated 
 * sequence of words. The first word is the file name of the program to be
 * executed, and the rest of the command—stripped of leading spaces—should 
 * be provided to the new program on request via the getargs system call.
 * 
 * @param {uint8_t*} command -- command number 
 * @return {int32_t}  returns -1 if the command cannot be executed, 
 * 256 if the program dies by an exception, 
 * or a value in the range 0 to 255 if the program executes a halt system call,
 * in which case the value returned is that given by the program’s call to halt.
 *
*/
execute:
    SAVE_REGS()
    COPY_ARGS(1)
    TRIGGER_SYSCALL_INT(0x02)
    HANDLE_DESCRIPTOR_AND_RET()

/**
 * The read system call reads data from the keyboard, a file, device (RTC), 
 * or directory. This call returns the number of bytes read. If the initial 
 * file position is at or beyond the end of file, 0 shall be returned (for 
 * normal files and the directory). In the case of the keyboard, read should
 * return data from one line that has been terminated by pressing Enter, or 
 * as much as fits in the buffer from one such line. The line returned should 
 * include the line feed character. In the case of a file, data should be read 
 * to the end of the file or the end of the buffer provided, whichever occurs 
 * sooner. In the case of reads to the directory, only the filename should be 
 * provided (as much as fits, or all 32 bytes), and subsequent reads should 
 * read from successive directory entries until the last is reached, at which 
 * point read should repeatedly return 0.
 *
 * @param {int32_t} fd - descriptor of file to read
 * @param {void*} buf - Buffer to copy bytes into
 * @param {int32_t} nbytes - number of bytes to read
 * @return {int32_t} Number of bytes read.
 */
read:
    SAVE_REGS()
    COPY_ARGS(3)
    TRIGGER_SYSCALL_INT(0x03)
    HANDLE_DESCRIPTOR_AND_RET()

/**
 * The write system call writes data to the terminal or to a device (RTC).
 * In the case of the terminal, all data should be displayed to the screen
 * immediately. In the case of the RTC, the system call should always accept
 * only a 4-byte integer specifying the interrupt rate in Hz, and should set
 * the rate of periodic interrupts accordingly. Writes to regular files should
 * always return -1 to indicate failure since the file system is read-only.
 * The call returns the number of bytes written, or -1 on failure.
 *
 * @param {int32_t} fd - file descriptor of file to write to
 * @param {const void*} buf - buffer of bytes to write
 * @param {int32_t} nbytes - Number of bytes to write from the buffer
 * @return {int32_t} Number of bytes written, or -1 on failure.
 */
write: 
    SAVE_REGS()
    COPY_ARGS(3)
    TRIGGER_SYSCALL_INT(0x04)
    HANDLE_DESCRIPTOR_AND_RET()

/**
 * Provides access to the file system. The call should find the directory
 * entry corresponding to the named file, allocate an unused file descriptor,
 * and set up any data necessary to handle the given type of file (directory, 
 * RTC device, or regular file). If the named file does not exist or no
 * descriptors are free, the call returns -1.
 *
 * @param {uint8_t*} filename - Name of the file to open
 * @return {int32_t} File descriptor of opened file or -1 on failure
 */
open:
    SAVE_REGS()
    COPY_ARGS(1)
    TRIGGER_SYSCALL_INT(0x5)
    HANDLE_DESCRIPTOR_AND_RET()

/**
 * Closes the specified file descriptor and makes it available for return 
 * from later calls to open. 
 * 
 * @param {uint32_t} fd - file descriptor 
 * @return {int32_t} 0 on success, -1 on failure
 *
 */
close:
    SAVE_REGS()
    COPY_ARGS(1)
    TRIGGER_SYSCALL_INT(0x06)
    HANDLE_DESCRIPTOR_AND_RET()

/**
 * Reads the program’s command line arguments into a user-level buffer. Obviously, 
 * these arguments must be stored as part of the task data when a new program is 
 * loaded. Here they are merely copied into user space. If there are no arguments, 
 * or if the arguments and a terminal NULL (0-byte) do not fit in the buffer, simply 
 * return -1. The shell does not request arguments, but you should probably still 
 * initialize the shell task’s argument data to the empty string.
 *
 * @param {uint8_t*} buf - user-level buffer to copy argument string into
 * @param {int32_t} nbytes - the number of bytes to copy.
 * @return {int32_t} Number of bytes copied into the buffer or -1 on failure.
 */
getargs:
    SAVE_REGS()
    COPY_ARGS(2)
    TRIGGER_SYSCALL_INT(0x07)
    HANDLE_DESCRIPTOR_AND_RET()


/* All systel calls return through this label */
syscall_common_done:
    RESTORE_REGS()
    ret
