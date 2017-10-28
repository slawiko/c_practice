#include <stdio.h>
#include <stdlib.h>

int main() {
    int N;
    int M;
    FILE* input = fopen("input.bin", "rb");
    fread(&N, 4, 1, input);
    fread(&M, 4, 1, input);
    const int unused_bytes = 8;
    const int BLOCK = 4;
    const int block_size = BLOCK * BLOCK;
    int block_N = BLOCK;
    int block_M = BLOCK;

    unsigned char* a = (unsigned char*)malloc(block_size);

    FILE* output = fopen("output.bin", "wb");
    fwrite(&M, 4, 1, output);
    fwrite(&N, 4, 1, output);

    for (int i = 0; i < N; i += BLOCK) {
        block_N = BLOCK < (N - i) ? BLOCK : (N - i);
        for (int j = 0; j < M; j += BLOCK) {
            block_M = BLOCK < (M - j) ? BLOCK : (M - j);
            for (int l = 0; l < block_N; ++l) {
                fseek(input, M * i + j + M * l + unused_bytes, SEEK_SET);
                fread(a + BLOCK * l, 1, block_M, input);
            }

            for (int jb = 0; jb < block_M; jb++) {
                for (int ib = 0; ib < block_N; ib++) {
                    fseek(output, N * (j + jb) + (i + ib) + unused_bytes, SEEK_SET);
                    fwrite(a + ib * BLOCK + jb, 1, 1, output);
                }
            }
        }
    }

    free(a);
    fclose(input);
    fclose(output);

//    unsigned char a[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0};
//    int N = 2;
//    int M = 6;
//
//    FILE* input = fopen("input.bin", "wb");
//    fwrite(&N, 4, 1, input);
//    fwrite(&M, 4, 1, input);
//    fwrite(&a, 1, N * M, input);
//
//    fclose(input);

    return 0;
}