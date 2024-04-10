#ifndef _FAT16_H_
#define _FAT16_H_

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    char system_name[9]; 
    char volume_name[12]; 
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    
    uint8_t num_fats;
    uint16_t root_entries;
    uint16_t sectors_per_fat;
    
} Fat16_Data;


int fat16_read_data(char* path, Fat16_Data* fat16);

#endif
