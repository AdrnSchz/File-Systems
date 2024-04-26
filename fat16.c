#include "fat16.h"

int fat16_read_data(char* path, Fat16_Data* fat16) {
    FILE *fs = fopen(path, "rb");
    if (fs == NULL) {
        printf("Failed to open file %s\n", path);
        return 1;
    }

    unsigned short clusters;
    fseek(fs, 19, SEEK_SET);
    fread(&clusters, sizeof(short), 1, fs);
    if (clusters < 4086) {
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

void printFat16TreeEntry(char* name, int depth) {
    printf("|");
    for(int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf("|-- %s\n", name);
}

int isDirectory(FILE* fd, unsigned int addr) {
    char attribute;
    fseek(fd, addr + FAT16_NAME_SIZE, SEEK_SET);
    fread(&attribute, sizeof(char), 1, fd);

    return (attribute == 0x10);
}
void recursive_search(Fat16_Data fat16, FILE* fd, unsigned int addr, int depth) {
    char file[FAT16_NAME_SIZE + 1];
    for (int i = 0; i < fat16.root_entries; i++) {
        fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE), SEEK_SET);
        fread(&file, sizeof(char), FAT16_NAME_SIZE, fd);
        file[FAT16_NAME_SIZE] = '\0';
        
        if (file[0] == 0x00) return;
        if (file[0] == 0x5 && file[1] == 0xE) continue;

        if (isDirectory(fd, addr + (i * FAT16_DIR_ENTRY_SIZE))) {
            printFat16TreeEntry(file, depth);
            
            unsigned short cluster;
            fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE) + FAT16_FIRST_CLUSTER_OFFSET, SEEK_SET);
            fread(&cluster, sizeof(short), 1, fd);
            unsigned int cluster_addr = (cluster - 2) * fat16.sectors_per_cluster * fat16.sector_size;
            unsigned int dir_addr = cluster_addr + (fat16.reserved_sectors * fat16.sector_size) + 
                (fat16.num_fats * fat16.sectors_per_fat * fat16.sector_size) + (fat16.root_entries * FAT16_DIR_ENTRY_SIZE);
            recursive_search(fat16, fd, dir_addr, depth + 1);
        } 
        else {
            printFat16TreeEntry(file, depth);
        }
    }
}
void fat16_print_tree(Fat16_Data fat16, char* path) {
    FILE *fd = fopen(path, "rb");

    if (fd == NULL) {
        printf("Failed to open file %s\n", path);
        return;
    }

    unsigned int root_dir = fat16.reserved_sectors + (fat16.num_fats * fat16.sectors_per_fat);
    
    recursive_search(fat16, fd, root_dir * fat16.sector_size, 0);

    fclose(fd);
}

