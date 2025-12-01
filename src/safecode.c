/* safecode -- [AOC 2025 Day 1 (Part 1)] Calculate total distance between two lists of location IDs
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

#define START_POS 50;

void usage(FILE *out, const char *prog) {
    fprintf(out, 
        "Usage: %s [OPTION]... [FILE]...\n"
        "\n"
        "Given a FILE containing a list of safe dial rotations, calculate the number of times a rotation lands on 0.\n"
        "\n"
        "With no FILE, read standard input.\n"
        "\n"
        "Options:\n"
        "   -m, --method    Use password method 0x434C49434B\n"
        "   -h, --help      Display this help and exit\n"
        "   -V, --version   Display version information and exit\n",
        prog
    );
}

long solve(FILE *input) {
    long curr_pos = START_POS;
    long zero_cnt = 0;

    char* line = NULL;
    size_t linecap = 0;
    size_t linelen;

    while ((linelen = getline(&line, &linecap, input)) != -1) {
        // skip blank lines
        if (linelen == 0 || line[0] == '\n') {
            continue;
        }

        char dir = line[0];

        char* p = line + 1;
        char* end;
        long dist = strtol(p, &end, 10);
        if (end == p) {
            printf("invalid dist");
            // invalid distance
            continue;
        }

        if (dir == 'L') {
            curr_pos -= dist;
        } else if (dir == 'R') {
            curr_pos += dist;
        }

        if (curr_pos % 100 == 0) {
            zero_cnt += 1;
        }
    }

    return zero_cnt;
}

int main(int argc, char **argv) {
    const char *prog = argv[0];

    static struct option long_opts[] = {
        {"method", no_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'}
    };

    int opt;
    int opt_index = 0;
    int m = 0;

    const char *short_opts = "mhV";

    while ((opt = getopt_long(argc, argv, short_opts, long_opts, &opt_index)) != -1) {
        switch (opt) {
            case 'm':
                m = 1;
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