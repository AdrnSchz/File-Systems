#ifndef _FAT16_H_
#define _FAT16_H_

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define FAT16_DIR_ENTRY_SIZE 32
#define FAT16_NAME_SIZE 11
#define FAT16_FIRST_CLUSTER_OFFSET 26
typedef struct {
    char system_name[8]; 
    int sector_size;
    int sectors_per_cluster;
    int reserved_sectors;
    
    int num_fats;
    int root_entries;
    int sectors_per_fat;
    char label_name[11];
} Fat16_Data;


int fat16_read_data(char* path, Fat16_Data* fat16);
void fat16_print_tree(Fat16_Data fat16, char* path);
void fat16_cat_file(Fat16_Data fat16, char* path, char* filename);

#endif
