#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 3 && strcmp(argv[1], "--info") == 0) {
        char* file_system = (argv[2]);
        printf("File system: %s\n", file_system);
    } else {
        printf("Usage: %s --info <file system>\n", argv[0]);
    }
    return 0;
}