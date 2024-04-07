#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s --info <file system>\n", argv[0]);
    } 
    
    char* file_system = (argv[2]);
    printf("File system: %s\n", file_system);
    
    return 0;
}