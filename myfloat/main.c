#include <stdio.h>

struct my_float {
    union {
        float f;

        struct tmp {
            unsigned int mantissa: 23;
            unsigned int exponent: 8;
            unsigned int sign: 1;
        } str;
    };
};

int binary(int n, int i)
{
    int k;
    printf(".");
    for (i--; i >= 0; i--)
    {
        k = n >> i;
        if (k & 1)
            printf("1");
        else
            printf("0");
    }
}

int main() {
    struct my_float f;
    scanf("%f", &f);

    if (f.str.sign == 0) {
        printf("%c", '+');
    } else {
        printf("%c", '-');
    }

    if (f.str.exponent == 0) {
        printf("%c", '0');
        if (f.str.mantissa == 0) {
            return 0;
        } else {
            binary(f.str.mantissa, 23);
            printf("_2 * 2^{%d}", -126);
        }
    } else {
        if (f.str.exponent == 255) {
            printf("INF");
            return 0;
        } else {
            printf("%c", '1');
            binary(f.str.mantissa, 23);
            printf("_2 * 2^{%d}", f.str.exponent - 127);
        }
    }

    return 0;
}
//+0.11011001110001111101110_2 * 2^{-126}
//+0.11011001110001111101110_2 * 2^{-126}