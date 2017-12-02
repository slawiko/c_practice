#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void free_save(char* buffer) {
    if (buffer) {
        free(buffer);
    }
}

unsigned long long read_everything(char** buffer) {
    int c;
    unsigned long long i = 0;

    c = getchar();
    while (c != EOF) {
        if (i == ULONG_LONG_MAX) {
            free_save(*buffer);
            return 0;
        }
        *buffer = (char *)realloc(*buffer, i + 1);
        if (*buffer == NULL) {
            free_save(*buffer);
            return 0;
        }
        (*buffer)[i++] = c;

        c = getchar();
    }

    return i;
}

int main() {
    char* buffer = NULL;
    unsigned long long bytes = read_everything(&buffer);

    printf("%llu\n", bytes);
    printf("%s", buffer);

    free_save(buffer);
    return 0;
}