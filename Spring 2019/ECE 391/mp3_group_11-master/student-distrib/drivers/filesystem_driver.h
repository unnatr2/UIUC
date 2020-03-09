#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "../types.h"

#define DIRECTORY_ENTRY_RESERVED 24
#define MAX_DATA_BLOCK_NUMBER 1023
#define MAX_DIRECTORY_ENTRIES 63
#define BOOT_BLOCK_RESERVED 52
#define BLOCK_SIZE 4096
#define NEG_ONE -1

#define EXEC_ENTRY_OFFSET 24
#define FILE_HEADER_LEN 40
#define FILE_NAME_SIZE 32
#define FILE_START_BYTE 0
#define FILE_TYPE_FILE 2
#define FILE_TYPE_DIR 1
#define FILE_TYPE_RTC 0

/*    64B Directory Entries
 *
 * +-------------------------+
 * |  file_name       (32B)  |
 * +-------------------------+
 * |  file_type        (4B)  |
 * +-------------------------+
 * |  inode_number     (4B)  |
 * +-------------------------+
 * |                         |
 * |  reserved_bytes  (24B)  |
 * |                         |
 * +-------------------------+
*/

typedef struct directory_entry {
    int8_t file_name[FILE_NAME_SIZE];
    int32_t file_type;
    int32_t inode_number;
    int8_t reserved_bytes[DIRECTORY_ENTRY_RESERVED];
} dentry_t;

/*       4kB Boot Block
 *
 * +-------------------------+
 * |  num_dirs          (4B)  |
 * +-------------------------+
 * |  num_inodes       (4B)  |
 * +-------------------------+
 * |  num_data_blocks  (4B)  |
 * +-------------------------+
 * |                         |
 * |  reserved_bytes  (52B)  |
 * |                         |
 * +-------------------------+
 * |                         |
 * |                         |
 * |  dir_entries   (4032B)  |
 * |                         |
 * |                         |
 * +-------------------------+
*/

typedef struct boot_block {
    int32_t num_dirs;
    int32_t num_inodes;
    int32_t num_data_blocks;
    int8_t reserved_bytes[BOOT_BLOCK_RESERVED];
    dentry_t dir_entries[MAX_DIRECTORY_ENTRIES];
} boot_block_t;

/*       4kB Inode Block
 *
 * +-------------------------+
 * |  length           (4B)  |
 * +-------------------------+
 * |                         |
 * |                         |
 * |  data_blocks   (4092B)  |
 * |                         |
 * |                         |
 * +-------------------------+
*/

typedef struct inode_block {
    int32_t length;
    int32_t data_blocks[MAX_DATA_BLOCK_NUMBER];
} inode_t;

// Initialize filesystem variables
void filesystem_init(uint32_t filesystem_addr);

// Operations on file
int32_t file_open(const uint8_t* filename);
int32_t file_open_index(const uint32_t index);
int32_t file_close(int32_t fd);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);

// Operations of directory file
int32_t dir_open(const uint8_t* filename);
int32_t dir_close(int32_t fd);
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);

// Helper functions
int32_t read_dentry_by_name(const uint8_t* filename, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
int32_t size_of_file();

#endif /* _FILESYSTEM_H */
