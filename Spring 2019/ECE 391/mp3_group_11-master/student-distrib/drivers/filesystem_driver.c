#include "filesystem_driver.h"
#include "../PCB.h"
#include "../handlers/syscall_handlers.h"
#include "../lib.h"

boot_block_t* boot_block;
inode_t* inode_start_addr;
uint32_t data_block_start_addr;
dentry_t direntry;
int8_t null_file[FILE_NAME_SIZE] = {NEG_ONE};

/* filesystem_init
 * Initializes the addresses of the boot block, first inode and first data block 
 * Inputs: filesystem_addr -- Start address of the filesystem 
 * Outputs: none
 */

void filesystem_init(uint32_t filesystem_addr) {
    boot_block = (boot_block_t*)filesystem_addr;
    inode_start_addr = (inode_t*)(filesystem_addr + BLOCK_SIZE);
    data_block_start_addr = filesystem_addr + BLOCK_SIZE + (boot_block->num_inodes) * BLOCK_SIZE;
}

/* file_open
 * Open file with name - filename and save its structure into global direntry
 * Inputs: filename -- Name if file in filesystem 
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t file_open(const uint8_t* filename) {
    // Check if file exists and structure is saved
    if (read_dentry_by_name(filename, &direntry) == 0) {
        // Passed
        return 0;
    }
    // Failed
    return -1;
}

/* file_open_index
 * Open file with index and save its structure into global direntry
 * Inputs: index -- Index into directory entries in filesystem 
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t file_open_index(const uint32_t index) {
    // Check if file exists and structure is saved
    if (read_dentry_by_index(index, &direntry) == 0) {
        // Passed
        return 0;
    }
    // Failed
    return -1;
}

/* file_close
 * Close last file
 * Inputs: fd -- unused for Checkpoint 2
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t file_close(int32_t fd) {
    // Clear direntry
    strncpy((int8_t*)direntry.file_name, (int8_t*)null_file, FILE_NAME_SIZE);
    direntry.file_type = NEG_ONE;
    direntry.inode_number = NEG_ONE;

    // Always passes
    return 0;
}

/* file_write
 * Write to file fd
 * Inputs: fd -- unused for Checkpoint 2
 *         buf -- unused for Checkpoint 2
 *         nbytes -- unused for Checkpoint 2
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t file_write(int32_t fd, const void* buf, int32_t nbytes) {
    // Always fails (Read-only filesystem)
    return -1;
}

/* file_read
 * Read nbytes from last opened file and save to buf
 * Inputs: fd -- unused for Checkpoint 2
 *         buf -- buffer to save bytes read
 *         nbytes -- number of bytes to be read
 * Outputs: number of bytes read
 *          -1 if failed
 */

int32_t file_read(int32_t fd, void* buf, int32_t nbytes) {
    int bytes_read = 0;

    /* Get file descriptor */
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);
    fd_t descriptor = process->fd_array[fd];

    // Read data from last open file
    bytes_read = read_data(descriptor.inode, descriptor.file_position, buf, nbytes);

    /* Update file position */
    process->fd_array[fd].file_position += bytes_read;

    // Return number of bytes read or -1 if failed
    return bytes_read;
}

/* dir_open
 * Open directory with name - filename and save its structure into global direntry
 * Inputs: filename -- Name if file in filesystem 
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t dir_open(const uint8_t* filename) {
    // Check if directory file exists and structure is saved
    if (read_dentry_by_name(filename, &direntry) == 0) {
        // Passed
        return 0;
    }
    // Failed
    return -1;
}

/* dir_close
 * Close last directory
 * Inputs: fd -- unused for Checkpoint 2
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t dir_close(int32_t fd) {
    // Clear direntry
    strncpy((int8_t*)direntry.file_name, (int8_t*)null_file, FILE_NAME_SIZE);
    direntry.file_type = NEG_ONE;
    direntry.inode_number = NEG_ONE;

    // Always passes
    return 0;
}

/* dir_write
 * Write to file fd
 * Inputs: fd -- unused for Checkpoint 2
 *         buf -- unused for Checkpoint 2
 *         nbytes -- unused for Checkpoint 2
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes) {
    // Always fails (Read-only filesystem)
    return -1;
}

/* dir_read
 * Read file names one by one and return name if file in buf
 * Inputs: fd -- unused for Checkpoint 2
 *         buf -- buffer to save bytes read
 *         nbytes -- number of bytes to be read
 * Outputs: number of bytes read into the buffer
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes) {
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid); /* Get current process */
    int8_t* buffer = (int8_t*)buf;        /* Cast to allow characters */
    dentry_t curr_dentry;                 /* directory entry for file to print */
    int i;                                /* Loop variable for file name characters */

    /* Check if buffer to copy into is valid */
    if (buffer == NULL) return -1;

    curr_dentry = boot_block->dir_entries[process->fd_array[fd].file_position];
    // each subsequent call to this function prints the next file
    if (process->fd_array[fd].file_position != boot_block->num_dirs) {
        for (i = 0; i < nbytes; i++) buffer[i] = curr_dentry.file_name[i];
        /* Increment file position in file_descriptor */
        process->fd_array[fd].file_position++;
        return i;
    }

    /* If control reaches this point, all files have been read */
    return 0;
}

