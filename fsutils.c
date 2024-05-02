#include "print.h"

void checkFileSystem(char* path, int* result, Ext2_Data* ext2, Fat16_Data* fat16);
void printInfo(char* path);
void printTree(char* path);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        USAGE_ERROR(argv[0]);
        return 1;
    }

    char* path = argv[2];
    if (argc == 3 && strcmp(argv[1], "--info") == 0) {   
        printInfo(path);
    } else if (argc == 3 && strcmp(argv[1], "--tree") == 0) {    
        printTree(path);
    } else if (argc == 4 && strcmp(argv[1], "--cat") == 0) {
        char* filename = argv[3];
        catFile(path, filename);
    } else {
        USAGE_ERROR(argv[0]);
        return 1;
    }
    return 0;
}

void checkFileSystem(char* path, int* result, Ext2_Data* ext2, Fat16_Data* fat16) {
    *result = ext2_read_data(path, ext2);

    if (*result == 1) {
       OPEN_FILE_ERROR(path);
    } else if (*result == -1) {
        *result = fat16_read_data(path, fat16);
        if (*result == 1) {
            OPEN_FILE_ERROR(path);
        }
    }
}

void printInfo(char* path) {
    Ext2_Data ext2;
    Fat16_Data fat16;
    int result;
    checkFileSystem(path, &result, &ext2, &fat16);

    printf("\n------ Filesystem Information ------\n");
    if (result == 2) {
        printExt2Info(ext2);
    } else if (result == 0) {
        printFat16Info(fat16);
    } else {
        FILE_SYSTEM_ERROR(path);
    }
}

void printTree(char* path) {
    Ext2_Data ext2;
    Fat16_Data fat16;
    int result;
    checkFileSystem(path, &result, &ext2, &fat16);

    if (result == 2) {
        ext2_print_tree(ext2, path);
    } else if (result == 0) {
        fat16_print_tree(fat16, path);
    } else {
        FILE_SYSTEM_ERROR(path);
    }
}

void catFile(char *path, char* filename) {
    Ext2_Data ext2;
    Fat16_Data fat16;
    int result;
    checkFileSystem(path, &result, &ext2, &fat16);

    if (result == 2) {
        //ext2_cat_file(ext2, path, filename);
        FILE_SYSTEM_ERROR(path);
    } else if (result == 0) {
        fat16_cat_file(fat16, path, filename);
    } else {
        FILE_SYSTEM_ERROR(path);
    }
}