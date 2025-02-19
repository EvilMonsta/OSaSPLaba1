#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <locale.h> 
#include <getopt.h>

#define MAX_FILES 10000

int sort_output = 0;
int show_links = 0;
int show_dirs = 0;
int show_files = 0;

int compare(const void *a, const void *b) {
    return strcoll(*(const char **)a, *(const char **)b);
}

int match_type(struct stat *sb) {
    if((show_links && S_ISLNK(sb->st_mode)) ||
    (show_dirs && S_ISDIR(sb->st_mode))     ||
    (show_files && S_ISREG(sb->st_mode))) {
        return 1;
    }
    return !(show_links || show_dirs || show_files);
}

int dirwalk(char *path, char **files, int *count) {
    DIR *d;
    struct dirent *dir;
    struct stat sb;
    char fullpath[MAXPATHLEN];

    if ((d = opendir(path)) == NULL) {
        perror("opendir");
        return 0;
    }

    while ((dir = readdir(d))) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, dir->d_name);

        if (lstat(fullpath, &sb) == -1) {
            perror("lstat");
            continue;
        }

        if (match_type(&sb)) {
            files[*count] = strdup(fullpath);
            if(files[*count] == NULL)  {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            (*count)++;
        }

        if(S_ISDIR(sb.st_mode)) {
            dirwalk(fullpath, files, count);
        }
    }
    closedir(d);
    return 0;
}

int main(int argc, char *argv[]) {
    setlocale(LC_COLLATE, "");

    char *files[MAX_FILES];
    int count = 0;
    char *dir_path = "./";

    int opt;
    while ((opt = getopt(argc, argv, "sldf")) != -1) {
        switch (opt) {
            case 's':
                sort_output = 1;
                break;
            case 'l':
                show_links = 1;
                break;
            case 'd':
                show_dirs = 1;
                break;
            case 'f':
                show_files = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-s] [-l] [-d] [-f] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        dir_path = argv[optind];
    }

    dirwalk(dir_path, files, &count);

    if (sort_output) {
        qsort(files, count, sizeof(char *), compare);
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", files[i]);
        free(files[i]);
    }

    return 0;
}





