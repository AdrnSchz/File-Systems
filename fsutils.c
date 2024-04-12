#include "print.h"

int main(int argc, char *argv[]) {
    Ext2_Data ext2;
    Fat16_Data fat16;

    if (argc != 3) {
        USAGE_ERROR(argv[0]);
        return 1;
    }
    
    char* path = argv[2];

   int result = ext2_read_data(path, &ext2);

    if (result == 1) {
       OPEN_FILE_ERROR(path);
       return 1;
    } else if (result == -1) {
        NOT_EXT2_ERROR(path);
        result = fat16_read_data(path, &fat16);
        if (result == 1) {
            OPEN_FILE_ERROR(path);
            return 1;
        } else if (result == -1) {
            printf("Error: File system: %s is not of type FAT16\n", path);
            return 1;
        }
    }

    printf("\n------ Filesystem Information ------\n");
    if (result == 2) {
        printExt2Info(ext2);
    } else {
        printFat16Info(fat16);
    }

    return 0;
}