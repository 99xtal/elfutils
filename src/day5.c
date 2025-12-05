/* jolt -- [AOC 2025 Day 3] Find the largest possible joltage from a set of battery banks.
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(FILE *out, const char *prog) {
    fprintf(out,
        "Usage: %s [OPTION]... [FILE]...\n"
        "\n"
        "Given a FILE containing a list of battery bank joltages, find the largest possible joltage.\n"
        "\n"
        "With no FILE, read standard input.\n"
        "\n"
        "Options:\n"
        "   -n, --number      Specify number of batteries to turn on in each bank (default: 12)\n"
        "   -h, --help        Display this help and exit\n"
        "   -V, --version     Display version information and exit\n",
        prog
    );
}

void free_arr(long** arr, size_t len) {
    for (size_t i = 0; i < len; i++) {
        free(arr[i]);
    }
    free(arr);
}

long solve(FILE *input) {
    size_t capacity = 16;
    size_t len = 0;
    
    long** fresh_ing_ranges = malloc(capacity * sizeof(long*));
    if (!fresh_ing_ranges) {
        fprintf(stderr, "memory allocation failed\n");
        return -1;
    }

    char* line = NULL;
    int linecap = 0;
    int linelen;
    int parsing_ranges = 1;

    long fresh_count = 0;

    while ((linelen = getline(&line, &linecap, input)) != -1) {
        if (parsing_ranges) {
            if (linelen == 0 || line[0] == '\n') {
                parsing_ranges = 0;
                continue;
            }

            // grow array if needed
            if (len >= capacity) {
                capacity *= 2;
                long** new_arr = realloc(fresh_ing_ranges, capacity * sizeof(long*));
                if (!new_arr) {
                    free_arr(fresh_ing_ranges, len);
                    free(line);
                    fprintf(stderr, "realloc failed\n");
                    return -1;
                }
                fresh_ing_ranges = new_arr;
            }

            long l_bound, u_bound;
            if (sscanf(line, "%ld-%ld", &l_bound, &u_bound) != 2) {
                fprintf(stderr, "bad range line: %s", line);
                continue;
            }

            long* arr = malloc(2*sizeof(long));
            arr[0] = l_bound;
            arr[1] = u_bound;
            fresh_ing_ranges[len] = arr;
            len++;
        } else {
            long ingredient = strtol(line, NULL, 10);
            for (size_t i = 0; i < len; i++) {
                long* range = fresh_ing_ranges[i];
                if (ingredient >= range[0] && ingredient <= range[1]) {
                    fresh_count++;
                    break;
                }
            }
        }
    }

    free(line);
    free_arr(fresh_ing_ranges, len);
    return fresh_count;
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
        long answer;
        if ((answer = solve(stdin)) == -1) {
            return EXIT_FAILURE;
        }
        fprintf(stdout, "%ld\n", answer);
    } else {
        FILE *file_ptr;
        long answer;

        for (int i = optind; i < argc; i++) {
            const char *filename = argv[i];

            file_ptr = fopen(filename, "r");
            if (file_ptr == NULL) {
                fprintf(stderr, "error opening file: %s\n", filename);
                return EXIT_FAILURE;
            }

            if ((answer = solve(file_ptr)) == -1) {
                fclose(file_ptr);
                return EXIT_FAILURE;
            }

            fprintf(stdout, "%ld\n", answer);
            fclose(file_ptr);
        }
    }

    return EXIT_SUCCESS;
}