////////////////////////////////////// HELPER FUNCTIONS //////////////////////////////////////

/* read_dentry_by_name
 * Fills dentry with file_name, file_type and inode_number using the given filename
 * Inputs: filename -- Name of file in directory
 *         dentry -- Address of an entry in the directory
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t read_dentry_by_name(const uint8_t* filename, dentry_t* dentry) {
    int i = 0;
    // Check if filename is not given
    if (filename == NULL || strlen((const int8_t*)filename) > FILE_NAME_SIZE) {
        // Failed
        return -1;
    }
    // Loop through all entries in the directory
    for (i = 0; i < boot_block->num_dirs; i++) {
        // Check if file is the same (must be all 32 bytes same)
        if (strncmp((int8_t*)filename, (int8_t*)boot_block->dir_entries[i].file_name, FILE_NAME_SIZE) == 0) {
            // Copy into dentry
            strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->dir_entries[i].file_name, FILE_NAME_SIZE);
            dentry->file_type = boot_block->dir_entries[i].file_type;
            dentry->inode_number = boot_block->dir_entries[i].inode_number;
            // Passed
            return 0;
        }
    }
    // Failed
    return -1;
}

/* read_dentry_by_index
 * Fills dentry with file_name, file_type and inode_number using the given index
 * Inputs: index -- Index into the boot block directory entries
 *         dentry -- Address of an entry in the directory
 * Outputs: 0 if passed
 *          -1 if failed
 */

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
    // Check if index is not valid
    if (index < 0 || index >= boot_block->num_dirs) {
        // Failed
        return -1;
    }
    // Copy into dentry
    strncpy((int8_t*)dentry->file_name, (int8_t*)boot_block->dir_entries[index].file_name, FILE_NAME_SIZE);
    dentry->file_type = boot_block->dir_entries[index].file_type;
    dentry->inode_number = boot_block->dir_entries[index].inode_number;
    // Passed
    return 0;
}

/* read_data
 * Copy length bytes of data starting from position offset into buf from the file with inode number inode
 * Inputs: inode -- Inode number of last opened file
 *         offset -- number of bytes offset from the start of file
 *         buf -- buffer to store the number of bytes read
 *         length -- number of bytes to be read
 * Outputs: number of bytes read and placed in buf 
 *          -1 if failed
 */

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    int i;
    int8_t* curr_ptr;                               // Pointer to current byte in file
    uint32_t block_curr = offset / BLOCK_SIZE;      // Current data block number
    uint32_t block_curr_idx = offset % BLOCK_SIZE;  // Position inside current data block

    // Check if inode is invalid
    if (inode < 0 || inode >= boot_block->num_inodes) {
        // Failed
        return -1;
    }

    // Check if current data block number is invalid
    if (inode_start_addr[inode].data_blocks[block_curr] < 0 || inode_start_addr[inode].data_blocks[block_curr] >= boot_block->num_data_blocks) {
        // Failed
        return -1;
    }

    // Find current byte in file
    curr_ptr = (int8_t*)(data_block_start_addr + (inode_start_addr[inode].data_blocks[block_curr]) * BLOCK_SIZE + block_curr_idx);

    // Find length bytes in file
    for (i = 0; i < length; i++) {
        // Check if next block
        if (block_curr_idx >= BLOCK_SIZE) {
            // Update current block
            block_curr_idx = 0;
            block_curr++;

            // Check if next block is invalid
            if (inode_start_addr[inode].data_blocks[block_curr] < 0 || inode_start_addr[inode].data_blocks[block_curr] >= boot_block->num_data_blocks) {
                return -1;
            }

            // Update pointer to current byte
            curr_ptr = (int8_t*)(data_block_start_addr + (inode_start_addr[inode].data_blocks[block_curr]) * BLOCK_SIZE);
        }

        // Check if reached end of file
        if (inode_start_addr[inode].length <= i + offset) {
            // Return number of bytes read
            return i;
        }

        // Copy to current byte to buf
        buf[i] = (uint8_t)*curr_ptr;

        // Increment current position and current byte
        block_curr_idx++;
        curr_ptr++;
    }

    // Return number of bytes read
    return i;
}

/* size_of_file
 * Return size of open file in bytes or 0 if no file is open
 * Inputs: none
 * Outputs: size of file
 */
int32_t size_of_file() {
    return (direntry.inode_number != NEG_ONE)
               ? inode_start_addr[direntry.inode_number].length
               : 0;
}
