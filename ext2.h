#ifndef _EXT2_H_
#define _EXT2_H_

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define EXT2_BOOT_BLOCK_OFFSET 1024
#define EXT2_ROOT_INODE 2

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
    char trash[20]; // i_atime, i_ctime, i_mtime, i_dtime, i_gid, i_links_count
    unsigned int i_blocks;
    char trash2[8]; // i_flags, i_osd1
    unsigned int blocks[15];
    char trash3[28]; // i_generation, i_file_acl, i_dir_acl, i_faddr, i_osd2
} Inode_Table;

typedef struct {
    int inode;
    int size;
    char name[];
} Ext2_Directory;

int ext2_read_data(char* path, Ext2_Data* ext2);
void ext2_print_tree(Ext2_Data* ext2, char* path);

#endif