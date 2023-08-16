/**
 * @file main.c
 * @author yutotnh (57719497+yutotnh@users.noreply.github.com)
 * @brief Measure the execution time of the function to check if the file exists
 * @version 0.1
 * @date 2023-08-11
 * @note This software is released under the MIT License.
 */

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Check if the file exists using stat
 * @param path The path to the file
 * @return true The file exists
 */
bool is_exist_using_stat(const char* path)
{
    struct stat st;

    if (stat(path, &st) == -1) {
        return false;
    }

    if (!S_ISREG(st.st_mode)) {
        return false;
    }

    if (!(st.st_mode & S_IRUSR)) {
        return false;
    }

    return true;
}

/**
 * @brief Check if the file exists using access
 * @param path The path to the file
 * @return true The file exists
 */
bool is_exist_using_access(const char* path)
{
    if (access(path, R_OK) == -1) {
        return false;
    }

    return true;
}

/**
 * @brief Check if the file exists using fopen
 * @param path The path to the file
 * @return true The file exists
 */
bool is_exist_using_fopen(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);

    return true;
}

/**
 * @brief Measure the execution time of the function
 * @param func The function to measure
 * @param path The path to the file
 * @param file_count The number of times to execute the function
 * @param func_name The name of the function
 */
void execution_time_measure(bool (*func)(const char* path), char** paths, uint32_t file_count, const char* func_name)
{
    const clock_t start = clock();

    for (uint32_t i = 0; i < file_count; i++) {
        if (func(paths[i]) == false) {
            fprintf(stderr, "Error: %s", paths[i]);
        }
    }

    const clock_t end = clock();

    printf("%s:%g\n", func_name, (double)(end - start) / CLOCKS_PER_SEC / file_count);
}

/**
 * @brief Print the usage
 * @param cmd The command name
 */
void usage(const char* cmd)
{
    printf("Usage: %s [DIR]\n", cmd);
}

/**
 * @brief Parse the arguments
 * @param argc The number of arguments
 * @param argv The arguments
 * @param path The path to the directory
 * @note If the number of arguments is not 3, exit with EXIT_FAILURE
 * @note If the second argument is not a decimal number, exit with EXIT_FAILURE
 */
void parse_args(int argc, char* argv[], char** path)
{
    // Check the number of arguments
    if (argc != 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get the path to the file
    *path = argv[1];

    // Check if the directory exists
    struct stat st;
    if (stat(*path, &st) == -1) {
        fprintf(stderr, "%s is not exists", *path);
        exit(EXIT_FAILURE);
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "%s is not a directory", *path);
    }
}

void list_of_files_in_directory(const char* dir_path, char*** file_paths, uint32_t* file_count)
{
    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory");
        exit(EXIT_FAILURE);
    }

    // Get the number of files
    *file_count          = 0;
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        char        path[PATH_MAX + 1];
        snprintf(path, PATH_MAX + 1, "%s/%s", dir_path, entry->d_name);
        if ((stat(path, &st) == 0) && S_ISREG(st.st_mode)) {
            (*file_count)++;
        }
    }

    if (*file_count == 0) {
        fprintf(stderr, "No files in directory");
        exit(EXIT_FAILURE);
    }

    // Get the file paths
    // char paths[*file_count][PATH_MAX + 1];
    *file_paths = (char**)malloc(sizeof(char*) * *file_count);

    rewinddir(dir);
    uint32_t i = 0;
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        char        path[PATH_MAX + 1];
        snprintf(path, PATH_MAX + 1, "%s/%s", dir_path, entry->d_name);
        if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
            (*file_paths)[i] = (char*)malloc(sizeof(char) * (PATH_MAX + 1));
            snprintf((*file_paths)[i], PATH_MAX + 1, "%s", path);
            i++;
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    char* dir_path = NULL;

    parse_args(argc, argv, &dir_path);

    char**   file_paths = NULL;
    uint32_t file_count = 0;
    list_of_files_in_directory(dir_path, &file_paths, &file_count);

    execution_time_measure(is_exist_using_stat, file_paths, file_count, "stat");
    execution_time_measure(is_exist_using_access, file_paths, file_count, "access");
    execution_time_measure(is_exist_using_fopen, file_paths, file_count, "fopen");
}
