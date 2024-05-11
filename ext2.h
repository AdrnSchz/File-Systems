#ifndef _EXT2_H_
#define _EXT2_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define EXT2_BOOT_BLOCK_OFFSET 1024
#define EXT2_ROOT_INODE 2
#define EXT2_NAME_SIZE 255
#define EXT2_IBLOCK_NUM 12
#define EXT2_DIR_ENTRIES 512
#define EXT2_DIR_TYPE 2
#define EXT2_FILE_TYPE 1

// Superblock
#define EXT2_IDENTIFIER_OFFSET 56
#define EXT2_INODE_SIZE_OFFSET 88
#define EXT2_INODE_NUM_OFFSET 0
#define EXT2_INODE_FIRST_OFFSET 84
#define EXT2_INODE_GROUP_OFFSET 40
#define EXT2_INODE_FREE_OFFSET 16

#define EXT2_BLOCK_SIZE_OFFSET 24
#define EXT2_BLOCK_RESERVED_OFFSET 8
#define EXT2_BLOCK_FREE_OFFSET 12
#define EXT2_BLOCK_TOTAL_OFFSET 4
#define EXT2_BLOCK_FIRST_OFFSET 20
#define EXT2_BLOCK_GROUP_OFFSET 32
#define EXT2_FLAG_GROUP_OFFSET 36

#define EXT2_VOLUME_NAME_OFFSET 120
#define EXT2_LAST_CHECK_OFFSET 64
#define EXT2_LAST_MOUNTED_OFFSET 44
#define EXT2_LAST_WRITTEN_OFFSET 48

// Group Descriptor block
#define EXT2_FIRST_INODE_BLOCK_OFFSET 8

typedef struct {
    int size;
    int reserved;
    int free;
    int total;
    int first;
    int groups;
    int flags;
} Ext2_Block;

typedef struct {
    int size;
    int total;
    int first;
    int groups;
    int free;
} Ext2_Inode;

typedef struct {
    char volume_name[16];
    time_t last_check;
    time_t last_mounted;
    time_t last_written;
    Ext2_Block block;
    Ext2_Inode inode;
} Ext2_Data;

typedef struct {
    unsigned short mode;
    unsigned short uid;
    unsigned int size;
    char trash[32]; // i_atime, i_ctime, i_mtime, i_dtime, i_gid, i_links_count, i_blocks, i_flags, i_osd1
    unsigned int blocks[15];
    char trash2[28]; // i_generation, i_file_acl, i_dir_acl, i_faddr, i_osd2
} Inode_Table;

typedef struct {
    unsigned int inode;
    unsigned short record_length;
    char name_length;
    char file_type;
    char name[EXT2_NAME_SIZE];
} Ext2_Dir_Entry;

int ext2_read_data(char* path, Ext2_Data* ext2);
void ext2_print_tree(Ext2_Data ext2, char* path);

#endif