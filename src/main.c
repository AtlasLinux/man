#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <page_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* man_path;
    int path_allocated = 0;
    char* env_path = getenv("MAN_PATH");
    
    if (env_path != NULL) {
        man_path = strdup(env_path);
        if (man_path == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            return EXIT_FAILURE;
        }
        path_allocated = 1;
    } else {
        man_path = "/usr/local/man";
    }

    DIR* man_dir = opendir(man_path);
    if (man_dir == NULL) {
        fprintf(stderr, "Error: Cannot open directory %s\n", man_path);
        if (path_allocated) {
            free(man_path);
        }
        return EXIT_FAILURE;
    }

    struct dirent* man_dir_contents;

    char target_file[256];
    snprintf(target_file, sizeof(target_file), "%s.man", argv[1]);

    do {
        man_dir_contents = readdir(man_dir);
        if (man_dir_contents == NULL) {
            fprintf(stderr, "Page %s not found at %s\n", argv[1], man_path);
            closedir(man_dir);
            if (path_allocated) {
                free(man_path);
            }
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
    if (path_allocated) {
        free(man_path);
    }
    return 0;
}