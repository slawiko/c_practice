#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int N;
    FILE* input = fopen("input.bin", "rb");
    fread(&N, 4, 1, input);
    const int matrix_size = N * N;
    const int unused_bytes = 8;
    const int BLOCK = 2;
    const int block_size = BLOCK * BLOCK;

    unsigned char* a = (unsigned char *)malloc(block_size);
    unsigned char* b = (unsigned char *)malloc(block_size);
    unsigned char* c = (unsigned char *)malloc(block_size);

    FILE* output = fopen("output.bin", "wb");
    fwrite(&N, 4, 1, output);
    fwrite(&N, 4, 1, output);

    for (int i = 0; i < N; i += BLOCK) {
        for (int j = 0; j < N; j += BLOCK) {
            for (int k = 0; k < N; k += BLOCK) {
                for (int l = 0; l < BLOCK; ++l) {
                    fseek(input, N * i + k + N * l + unused_bytes, SEEK_SET);
                    fread(a + BLOCK * l, BLOCK, 1, input);
                    fseek(input, N * k + j + N * l + unused_bytes * 2 + matrix_size, SEEK_SET);
                    fread(b + BLOCK * l, 1, BLOCK, input);
                }

                for (int ib = 0; ib < BLOCK; ++ib) {
                    for (int jb = 0; jb < BLOCK; ++jb) {
                        if (k == 0) {
                            c[ib * BLOCK + jb] = 0;
                        }
                        for (int kb = 0; kb < BLOCK; ++kb) {
                            c[ib * BLOCK + jb] += a[ib * BLOCK + kb] * b[kb * BLOCK + jb];
                        }
                    }
                }
            }

            for (int l = 0; l < BLOCK; ++l) {
                fseek(output, N * i +  j + N * l + unused_bytes, SEEK_SET);
                fwrite(c + BLOCK * l, 1, BLOCK, output);
            }
        }
    }

    free(a);
    free(b);
    free(c);
    fclose(input);
    fclose(output);

//    unsigned char a[4 * 4] = { 2, 0, 0, 1, 1, 0, 0, 0, 3, 4, 0, 2, 0, 5, 3, 1 };
//    unsigned char b[4 * 4] = { 3, 0, 1, 2, 4, 4, 0, 1, 0, 3, 2, 1, 2, 4, 1, 1 };
//
//    size_t M = 4;
//    FILE* output = fopen("input.bin", "wb");
//    fwrite(&M, 4, 1, output);
//    fwrite(&M, 4, 1, output);
//    fwrite(&a, 1, M * M, output);
//
//    fwrite(&M, 4, 1, output);
//    fwrite(&M, 4, 1, output);
//    fwrite(&b, 1, M * M, output);
//
//    fclose(output);
    return 0;
}