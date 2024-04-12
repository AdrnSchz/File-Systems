#include "print.h"
#include <string.h>

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
    printf("\rSize = %d\n", ext2.inode.size);
    
    printf("\rNum Inodes: %d\n", ext2.inode.total);
    printf("\rFirst Inode: %d\n", ext2.inode.first);
    printf("\rInodes Group = %d\n", ext2.inode.groups);
    printf("\rFree Inodes = %d\n", ext2.inode.free);

    printf("\nINFO BLOCK\n");
    printf("\rBlock size: %d\n", ext2.block.size);
    printf("\rReserved blocks: %d\n", ext2.block.reserved);
    printf("\rFree blocks: %d\n", ext2.block.free);
    printf("\rTotal blocks: %d\n", ext2.block.total);
    printf("\rFirst block: %d\n", ext2.block.first);
    printf("\rGroups block: %d\n", ext2.block.groups);
    printf("\rGroups flags: %d\n", ext2.block.flags);

    printf("\nINFO VOLUME\n");
    printf("\rVolume name: %s\n", ext2.volume_name);
    
    char* buffer = unix_time_to_normal(ext2.last_check);
    printf("\rLast checked: %s\n", buffer);
    free(buffer);

    buffer = unix_time_to_normal(ext2.last_mounted);
    printf("\rLast mounted: %s\n", buffer);
    free(buffer);

    buffer = unix_time_to_normal(ext2.last_written);
    printf("\rLast written: %s\n", buffer);
    free(buffer);
}

void printFat16Info(Fat16_Data fat16) {
    printf("\nFilesystem: FAT16");

    printf("System name: %s\n", fat16.system_name);
    printf("Sector size: %d\n", fat16.sector_size);
    printf("Sectors per cluster: %d\n", fat16.sectors_per_cluster);
    printf("Reserved sectors: %d\n", fat16.reserved_sectors);
    printf("# of FATs: %d\n", fat16.num_fats);
    printf("Max root entries: %d\n", fat16.root_entries);
    printf("Sector per FAT: %d\n", fat16.sectors_per_fat);
    printf("Label: %s\n", fat16.label_name);
}
