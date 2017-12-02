#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(handle) setmode(handle, O_BINARY)
#else
# define SET_BINARY_MODE(handle) ((void)0)
#endif

int digit_length(unsigned long long x) {
    int counter = 0;

    if (x == 0) {
        return 1;
    }
    while(x != 0) {
        x /= 10;
        ++counter;
    }

    return counter;
}

int max_in_three(int x, int y, int z) {
    int max = x;
    if (y > max) {
        max = y;
    }
    if (z > max) {
        max = z;
    }
    return max;
}

int main() {
    unsigned long long line_count = 0;
    unsigned long long word_count = 0;
    unsigned long long byte_count = 0;

    int cur;
    bool in_word = false;

    SET_BINARY_MODE(fileno(stdin));

    cur = getchar();
    while (cur != EOF) {
        byte_count++;

        if (cur == '\n') {
            line_count++;
        }

        if (!in_word && isprint(cur) && !isspace(cur)) {
            in_word = true;
        } else if (in_word && isspace(cur)) {
            in_word = false;
            word_count++;
        }

        cur = getchar();
    }
    if (in_word) {
        word_count++;
    }

    int l = digit_length(line_count);
    int w = digit_length(word_count);
    int b = digit_length(byte_count);
    int max = max_in_three(l, w, b);

    printf("%*s%llu %*s%llu %*s%llu\n", max - l, "", line_count, max - w, "", word_count, max - b, "", byte_count);

    return 0;
}