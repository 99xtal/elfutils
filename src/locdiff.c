/* locdiff -- [AOC 2024 Day 1 (Part 1)] Calculate total distance between two lists of location IDs
   Copyright (C) 2025 99xtal

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void usage(FILE *out, const char *prog) {
    fprintf(out, 
        "Usage: %s [OPTION]... [FILE]...\n"
        "\n"
        "Calculate the total distance between two lists of location IDs contained in each FILE.\n"
        "\n"
        "With no FILE, read standard input.\n"
        "\n"
        "Options:\n"
        "   -h, --help      Display this help and exit\n"
        "   -V, --version   Display version information and exit\n",
        prog
    );
}

int read_input(FILE *input, long **left_list, long **right_list, size_t *len) {
    size_t capacity = 16;
    size_t length = 0;

    long *left = malloc(capacity * sizeof(long));
    long *right = malloc(capacity * sizeof(long));
    if (!left || !right) {
        free(left);
        free(right);
        fprintf(stderr, "memory allocation failed\n");
        return -1;
    }

    char *line = NULL;
    size_t linecap = 0;
    size_t linelen;

    while ((linelen = getline(&line, &linecap, input)) != -1) {
        // skip blank lines
        if (linelen == 0 || line[0] == '\n') {
            continue;
        }

        // grow arrays if needed
        if (length >= capacity) {
            capacity *= 2;
            long *new_left = realloc(left, capacity * sizeof(long));
            long *new_right = realloc(right, capacity * sizeof(long));
            if (!new_left || !new_right) {
                free(new_left);
                free(new_right);
                free(left);
                free(right);
                free(line);
                fprintf(stderr, "realloc failed\n");
                return -1;
            }
            left = new_left;
            right = new_right;
        }

        char *p = line;
        char *end;

        long a = strtol(p, &end, 10);
        if (end == p) {
            free(left);
            free(right);
            free(line);
            fprintf(stderr, "failed to parse first number: %s\n", line);
            return -1;
        }

        p = end;
        while (*p == ' ') {
            p++;
        }

        long b = strtol(p, &end, 10);
        if (end == p) {
            free(left);
            free(right);
            free(line);
            fprintf(stderr, "failed to parse second number: %s\n", line);
            return -1;
        }

        left[length] = a;
        right[length] = b;
        length++;
    }

    free(line);

    *left_list = left;
    *right_list = right;
    *len = length;
    return 0;
}

int sort_asc(const void *a, const void *b) {
    return (*(long *)a - *(long *)b);
}

long solve(FILE *input) {
    long *left = NULL;
    long *right = NULL;
    size_t n = 0;

    if (read_input(input, &left, &right, &n) != 0) {
        fprintf(stderr, "error reading input\n");
        return -1;
    }

    qsort(left, n, sizeof(long), sort_asc);
    qsort(right, n, sizeof(long), sort_asc);

    long total_distance = 0;

    for (size_t i = 0; i < n; i++) {
        total_distance += abs(left[i] - right[i]);
    }

    free(left);
    free(right);

    return total_distance;
}

int main(int argc, char **argv) {
    const char *prog = argv[0];

    static struct option long_opts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'}
    };

    int opt;
    int opt_index = 0;

    const char *short_opts = "hV";

    while ((opt = getopt_long(argc, argv, short_opts, long_opts, &opt_index)) != -1) {
        switch (opt) {
            case 'h':
                usage(stdout, prog);
                return EXIT_SUCCESS;
            case 'V':
                printf("%s 0.1.0\n", prog);
                return EXIT_SUCCESS;
            default:
                usage(stderr, prog);
                return EXIT_FAILURE;
        }
    }

    if (optind == argc) {
        long total_distance;
        if ((total_distance = solve(stdin)) == -1) {
            return EXIT_FAILURE;
        }
        fprintf(stdout, "%ld\n", total_distance);
    } else {
        FILE *file_ptr;
        long total_distance;

        for (int i = optind; i < argc; i++) {
            const char *filename = argv[i];

            file_ptr = fopen(filename, "r");
            if (file_ptr == NULL) {
                fprintf(stderr, "error opening file: %s\n", filename);
                return EXIT_FAILURE;
            }

            if ((total_distance = solve(file_ptr)) == -1) {
                fclose(file_ptr);
                return EXIT_FAILURE;
            }

            fprintf(stdout, "%ld\n", total_distance);
            fclose(file_ptr);
        }
    }

    return EXIT_SUCCESS;
}
