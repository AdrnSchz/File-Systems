#include <stdio.h>
#include <stdint.h>
#include <time.h>

void unix_time_to_normal(time_t unix_time) {
    struct tm *local_time;
    char buffer[80];

    // Convert Unix time to local time
    local_time = localtime(&unix_time);

    // Format the local time
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", local_time);

    printf("Normal Time: %s\n", buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s --info <file system>\n", argv[0]);
        return 1;
    }
     
    FILE *fs = fopen(argv[2], "rb");
    if (fs == NULL) {
        perror("Failed to open file system image");
        return 1;
    }

    // Read 1024 bytes = 1 block
    // ================================================================
    // INODE INFO  
    // ================================================================
    int inode_size = 0;
    fseek(fs, 1024 + 88, SEEK_SET);
    fread(&inode_size, sizeof(int), 1, fs);

    int num_inodes = 0; 
    fseek(fs, 1024, SEEK_SET);
    fread(&num_inodes, sizeof(int), 1, fs);

    int first_inode = 0;
    fseek(fs, 1024 + 84, SEEK_SET);
    fread(&first_inode, sizeof(int), 1, fs);

    int inodes_group = 0;
    fseek(fs, 1024 + 40, SEEK_SET);
    fread(&inodes_group, sizeof(int), 1, fs);

    int free_inodes = 0;
    fseek(fs, 1024 + 16, SEEK_SET);
    fread(&free_inodes, sizeof(int), 1, fs);

    // ================================================================
    // INFO BLOCK
    // ================================================================

    // FALTA EL BLOCK SIZE !!!

    int reserved_blocks = 0;
    fseek(fs, 1024 + 8, SEEK_SET);
    fread(&reserved_blocks, sizeof(int), 1, fs);

    int free_blocks = 0;
    fseek(fs, 1024 + 12, SEEK_SET);
    fread(&free_blocks, sizeof(int), 1, fs);

    int total_blocks = 0;
    fseek(fs, 1024 + 4, SEEK_SET);
    fread(&total_blocks, sizeof(int), 1, fs);

    int first_block = 0;
    fseek(fs, 1024 + 20, SEEK_SET);
    fread(&first_block, sizeof(int), 1, fs);

    int groups_block = 0;
    fseek(fs, 1024 + 32, SEEK_SET);
    fread(&groups_block, sizeof(int), 1, fs);

    int groups_flags = 0;
    fseek(fs, 1024 + 36, SEEK_SET);
    fread(&groups_flags, sizeof(int), 1, fs);

    // ================================================================
    // INFO VOLUME
    // ================================================================
    char volume_name[16];
    fseek(fs, 1024 + 120, SEEK_SET);
    fread(&volume_name, sizeof(char) * 16, 1, fs);
    
    int last_checked = 0;
    fseek(fs, 1024 + 64, SEEK_SET);
    fread(&last_checked, sizeof(int), 1, fs);

    int last_mounted = 0;
    fseek(fs, 1024 + 44, SEEK_SET);
    fread(&last_mounted, sizeof(int), 1, fs);

    int last_written = 0;
    fseek(fs, 1024 + 48, SEEK_SET);
    fread(&last_written, sizeof(int), 1, fs);

    printf("Inode size = %d\n", inode_size);
    printf("Number of inodes = %d\n", num_inodes);
    printf("First inode = %d\n", first_inode);
    printf("Inodes per group = %d\n", inodes_group);
    printf("Free inodes = %d\n", free_inodes);

    printf("\n");
    
    printf("Reserved blocks = %d\n", reserved_blocks);
    printf("Free blocks = %d\n", free_blocks);
    printf("Total blocks = %d\n", total_blocks);
    printf("First block = %d\n", first_block);
    printf("Groups block = %d\n", groups_block);
    printf("Groups flags = %d\n", groups_flags);

    printf("\n");

    unix_time_to_normal((time_t)(last_checked));
    unix_time_to_normal((time_t)(last_mounted));
    unix_time_to_normal((time_t)(last_written));

    printf("Volume name = %s\n", volume_name);
    printf("Last checked = %d\n", last_checked);
    printf("Last mounted = %d\n", last_mounted);
    printf("Last written = %d\n", last_written);


    fclose(fs);
    
    return 0;
}