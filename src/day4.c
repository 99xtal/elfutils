/* jolt -- [AOC 2025 Day 4] 
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

void free_map(char** map, size_t map_height) {
    for (size_t i = 0; i < map_height; i++) {
        free(map[i]);
    }
    free(map);
}

int count_nearby_rolls(char** map, int x, int y, size_t map_width, size_t map_height) {
    int upper_bound = y - 1 < 0 ? 0 : y - 1;
    int lower_bound = y + 1 > map_height - 1 ? map_height - 1 : y + 1;
    int left_bound = x - 1 < 0 ? 0 : x - 1;
    int right_bound = x + 1 > map_width - 1 ? map_width - 1 : x + 1;

    int count = 0;
    for (int j = upper_bound; j <= lower_bound; j++) {
        for (int i = left_bound; i <= right_bound; i++) {
            if (j == y && i == x) continue;

            if (map[j][i] == '@') {
                count++;
            }
        }
    }

    return count;
}

long solve(FILE *input) {
    size_t capacity = 16;
    size_t map_width = 0;
    size_t map_height = 0;

    // initialize an array containing all of the lines of input (representing the map)
    char** map = malloc(capacity * sizeof(char*));
    if (map == NULL) {
        fprintf(stderr, "memory allocation failed\n");
    }

    char* line = NULL;
    size_t linecap = 0;
    int linelen;

    // fill map
    while ((linelen = getline(&line, &linecap, input)) != -1) {
        // remove \n character from line
        if (linelen > 0 && line[linelen - 1] == '\n') {
            line[linelen - 1] = '\0';
            linelen--;
        }

        // grow array if needed
        if (map_height >= capacity) {
            capacity *= 2;
            char** new_map = realloc(map, capacity * sizeof(char*));
            if (map == NULL) {
                free_map(map, map_height);
                return -1;
            }
            map = new_map;
        }

        char* map_line = malloc((linelen + 1) * sizeof(char));
        if (map_line == NULL) {
            fprintf(stderr, "memory allocation failed\n");
        }
        strcpy(map_line, line);
        map[map_height] = map_line;
        
        map_height++;
        map_width = linelen > map_width ? linelen : map_width;
    }

    long accessible_rolls = 0;
    for (size_t y = 0; y < map_height; y++) {
        for (size_t x = 0; x < map_width; x++) {
            if (map[y][x] == '@') {
                int nearby = count_nearby_rolls(map, x, y, map_width, map_height);
                if (nearby < 4) {
                    accessible_rolls++;
                }
                // printf("x: %d, y: %d, count: %d\n", x, y, nearby);
            }
        }
    };

    free(line);
    free_map(map, map_height);
    return accessible_rolls;
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
