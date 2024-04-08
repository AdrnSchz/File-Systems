#include "print.h"

char* unix_time_to_normal(time_t unix_time) {
    struct tm *local_time;
    char* buffer;

    buffer = malloc(20);

    // Convert Unix time to local time
    local_time = localtime(&unix_time);

    // Format the local time
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", local_time);

    return buffer;
}

void printExt2Info(Ext2_Data ext2) {
    printf("\n\nBlock size = %d\n\n", ext2.block.size);

    printf("Inode size = %d\n", ext2.inode.size);
    printf("Number of inodes = %d\n", ext2.inode.total);
    printf("First inode = %d\n", ext2.inode.first);
    printf("Inodes per group = %d\n", ext2.inode.groups);
    printf("Free inodes = %d\n", ext2.inode.free);

    printf("\n");
    
    printf("Reserved blocks = %d\n", ext2.block.reserved);
    printf("Free blocks = %d\n", ext2.block.free);
    printf("Total blocks = %d\n", ext2.block.total);
    printf("First block = %d\n", ext2.block.first);
    printf("Groups block = %d\n", ext2.block.groups);
    printf("Groups flags = %d\n", ext2.block.flags);

    printf("\n");
    

    printf("Volume name = %s\n", ext2.volume_name);
    
    char* buffer = unix_time_to_normal(ext2.last_check);
    printf("Last checked = %s\n", buffer);
    free(buffer);

    buffer = unix_time_to_normal(ext2.last_mounted);
    printf("Last mounted = %s\n", buffer);
    free(buffer);

    buffer = unix_time_to_normal(ext2.last_written);
    printf("Last written = %s\n", buffer);
    free(buffer);
}