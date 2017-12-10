#include <stdio.h>

#include "print.h"

void print_checksum(struct file_checksum checksum) {
    printf("%lx %s\n", checksum.checksum, checksum.path);
}