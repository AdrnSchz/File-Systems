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
    if (depth > 0) {
        printf("|");
    }
    for(int i = 0; i < depth; i++) {
        printf("    ");
    }

    for (int i = 0; i < FAT16_NAME_SIZE + 1; i++) {
        name[i] = tolower(name[i]);
    }
    printf("|__ %s\n", name);
}

int isDirectory(FILE* fd, unsigned int addr) {
    char attribute;
    fseek(fd, addr + FAT16_NAME_SIZE, SEEK_SET);
    fread(&attribute, sizeof(char), 1, fd);

    return (attribute == 0x10);
}

unsigned char* prepareName(unsigned char* name) {
    unsigned char* new_name = malloc(FAT16_NAME_SIZE + 2);
    int i,j = 0;
    for (i = 0; i < 8 && name[i] != ' '; i++) {
        new_name[j] = name[i];
        j++;
    }

    if (name[8] != ' ') {
        new_name[j] = '.';
        j++;
        for (i = 8; i < FAT16_NAME_SIZE && name[i] != ' '; i++) {
            new_name[j] = name[i];
            j++;
        }
    }
    new_name[j] = '\0';
    free(name);

    return new_name;
}

int checkName(unsigned char* name) {
    int illegal[16] = { 0x22, 0x2A, 0x2B, 0x2C, 0x2E, 0x2F, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x5B, 0x5C, 0x5D, 0x7C };
    if (name[0] == 0x05 || name[0] == 0xE5) {
        return 1;
    }

    for (int i = 0; i < FAT16_NAME_SIZE; i++) {
        if (name[i] < 0x20) {
            return 1;
        }
        else {
            for (int j = 0; j < 16; j++) {
                if (name[i] == illegal[j]) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void search(Fat16_Data fat16, FILE* fd, unsigned int addr, int depth) {
    unsigned char *file = malloc(FAT16_NAME_SIZE + 1);
    for (int i = 0; i < fat16.root_entries; i++) {
        memset(file, 0, FAT16_NAME_SIZE + 1);
        fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE), SEEK_SET);
        fread(file, sizeof(char), FAT16_NAME_SIZE, fd);
        file[FAT16_NAME_SIZE] = '\0';
        
        if (file[0] == 0x00) {
            free(file);
            return;
        }
        if (checkName(file)) continue;
        
        file = prepareName(file);
        if (isDirectory(fd, addr + (i * FAT16_DIR_ENTRY_SIZE))) {
            if (strcmp((char*)file, ".") == 0 || strcmp((char*)file, "..") == 0) continue;
            
            printFat16TreeEntry((char*)file, depth);
            
            unsigned short cluster;
            fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE) + FAT16_FIRST_CLUSTER_OFFSET, SEEK_SET);
            fread(&cluster, sizeof(short), 1, fd);
            unsigned int cluster_addr = (cluster - 2) * fat16.sectors_per_cluster * fat16.sector_size;
            unsigned int dir_addr = cluster_addr + (fat16.reserved_sectors * fat16.sector_size) + 
                (fat16.num_fats * fat16.sectors_per_fat * fat16.sector_size) + (fat16.root_entries * FAT16_DIR_ENTRY_SIZE);
            search(fat16, fd, dir_addr, depth + 1);
        } 
        else {
            printFat16TreeEntry((char*)file, depth);
        }
    }
    free(file);
}

void fat16_print_tree(Fat16_Data fat16, char* path) {
    FILE *fd = fopen(path, "rb");

    if (fd == NULL) {
        printf("Failed to open file %s\n", path);
        return;
    }

    unsigned int root_dir = fat16.reserved_sectors + (fat16.num_fats * fat16.sectors_per_fat);
    
    search(fat16, fd, root_dir * fat16.sector_size, 0);

    fclose(fd);
}


// PHASE 3

void searchAndPrint(Fat16_Data fat16, FILE* fd, unsigned int addr, int depth, char* filename) {
    unsigned char *file = malloc(FAT16_NAME_SIZE + 1);
    for (int i = 0; i < fat16.root_entries; i++) {
        memset(file, 0, FAT16_NAME_SIZE + 1);
        fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE), SEEK_SET);
        fread(file, sizeof(char), FAT16_NAME_SIZE, fd);
        file[FAT16_NAME_SIZE] = '\0';
        
        if (file[0] == 0x00) {
            free(file);
            return;
        }
        if (checkName(file)) continue;
        
        file = prepareName(file);
        if (isDirectory(fd, addr + (i * FAT16_DIR_ENTRY_SIZE))) {
            if (strcmp((char*)file, ".") == 0 || strcmp((char*)file, "..") == 0) continue;
            
            //printFat16TreeEntry((char*)file, depth);
            
            unsigned short cluster;
            fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE) + FAT16_FIRST_CLUSTER_OFFSET, SEEK_SET);
            fread(&cluster, sizeof(short), 1, fd);
            unsigned int cluster_addr = (cluster - 2) * fat16.sectors_per_cluster * fat16.sector_size;
            unsigned int dir_addr = cluster_addr + (fat16.reserved_sectors * fat16.sector_size) + 
                (fat16.num_fats * fat16.sectors_per_fat * fat16.sector_size) + (fat16.root_entries * FAT16_DIR_ENTRY_SIZE);
            searchAndPrint(fat16, fd, dir_addr, depth + 1, filename);
        } 
        else {
            if (strcmp((char*)file, filename) == 0) {
                //printFat16TreeEntry((char*)file, depth);

                // Read the cluster number of the file from the directory entry
                unsigned short cluster;
                fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE) + FAT16_FIRST_CLUSTER_OFFSET, SEEK_SET);
                fread(&cluster, sizeof(short), 1, fd);

                // Calculate the address of the cluster
                unsigned int cluster_addr = (cluster - 2) * fat16.sectors_per_cluster * fat16.sector_size;

                // Calculate the address of the file data within the cluster
                unsigned int file_addr = cluster_addr + (fat16.reserved_sectors * fat16.sector_size) + 
                    (fat16.num_fats * fat16.sectors_per_fat * fat16.sector_size) + (fat16.root_entries * FAT16_DIR_ENTRY_SIZE);

                // read file size
                unsigned int file_size;
                fseek(fd, addr + (i * FAT16_DIR_ENTRY_SIZE) + 28, SEEK_SET); // +28 offset to reach the location where the file size is stored
                fread(&file_size, sizeof(int), 1, fd);
                printf("\nFile size: %d\n\n", file_size);

                // read file data
                unsigned char* file_data = malloc(file_size);
                fseek(fd, file_addr, SEEK_SET);
                fread(file_data, sizeof(char), file_size, fd);
                
                printf("%s\n", file_data);

                free(file_data);
                free(file);
                return;
            }
        }
    }
    free(file);
}

void fat16_cat_file(Fat16_Data fat16, char* path, char* filename) {
    FILE *fd = fopen(path, "rb");

    if (fd == NULL) {
        printf("Failed to open file %s\n", path);
        return;
    }
    
    unsigned int root_dir = fat16.reserved_sectors + (fat16.num_fats * fat16.sectors_per_fat);
    
    searchAndPrint(fat16, fd, root_dir * fat16.sector_size, 0, filename);

    fclose(fd);
}
