#include "ext2.h"


int ext2_read_data(char* path, Ext2_Data* ext2) {

    FILE *fs = fopen(path, "rb");
    if (fs == NULL) {
        perror("Failed to open file system image");
        return 1;
    }



    // ================================================================
    // TODO: CHECKEAR SI EL FILEES EXT2
    // ================================================================
    int type;
    fseek(fs, 1024 + 56, SEEK_SET);
    fread(&type, sizeof(char), 2, fs);
    if (type != 0xEF53) {
        return -1;
    }

    int shift = 0;
    fseek(fs, EXT2_BLOCK_SIZE_OFFSET, SEEK_SET);
    fread(&shift, sizeof(int), 1, fs);
    ext2->block.size = EXT2_SUPERBLOCK_SIZE << shift;

    // ================================================================
    // INODE INFO  
    // ================================================================
    fseek(fs, ext2->block.size + EXT2_INODE_SIZE_OFFSET, SEEK_SET);
    fread(&ext2->inode.size, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_INODE_NUM_OFFSET, SEEK_SET);
    fread(&ext2->inode.total, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_INODE_FIRST_OFFSET, SEEK_SET);
    fread(&ext2->inode.first, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_INODE_GROUP_OFFSET, SEEK_SET);
    fread(&ext2->inode.groups, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_INODE_FREE_OFFSET, SEEK_SET);
    fread(&ext2->inode.free, sizeof(int), 1, fs);

    // ================================================================
    // INFO BLOCK
    // ================================================================
    fseek(fs, ext2->block.size + EXT2_BLOCK_RESERVED_OFFSET, SEEK_SET);
    fread(&ext2->block.reserved, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_BLOCK_FREE_OFFSET, SEEK_SET);
    fread(&ext2->block.free, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_BLOCK_TOTAL_OFFSET, SEEK_SET);
    fread(&ext2->block.total, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_BLOCK_FIRST_OFFSET, SEEK_SET);
    fread(&ext2->block.first, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_BLOCK_GROUP_OFFSET, SEEK_SET);
    fread(&ext2->block.groups, sizeof(int), 1, fs);

    fseek(fs, ext2->block.size + EXT2_FLAG_GROUP_OFFSET, SEEK_SET);
    fread(&ext2->block.flags, sizeof(int), 1, fs);

    // ================================================================
    // INFO VOLUME
    // ================================================================
    ext2->last_check = 0;
    ext2->last_mounted = 0;
    ext2->last_written = 0;

    fseek(fs, ext2->block.size + EXT2_VOLUME_NAME_OFFSET, SEEK_SET);
    fread(&ext2->volume_name, sizeof(char) * 16, 1, fs);
    
    fseek(fs, ext2->block.size + EXT2_LAST_CHECK_OFFSET, SEEK_SET);
    fread(&ext2->last_check, sizeof(char), 4, fs);

    fseek(fs, ext2->block.size + EXT2_LAST_MOUNTED_OFFSET, SEEK_SET);
    fread(&ext2->last_mounted, sizeof(char), 4, fs);

    fseek(fs, ext2->block.size + EXT2_LAST_WRITTEN_OFFSET, SEEK_SET);
    fread(&ext2->last_written, sizeof(char), 4, fs);

    fclose(fs);

    return 2;
}

void ext2_print_tree(Ext2_Data ext2, char* path) {
    printf("EXT2 Print Tree\n");
    printf("Path: %s\n", path);
    printf("Volume name: %s\n", ext2.volume_name);
}