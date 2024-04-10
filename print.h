#ifndef _PRINT_H_
#define _PRINT_H_

#include "ext2.h"
#include "fat16.h"

#define USAGE_ERROR(x) printf("Usage: %s --info <file system>\n", x)
#define OPEN_FILE_ERROR(x) printf("Error: Could not open file %s\n", x)

char* unix_time_to_normal(time_t unix_time);
void printExt2Info(Ext2_Data ext2);
void printFat16Info(Fat16_Data fat16);

#endif