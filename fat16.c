#include "fat16.h"

int fat16_read_data(char* path, Fat16_Data* fat16) {
    FILE *fs = fopen(path, "rb");
    if (fs == NULL) {
        perror("Failed to open file system image");
        return 1;
    }

    

    
    fclose(fs);

    return 0;
}

