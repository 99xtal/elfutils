/* prodval -- [AOC 2025 Day 2] Calculate total distance between two lists of location IDs
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
#include <string.h>

void usage(FILE *out, const char *prog) {
    fprintf(out, 
        "Usage: %s [OPTION]... [FILE]...\n"
        "\n"
        "Calculate the sum of all invalid product IDs in a set of ranges given in FILE.\n"
        "\n"
        "With no FILE, read standard input.\n"
        "\n"
        "Options:\n"
        "   -h, --help      Display this help and exit\n"
        "   -V, --version   Display version information and exit\n",
        prog
    );
}

int has_n_repeats(char* id, size_t n) {
    size_t length = strlen(id);
    char to_check;

    // starting with char at index j
    for (size_t j = 0; j < n; j++) {
        to_check = id[j];

        // check every n number
        for (size_t i = j; i < length; i += n) {
            if (id[i] != to_check) {
                return 0;
            }
        }
    }

    return 1;
}

int is_repeated_at_least_twice(long id) {
    char num_str[20];
    // convert number to string
    sprintf(num_str, "%ld", id);

    size_t length = strlen(num_str);

    for (size_t n = 1; n < length; n++) {
        // check if n is a factor of length
        if (length % n == 0) {
            if (has_n_repeats(num_str, n)) {
                return 1;
            }
        }
    }

    return 0;
}

int is_repeated_twice(long id) {
    char num_str[20];
    // convert number to string
    sprintf(num_str, "%ld", id);

    size_t length = strlen(num_str);

    // not repeated twice if odd number of digits
    if (length % 2 != 0) {
        return 0;
    }

    // compare both halves of number
    for (size_t i = 0; i < length / 2; i++) {
        size_t j = i + (length / 2);
        if (num_str[i] != num_str[j]) {
            return 0;
        }
    }

    return 1;
}

long invalid_id_sum(long lower, long upper) {
    long sum = 0;

    for (long i = lower; i <= upper; i++) {
        if (is_repeated_at_least_twice(i)) {
            sum += i;
        }
    }

    return sum;
}

long solve(FILE *input) {
    char buffer[1024];
    char* delims = "-,";
    char* token;
    long l_bound;
    long u_bound;
    long total_sum = 0;

    if (fgets(buffer, sizeof(buffer), input) == NULL) {
        fprintf(stderr, "error reading input\n");
        return -1;
    }

    // split input by "-" and "," characters
    token = strtok(buffer, delims);
    while (token != NULL) {
        // get lower and upper bound from each range
        l_bound = strtol(token, NULL, 10);
        token = strtok(NULL, delims);
        u_bound = strtol(token, NULL, 10);

        // sum invalid ids in this range
        total_sum += invalid_id_sum(l_bound, u_bound);

        // move on to next range
        token = strtok(NULL, delims);
    }

    return total_sum;
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
