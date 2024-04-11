#include "print.h"

int main(int argc, char *argv[]) {
    Ext2_Data ext2;
    Fat16_Data fat16;

    if (argc != 3) {
        USAGE_ERROR(argv[0]);
        return 1;
    }
    
    char* path = argv[2];

    switch(ext2_read_data(path, &ext2)) {
        case 1:
            OPEN_FILE_ERROR(path);
    }

    printf("\n------ Filesystem Information ------\n");
    printExt2Info(ext2);

    return 0;
}