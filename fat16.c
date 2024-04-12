#include "fat16.h"

int fat16_read_data(char* path, Fat16_Data* fat16) {
    FILE *fs = fopen(path, "rb");
    if (fs == NULL) {
        perror("Failed to open file system image");
        return 1;
    }

    char type[8];
    fseek(fs, 54, SEEK_SET);
    fread(&type, sizeof(char), 8, fs);
    if (strncmp(type, "FAT16", 5) == 0) {
        return -1;
    }

    fat16->sector_size = 0;
    fat16->sectors_per_cluster = 0;
    fat16->reserved_sectors = 0;

    fat16->num_fats = 0;
    fat16->root_entries = 0;
    fat16->sectors_per_fat = 0;

    fseek(fs, 3, SEEK_SET);
    fread(&fat16->system_name, sizeof(char), 8, fs);

    fseek(fs, 11, SEEK_SET);
    fread(&fat16->sector_size, sizeof(char), 2, fs);

    fseek(fs, 13, SEEK_SET);
    fread(&fat16->sectors_per_cluster, sizeof(char), 1, fs);

    fseek(fs, 14, SEEK_SET);
    fread(&fat16->reserved_sectors, sizeof(char), 2, fs);

    fseek(fs, 16, SEEK_SET);
    fread(&fat16->num_fats, sizeof(char), 1, fs);

    fseek(fs, 17, SEEK_SET);
    fread(&fat16->root_entries, sizeof(char), 2, fs);

    fseek(fs, 22, SEEK_SET);
    fread(&fat16->sectors_per_fat, sizeof(char), 2, fs);

    fseek(fs, 43, SEEK_SET);
    fread(&fat16->label_name, sizeof(char), 11, fs);
    
    fclose(fs);

    return 0;
}

