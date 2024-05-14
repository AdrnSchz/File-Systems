#include "ext2.h"

int ext2_read_data(char* path, Ext2_Data* ext2) {

    FILE *fs = fopen(path, "rb");
    if (fs == NULL) {
        printf("Failed to open file %s\n", path);
        return 1;
    }

    // ================================================================
    // TODO: CHECKEAR SI EL FILEES EXT2
    // ================================================================
    int type;
    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_IDENTIFIER_OFFSET, SEEK_SET);
    fread(&type, sizeof(char), 2, fs);
    if (type != 0xEF53) {
        return -1;
    }

    int shift = 0;
    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_BLOCK_SIZE_OFFSET, SEEK_SET);
    fread(&shift, sizeof(int), 1, fs);
    ext2->block.size = 1024 << shift;

    // ================================================================
    // INODE INFO  
    // ================================================================
    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_INODE_SIZE_OFFSET, SEEK_SET);
    fread(&ext2->inode.size, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_INODE_NUM_OFFSET, SEEK_SET);
    fread(&ext2->inode.total, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_INODE_FIRST_OFFSET, SEEK_SET);
    fread(&ext2->inode.first, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_INODE_GROUP_OFFSET, SEEK_SET);
    fread(&ext2->inode.groups, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_INODE_FREE_OFFSET, SEEK_SET);
    fread(&ext2->inode.free, sizeof(int), 1, fs);

    // ================================================================
    // INFO BLOCK
    // ================================================================
    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_BLOCK_RESERVED_OFFSET, SEEK_SET);
    fread(&ext2->block.reserved, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_BLOCK_FREE_OFFSET, SEEK_SET);
    fread(&ext2->block.free, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_BLOCK_TOTAL_OFFSET, SEEK_SET);
    fread(&ext2->block.total, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_BLOCK_FIRST_OFFSET, SEEK_SET);
    fread(&ext2->block.first, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_BLOCK_GROUP_OFFSET, SEEK_SET);
    fread(&ext2->block.groups, sizeof(int), 1, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_FLAG_GROUP_OFFSET, SEEK_SET);
    fread(&ext2->block.flags, sizeof(int), 1, fs);

    // ================================================================
    // INFO VOLUME
    // ================================================================
    ext2->last_check = 0;
    ext2->last_mounted = 0;
    ext2->last_written = 0;

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_VOLUME_NAME_OFFSET, SEEK_SET);
    fread(&ext2->volume_name, sizeof(char) * 16, 1, fs);
    
    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_LAST_CHECK_OFFSET, SEEK_SET);
    fread(&ext2->last_check, sizeof(char), 4, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_LAST_MOUNTED_OFFSET, SEEK_SET);
    fread(&ext2->last_mounted, sizeof(char), 4, fs);

    fseek(fs, EXT2_BOOT_BLOCK_OFFSET + EXT2_LAST_WRITTEN_OFFSET, SEEK_SET);
    fread(&ext2->last_written, sizeof(char), 4, fs);

    fclose(fs);

    return 2;
}

Inode readInode(Ext2_Data ext2, FILE* fd, int inode) {
    Inode inode_data;
    
    unsigned int block_group_num = (inode - 1) / ext2.inode.groups;
    unsigned int index_inode = (inode - 1) % ext2.inode.groups;
    unsigned int block_group_offset = ext2.block.groups * ext2.block.size * block_group_num;

    unsigned int inode_block;
    fseek(fd, EXT2_BOOT_BLOCK_OFFSET + ext2.block.size + block_group_offset + EXT2_FIRST_INODE_BLOCK_OFFSET , SEEK_SET);
    fread(&inode_block, sizeof(int), 1, fd);

    unsigned int inode_offset = index_inode * ext2.inode.size + inode_block * ext2.block.size + block_group_offset;

    fseek(fd, inode_offset, SEEK_SET);
    fread(&inode_data, sizeof(Inode), 1, fd);

    return inode_data;
}

int readDirEntry(Ext2_Dir_Entry *entry, FILE* fd, int pos, unsigned int *size, unsigned int max_size) {
    unsigned int inode;
    unsigned short rec_len;
    char name_len;

    fseek(fd, pos + *size, SEEK_SET);
    fread(&inode, sizeof(int), 1, fd);
    
    if (inode == 0) return -1;
    
    fread(&rec_len, sizeof(short), 1, fd);

    if (rec_len + *size > max_size) return -1;
    
    fread(&name_len, sizeof(char), 1, fd);
    fseek(fd, pos + *size, SEEK_SET);
    fread(entry, sizeof(Ext2_Dir_Entry) - EXT2_NAME_SIZE + name_len, 1, fd);
    
    entry->name[(int)name_len] = '\0';
    *size += rec_len;
    
    return 0;
}

void printExt2TreeEntry(char* name, int depth) { 
    printf("|");
    
    for(int i = 0; i < depth; i++) {
        printf("    |");
    }

    printf("__ %s\n", name);
}

void ext2_search(Ext2_Data ext2, FILE* fd, int inode, int depth) {
    Inode inode_data = readInode(ext2 , fd, inode);
    unsigned int size = 0;
    Ext2_Dir_Entry entry;

    for (int i = 0; i < EXT2_IBLOCK_NUM && size < inode_data.size; i++) {
        unsigned int dir_pos = inode_data.blocks[i] * ext2.block.size;

        for (int j = 0; j < EXT2_DIR_ENTRIES; j++) { // check num dir entries
            if (readDirEntry(&entry, fd, dir_pos, &size, inode_data.size) == -1) continue;
            
            if (entry.file_type == EXT2_FILE_TYPE) printExt2TreeEntry(entry.name, depth);
            if (entry.file_type == EXT2_DIR_TYPE && strcmp(entry.name, ".") != 0 && strcmp(entry.name, "..") != 0 && strcmp(entry.name, "lost+found") != 0) {
                printExt2TreeEntry(entry.name, depth);
                
                ext2_search(ext2, fd, entry.inode, depth + 1);
            }
        }
    }
}

void ext2_print_tree(Ext2_Data ext2, char* path) {
    FILE* fd = fopen(path, "rb");
    if (fd == NULL) {
        printf("Failed to open file %s\n", path);
        return;
    }

    ext2_search(ext2, fd, EXT2_ROOT_INODE, 0);
    fclose(fd);
}