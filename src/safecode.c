/* safecode -- [AOC 2025 Day 1 (Part 1)] Calculate the door code for the North Pole base using safe dial rotations.
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

#define START_POS 50;

void usage(FILE *out, const char *prog) {
    fprintf(out, 
        "Usage: %s [OPTION]... [FILE]...\n"
        "\n"
        "Given a FILE containing a list of safe dial rotations, calculate the door code for the North Pole base using password method 0x434C49434B.\n"
        "\n"
        "With no FILE, read standard input.\n"
        "\n"
        "Options:\n"
        "   -d, --deprecated  Use deprecated password method\n"
        "   -h, --help        Display this help and exit\n"
        "   -V, --version     Display version information and exit\n",
        prog
    );
}

int read_input(FILE *input, int** out_list, size_t* len) {
    size_t length = 0;
    size_t capacity = 16;

    int* list = malloc(capacity * sizeof(int));
    if (list == NULL) {
        free(list);
        fprintf(stderr, "memory allocation failed\n");
        return -1;
    }

    char* line = NULL;
    size_t linecap = 0;
    size_t linelen;

    while ((linelen = getline(&line, &linecap, input)) != -1) {
        if (linelen == 0 || line[0] == '\n') {
            continue;
        }

        char dir = line[0];

        char* p = line + 1;
        char* end;
        long dist = strtol(p, &end, 10);
        if (end == p) {
            continue;
        }

        if (length >= capacity) {
            capacity *= 2;
            int* new_list = realloc(list, capacity * sizeof(int));
            if (new_list == NULL) {
                free(new_list);
                free(list);
                return -1;
            }
            
            list = new_list;
        }      

        if (dir == 'L') {
            list[length] = 0 - dist;
            length++;
        } else if (dir == 'R') {
            list[length] = dist;
            length++;
        }
    }

    free(line);

    *out_list = list;
    *len = length;
    return 0;
}

long solve(FILE *input) {
    long curr_pos = START_POS;
    long zero_cnt = 0;

    int* turns = NULL;
    size_t len = 0;

    if (read_input(input, &turns, &len) == -1) {
        fprintf(stderr, "error reading input\n");
        return -1;
    }

    for (int i = 0; i < len; i++) {
       curr_pos += turns[i];
       if (curr_pos % 100 == 0) {
           zero_cnt++;
       }
    }
     
    return zero_cnt;
}

long solve_secure(FILE *input) {
    long curr_pos = START_POS;
    long next_pos = 0;
    long zero_cnt = 0;

    int* turns = NULL;
    size_t len = 0;

    if (read_input(input, &turns, &len) == -1) {
        fprintf(stderr, "error reading input\n");
        return -1;
    }

    for (int i = 0; i < len; i++) {
        printf("Current Pos: %ld ", curr_pos);
        int turn = turns[i];
        next_pos = curr_pos + turn;

        if (next_pos < 0) {
            while (next_pos < 0) {
                next_pos += 100;
                zero_cnt++;
            }

            if (next_pos == 0) zero_cnt++;
        } else if (next_pos >= 100) {
            while (next_pos >= 100) {
                next_pos -= 100;
                zero_cnt++;
            }
        } else if (next_pos == 0) {
            zero_cnt++;
        }

        curr_pos = next_pos;

        printf("Turn: %d, Next Pos: %ld, Zeros: %ld\n", turns[i], curr_pos, zero_cnt);
    }

    return zero_cnt;
}

int main(int argc, char **argv) {
    const char *prog = argv[0];

    static struct option long_opts[] = {
        {"deprecated", no_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'}
    };

    int opt;
    int opt_index = 0;
    int deprecated = 0;

    const char *short_opts = "dhV";

    while ((opt = getopt_long(argc, argv, short_opts, long_opts, &opt_index)) != -1) {
        switch (opt) {
            case 'd':
                deprecated = 1;
                break;
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

    long (*solver)(FILE*);
    if (deprecated) {
        solver = &solve;
    } else {
        solver = &solve_secure;
    }

    if (optind == argc) {
        long answer;
        if ((answer = solver(stdin)) == -1) {
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

            if ((answer = solver(file_ptr)) == -1) {
                fclose(file_ptr);
                return EXIT_FAILURE;
            }

            fprintf(stdout, "%ld\n", answer);
            fclose(file_ptr);
        }
    }

    return EXIT_SUCCESS;
}
