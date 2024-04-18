#ifndef _FAT16_H_
#define _FAT16_H_

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

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

#endif
