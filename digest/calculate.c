#include <stdio.h>
#include <zlib.h>
#include <stdlib.h>

#include "calculate.h"

int calculate(FILE* input, struct file_checksum* crc) {
    const uInt buffer_length = 10000;
    uInt tmp_buf_len;
    Bytef* buffer = (Bytef*) malloc(buffer_length * sizeof(Bytef));
    if (buffer == NULL) {
        return 1;
    }

    crc->checksum = crc32(0L, Z_NULL, 0);

    while ((tmp_buf_len = fread(buffer, 1, buffer_length, input)) > 0) {
        crc->checksum = crc32(crc->checksum, buffer, tmp_buf_len);
    }

    free(buffer);
    return 0;
}