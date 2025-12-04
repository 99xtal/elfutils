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

long long get_max_joltage(char* bank, int num_batteries) {
    int battery_pos[num_batteries];
    size_t bank_len = strlen(bank);

    int curr_battery = 0;

    while (curr_battery < num_batteries) {
        int largest_index = 0;
        int largest_joltage = 0;

        // next loc after previously selected battery
        int first_pos_to_check = curr_battery == 0 ? 0 : battery_pos[curr_battery - 1] + 1;
        int last_pos_to_check = bank_len - num_batteries + curr_battery;

        for (int i = first_pos_to_check; i <= last_pos_to_check; i++) {
            int joltage = bank[i] - '0';

            if (joltage > largest_joltage) {
                largest_joltage = joltage;
                largest_index = i;
            }
        }

        battery_pos[curr_battery] = largest_index;
        curr_battery++;
    }


    long long max_joltage = 0;
    for (int i = 0; i < num_batteries; i++) {
        int battery_val = bank[battery_pos[num_batteries - i - 1]] - '0';
        max_joltage += battery_val * pow(10, i);
    }

    return max_joltage;
}

long long solve(FILE *input, int num_batteries) {
    char* line = NULL;
    size_t linecap = 0;
    int linelen;

    long long total_joltage = 0;

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

        long long max_joltage = get_max_joltage(line, num_batteries);
        total_joltage += max_joltage;
    }

    free(line);

    return total_joltage;
}

int main(int argc, char **argv) {
    const char *prog = argv[0];

    static struct option long_opts[] = {
        {"number", required_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'}
    };

    int opt;
    int opt_index = 0;
    int num_batteries = 12;

    const char *short_opts = "n:hV";

    while ((opt = getopt_long(argc, argv, short_opts, long_opts, &opt_index)) != -1) {
        switch (opt) {
            case 'n':
                num_batteries = atoi(optarg);
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

    if (optind == argc) {
        long long answer;
        if ((answer = solve(stdin, num_batteries)) == -1) {
            return EXIT_FAILURE;
        }
        fprintf(stdout, "%lld\n", answer);
    } else {
        FILE *file_ptr;
        long long answer;

        for (int i = optind; i < argc; i++) {
            const char *filename = argv[i];

            file_ptr = fopen(filename, "r");
            if (file_ptr == NULL) {
                fprintf(stderr, "error opening file: %s\n", filename);
                return EXIT_FAILURE;
            }

            if ((answer = solve(file_ptr, num_batteries)) == -1) {
                fclose(file_ptr);
                return EXIT_FAILURE;
            }

            fprintf(stdout, "%lld\n", answer);
            fclose(file_ptr);
        }
    }

    return EXIT_SUCCESS;
}
