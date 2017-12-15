#include <stdio.h>
#include <string.h>

#include "calculate.h"
#include "structure.h"
#include "print.h"

int main(int argc, char** argv) {
    int exit_code = 0, tmp_code = 0;
    if (argc == 1) {
        fprintf(stderr, "It's a program for CRC32 checksum calculating. Please specify paths to files, or any text.\n");
        return 2;
    }
    struct file_checksum crc;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-") == 0) {
            tmp_code = calculate(stdin, &crc);
            exit_code = tmp_code != 0 ? tmp_code : exit_code;
        } else {
            FILE* input = fopen(argv[i], "rb");
            if (input == NULL) {
                exit_code = 1;
                fprintf(stderr, "Error with file opening\n");
                continue;
            }
            tmp_code = calculate(input, &crc);
            exit_code = tmp_code != 0 ? tmp_code : exit_code;
            fclose(input);
        }

        crc.path = argv[i];
        print_checksum(crc);
    }

    return exit_code;
}