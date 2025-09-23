#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    char* man_path = (getenv("MAN_PATH") != NULL) ? strdup(getenv("MAN_PATH")) : "/usr/local/man";

    DIR* man_dir = opendir(man_path);

    struct dirent* man_dir_contents;

    char target_file[256];
    snprintf(target_file, sizeof(target_file), "%s.man", argv[1]);

    do {
        man_dir_contents = readdir(man_dir);
        if (man_dir_contents == NULL) {
            fprintf(stderr, "Page %s not found at %s\n", argv[1], man_path);
            exit(EXIT_FAILURE);
        }
    } while (strcmp(man_dir_contents->d_name, target_file) != 0);

    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s/%s", man_path, man_dir_contents->d_name);
    FILE* file = fopen(file_path, "r");

    char output[1024];
    while (fgets(output, sizeof(output), file) != NULL) {
        printf(output);
    }

    fclose(file);
    closedir(man_dir);
    return 0;
}