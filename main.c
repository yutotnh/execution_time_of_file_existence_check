/**
 * @file main.c
 * @author yutotnh (57719497+yutotnh@users.noreply.github.com)
 * @brief Measure the execution time of the function to check if the file exists
 * @version 0.1
 * @date 2023-08-11
 * @note This software is released under the MIT License.
 */

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
 * @
 */
bool is_exist_using_stat(const char* path)
{
    struct stat st;

    int exist = stat(path, &st);

    if (exist == 0) {
        return true;
    }

    return false;
}

/**
 * @brief Check if the file exists using access
 * @param path The path to the file
 * @
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
 */
bool is_exist_using_fopen(const char* path)
{
    FILE* fp = fopen(path, "r");
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
 * @param loop_count The number of times to execute the function
 * @param func_name The name of the function
 */
void execution_time_measure(bool (*func)(const char* path), const char* path, uint32_t loop_count,
                            const char* func_name)
{
    const clock_t start = clock();

    for (uint32_t i = 0; i < loop_count; i++) {
        if (func(path) == false) {
            printf("Error opening file\n");
        }
    }

    const clock_t end = clock();

    printf("%s:%g\n", func_name, (double)(end - start) / CLOCKS_PER_SEC / loop_count);
}

/**
 * @brief Print the usage
 * @param cmd The command name
 */
void usage(const char* cmd)
{
    printf("Usage: %s [FILE] [LOOP_COUNT]\n", cmd);
}

/**
 * @brief Parse the arguments
 * @param argc The number of arguments
 * @param argv The arguments
 * @param path The path to the file
 * @param loop_count The number of times to execute the function
 * @note If the number of arguments is not 3, exit with EXIT_FAILURE
 * @note If the second argument is not a decimal number, exit with EXIT_FAILURE
 */
void parse_args(int argc, char* argv[], char** path, uint32_t* loop_count)
{
    // Check the number of arguments
    if (argc != 3) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get the path to the file
    *path = argv[1];

    // Get the number of times to execute the function
    const char* const str = argv[2];
    char*             end;
    int               si;

    errno = 0;

    const long sl = strtol(str, &end, 10);

    if (end == str) {
        fprintf(stderr, "%s: not a decimal number\n", str);
        exit(EXIT_FAILURE);
    } else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", str, end);
        exit(EXIT_FAILURE);
    } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", str);
        exit(EXIT_FAILURE);
    } else if (sl > INT_MAX) {
        fprintf(stderr, "%ld greater than INT_MAX\n", sl);
        exit(EXIT_FAILURE);
    } else if (sl < INT_MIN) {
        fprintf(stderr, "%ld less thatn INT_MIN\n", sl);
        exit(EXIT_FAILURE);
    } else {
        si = (int)sl;

        if (si <= 0) {
            fprintf(stderr, "%d is less than or equal to 0", si);
            exit(EXIT_FAILURE);
        } else {
            *loop_count = si;
        }
    }
}

int main(int argc, char* argv[])
{
    char*    path       = NULL;
    uint32_t loop_count = 1;

    parse_args(argc, argv, &path, &loop_count);

    execution_time_measure(is_exist_using_stat, path, loop_count, "stat");
    execution_time_measure(is_exist_using_access, path, loop_count, "access");
    execution_time_measure(is_exist_using_fopen, path, loop_count, "fopen");
}
