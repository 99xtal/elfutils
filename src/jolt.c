/* jolt -- [AOC 2025 Day 1 (Part 1)] Find the largest possible joltage from a set of battery banks.
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
        "Given a FILE containing a list of battery bank joltages, find the largest possible joltage.\n"
        "\n"
        "With no FILE, read standard input.\n"
        "\n"
        "Options:\n"
        "   -h, --help        Display this help and exit\n"
        "   -V, --version     Display version information and exit\n",
        prog
    );
}

int get_max_joltage(char* bank) {
    int largest_index = 0;
    int second_largest_index = 0;

    // find largest number in bank for first battery
    int largest_joltage = 0;
    for (int i = 0; bank[i + 1] != '\0'; i++) {
        int joltage = bank[i] - '0';

        if (joltage > largest_joltage) {
            largest_joltage = joltage;
            largest_index = i;
        }
    }

    // find next largest number starting from first battery
    largest_joltage = 0;
    for (int i = largest_index + 1; bank[i] != '\0'; i++) {
        int joltage = bank[i] - '0';

        if (joltage > largest_joltage) {
            largest_joltage = joltage;
            second_largest_index = i;
        }
    }

    return ((bank[largest_index] - '0') * 10) + (bank[second_largest_index] - '0');
}

long solve(FILE *input) {
    char* line = NULL;
    size_t linecap = 0;
    int linelen;

    long total_joltage = 0;

    while ((linelen = getline(&line, &linecap, input)) != -1) {
        if (linelen == 0 || line[0] == '\n') {
            continue;
        }

        // terminate strings at first non-numerical character (\n causes bugs)
        int i = 0;
        while (line[i] - '0' >= 0 && line[i] - '0' <= 9) {
            i++;
        }
        line[i] = '\0';

        int max_joltage = get_max_joltage(line);
        total_joltage += max_joltage;
    }

    free(line);

    return total_joltage;
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
