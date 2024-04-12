#include "print.h"

char* unix_time_to_normal(time_t unix_time) {
    struct tm *local_time;
    char* buffer;

    buffer = malloc(21);
    local_time = localtime(&unix_time);

    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", local_time);
    buffer[20] = '\0';

    return buffer;
}

void printExt2Info(Ext2_Data ext2) {
    printf("\nFilesystem: EXT2\n\n");
    printf("INODE INFO\n");
    printf("\tSize = %d\n", ext2.inode.size);
    
    printf("\tNum Inodes: %d\n", ext2.inode.total);
    printf("\tFirst Inode: %d\n", ext2.inode.first);
    printf("\tInodes Group = %d\n", ext2.inode.groups);
    printf("\tFree Inodes = %d\n", ext2.inode.free);

    printf("\nINFO BLOCK\n");
    printf("\tBlock size: %d\n", ext2.block.size);
    printf("\tReserved blocks: %d\n", ext2.block.reserved);
    printf("\tFree blocks: %d\n", ext2.block.free);
    printf("\tTotal blocks: %d\n", ext2.block.total);
    printf("\tFirst block: %d\n", ext2.block.first);
    printf("\tGroups block: %d\n", ext2.block.groups);
    printf("\tGroups flags: %d\n", ext2.block.flags);

    printf("\nINFO VOLUME\n");
    printf("\tVolume name: %s\n", ext2.volume_name);
    
    char* buffer = unix_time_to_normal(ext2.last_check);
    printf("\tLast checked: %s\n", buffer);
    free(buffer);

    buffer = unix_time_to_normal(ext2.last_mounted);
    printf("\tLast mounted: %s\n", buffer);
    free(buffer);

    buffer = unix_time_to_normal(ext2.last_written);
    printf("\tLast written: %s\n", buffer);
    free(buffer);
}

void printFat16Info(Fat16_Data fat16) {
    printf("\nFilesystem: FAT16\n\n");

    printf("System name: %s\n", fat16.system_name);
    printf("Sector size: %d\n", fat16.sector_size);
    printf("Sectors per cluster: %d\n", fat16.sectors_per_cluster);
    printf("Reserved sectors: %d\n", fat16.reserved_sectors);
    printf("# of FATs: %d\n", fat16.num_fats);
    printf("Max root entries: %d\n", fat16.root_entries);
    printf("Sector per FAT: %d\n", fat16.sectors_per_fat);
    printf("Label: %s\n", fat16.label_name);
}
