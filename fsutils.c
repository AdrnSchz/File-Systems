#include "print.h"

int main(int argc, char *argv[]) {
    Ext2_Data ext2;
    Fat16_Data fat16;

    if (argc != 3) {
        USAGE_ERROR(argv[0]);
        return 1;
    }
    
    char* path = argv[2];

    switch(fat16_read_data(path, &fat16)) {
        case 1:
            OPEN_FILE_ERROR(path);
    }
    
    
    printFat16Info(fat16);

    return 0;
}